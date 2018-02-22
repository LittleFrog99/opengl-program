#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct FlashLight {
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform FlashLight spotLight;
uniform Material material;
uniform mat4 view;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(vec3(view * vec4(-light.direction, 1.0)));

    float diff = max(dot(normal, lightDir), 0.0); // diffuse
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // specular

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 viewLightPos = vec3(view * vec4(light.position, 1.0));
    vec3 lightDir = normalize(viewLightPos - fragPos);

    float diff = max(dot(normal, lightDir), 0.0); // diffuse
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // specular

    float dist = length(viewLightPos - fragPos); // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 viewLightPos = vec3(0, 0, 0); // spotlight
    vec3 lightDir = normalize(viewLightPos - fragPos);
    float theta = dot(lightDir, normalize(-vec3(0, 0, -1)));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(viewLightPos - fragPos); // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoords));
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
    return (ambient + diffuse + specular) * intensity * attenuation;
}

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(-fragPos);
    vec3 result = calcDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
    result += calcFlashLight(spotLight, norm, fragPos, viewDir);
    fragColor = vec4(result, 1.0);
}
