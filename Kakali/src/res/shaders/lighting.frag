#version 460 core

in vec3 oPos;
in vec2 oTexCoord;
in vec3 oNormal;

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
    sampler2D shadowMap;
};

uniform vec3 cameraPosition;
uniform GlobalLight globalLight;
uniform DirectionalLight directionalLight;

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
    return (ambient + diffuse + specular) * directionalLight.intensity;
}


void main() {
	vec4 ambient = vec4(globalLight.ambient * globalLight.intensity, 1.0);
    vec4 lights = vec4(directional_light(), 1.0);
	FragColor = ambient * lights;
}