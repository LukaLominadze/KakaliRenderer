#version 460 core

in vec3 oPos;
in vec2 oTexCoord;
in vec3 oNormal;
in vec4 oDirShadowProjPos;
in vec4 oSpotShadowProjPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;

struct GlobalLight {
	vec3 ambient;
	float intensity;
};

struct DirectionalLight {
	vec3 ambient;
	float intensity;
	vec3 direction;
	vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 ambient;
    float intensity;
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    float cutoffAngle;
    float outerAngle;
};

uniform vec3 cameraPosition;
uniform GlobalLight globalLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;

uniform sampler2D directionalLight_shadowMap;
uniform sampler2D spotLight_shadowMap;

float calculate_shadow_factor(sampler2D shadowMap, vec4 projpos) {
    vec3 projCoords = projpos.xyz / projpos.w;
    vec2 UVCoords = vec2(0.5 * projCoords.x + 0.5, 0.5 * projCoords.y + 0.5);
    float z = 0.5 * projCoords.z + 0.5;

    if (UVCoords.x < 0.0 || UVCoords.x > 1.0 || UVCoords.y < 0.0 || UVCoords.y > 1.0)
        return 1.0;

    float depth = texture(shadowMap, UVCoords).x;

    float bias = max(0.0025 * (1.0 - dot(oNormal, directionalLight.direction)), 0.0005);

    float shadow = 0.0;
    float texelSize = 1.0 / textureSize(shadowMap, 0).x;

    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(shadowMap, UVCoords + offset).r;
            shadow += (sampleDepth + bias < z) ? 0.33 : 1.0;
        }
    }

    return shadow / 25.0;
}

vec3 directional_light()
{
    vec3 viewDir = normalize(cameraPosition - oPos);

    vec3 lightDir = normalize(-directionalLight.direction);
 
    float diff = max(dot(oNormal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, oNormal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 120.0);

    vec3 ambient  = directionalLight.ambient  * vec3(texture(texture_diffuse0, oTexCoord));
    vec3 diffuse  = directionalLight.diffuse  * diff * vec3(texture(texture_diffuse0, oTexCoord));
    vec3 specular = directionalLight.specular * spec * vec3(texture(texture_specular0, oTexCoord));

    float shadowFactor = calculate_shadow_factor(directionalLight_shadowMap, oDirShadowProjPos);
    return shadowFactor * (ambient + diffuse + specular) * directionalLight.intensity;
}

vec3 spot_light() {
    vec3 lightDir = normalize(oPos - spotLight.position);
    float theta = dot(lightDir, normalize(-spotLight.direction)); 
    if (theta > spotLight.outerAngle) {
        vec3 ambient = spotLight.ambient * spotLight.intensity;

        float brightness = max(dot(normalize(-spotLight.direction), oNormal), 0.0);
        vec3 diffused = spotLight.ambient * brightness * spotLight.intensity;

        vec3 reflected = reflect(-normalize(spotLight.direction), oNormal);
        vec3 eyeVector = normalize(cameraPosition - oPos);
        float spec = pow(max(dot(reflected, eyeVector), 0.0), 120.0f);
        vec3 specular = vec3(spec);

        float epsilon = spotLight.cutoffAngle - spotLight.outerAngle;
        float lightIntensity = clamp((theta - spotLight.outerAngle) / epsilon, 0.0, 1.0);

        float shadowFactor = calculate_shadow_factor(spotLight_shadowMap, oSpotShadowProjPos);

        vec3 result = shadowFactor * ((ambient + diffused + specular) * lightIntensity);

        return result;
    }

    return vec3(0.0);
}


void main() {
	vec4 ambient = vec4(globalLight.ambient * globalLight.intensity, 1.0);
    vec4 lights = vec4(directional_light() + spot_light(), 1.0);
	FragColor = ambient * lights;
}