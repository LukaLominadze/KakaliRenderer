#version 460 core

in vec3 oPos;
in vec2 oTexCoord;
in vec3 oNormal;
in vec4 oDirShadowProjPos;
in vec4 oSpotShadowProjPos;
in vec3 oWorldPos;

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

struct PointLight {
    vec3 ambient;
    float intensity;
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    float radius;
    float farPlane;
};

uniform vec3 cameraPosition;
uniform GlobalLight globalLight;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform PointLight pointLight;

uniform sampler2D directionalLight_shadowMap;
uniform sampler2D spotLight_shadowMap;
uniform samplerCube pointLight_shadowMap;

// array of offset direction for sampling
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

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

float calculate_point_shadow_factor(vec3 lightToPixel) {
    float dist = length(lightToPixel);

    float sampledDist = texture(pointLight_shadowMap, lightToPixel).r;

    vec3 lightDir = normalize(lightToPixel);
    
    float bias = 0.15;
    int samples = 20;
    float shadow = samples;
    float viewDistance = length(pointLight.position - oPos);
    float diskRadius = (1.0 + (viewDistance / pointLight.farPlane)) / 100.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(pointLight_shadowMap, lightToPixel + gridSamplingDisk[i] * diskRadius).r;
        if(closestDepth + bias < dist)
            shadow -= 1.0;
    }
    shadow /= float(samples);

    return shadow;
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
        vec3 diffused = spotLight.diffuse * brightness * spotLight.intensity;

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

vec3 point_light() {
    float d = distance(pointLight.position, oPos);
    vec3 dist = oWorldPos - pointLight.position;

    float shadowFactor = calculate_point_shadow_factor(dist);
    
    //TODO: better attenuation
    float attenuation = clamp(1.0 - pow(d / pointLight.radius, 3.0), 0.0, 1.0);

    // Lighting vectors
    vec3 lightDir = normalize(pointLight.position - oPos);
    vec3 normal = normalize(oNormal);
    vec3 eyeDir = normalize(cameraPosition - oPos);

    // Ambient
    vec3 ambient = pointLight.ambient * attenuation * pointLight.intensity;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * attenuation * pointLight.intensity;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, eyeDir), 0.0), 64.0);
    vec3 specular = pointLight.specular * spec * attenuation * pointLight.intensity;

    return shadowFactor * (ambient + diffuse + specular);
}


void main() {
	vec4 ambient = vec4(globalLight.ambient * globalLight.intensity, 1.0);
    vec4 lights = vec4(directional_light() + spot_light() + point_light(), 1.0);
	FragColor = ambient * lights;
    //FragColor = vec4(oNormal * 0.5 + 0.5, 1.0);
}