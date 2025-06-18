#version 460 core

in vec3 oPos;
in vec2 oTexCoord;
in vec3 oNormal;
in vec4 oShadowProjPos;

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
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 cameraPosition;
uniform GlobalLight globalLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;

uniform sampler2D shadowMap;

float calculate_shadow_factor(sampler2D shadowMap) {
    vec3 projCoords = oShadowProjPos.xyz / oShadowProjPos.w;
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

    float shadowFactor = calculate_shadow_factor(shadowMap);
    return shadowFactor * (ambient + diffuse + specular) * directionalLight.intensity;
}

vec3 spot_lgiht() {
    return vec3(0.0);
}


void main() {
	vec4 ambient = vec4(globalLight.ambient * globalLight.intensity, 1.0);
    vec4 lights = vec4(directional_light(), 1.0);
	FragColor = ambient * lights;
}