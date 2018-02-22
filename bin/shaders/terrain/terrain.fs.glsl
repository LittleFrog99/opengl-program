#version 410 core
struct Material {
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

#define NR_POINT_LIGHTS 4
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform mat4 view;
uniform bool useBlinn;
uniform Material material;

in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

float diffuse(vec3 normal, vec3 lightDir) {
    return max(dot(normal, lightDir), 0.0);
}

float specular(vec3 viewDir, vec3 lightDir, vec3 normal, float shininess) {
    vec3 reflectDir = reflect(-lightDir, normal);
    if (useBlinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        return pow(max(dot(normal, halfwayDir), 0.0), shininess);
    }
    else 
        return pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(vec3(view * vec4(-light.direction, 0.0)));

    float diff = diffuse(normal, lightDir); // diffuse
    float spec = specular(viewDir, lightDir, normal, material.shininess); // specular

    vec3 ambient = light.ambient * vec3(1.0);
    vec3 diffuse = light.diffuse * diff * vec3(1.0);
    vec3 specular = light.specular * spec * vec3(1.0);
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 viewLightPos = vec3(view * vec4(light.position, 1.0));
    vec3 lightDir = normalize(viewLightPos - fragPos);

    float diff = diffuse(normal, lightDir); // diffuse
    float spec = specular(viewDir, lightDir, normal, material.shininess); // specular

    float dist = length(viewLightPos - fragPos); // attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    vec3 ambient = light.ambient * vec3(1.0);
    vec3 diffuse = light.diffuse * diff * vec3(1.0);
    vec3 specular = light.specular * spec * vec3(1.0);
    return (ambient + diffuse + specular) * attenuation;
}

vec3 previewModel(vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(vec3(0, 0, 1));

    float diff = max(dot(normal, lightDir), 0.0); // diffuse
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f); // specular

    vec3 ambient = 0.1f * vec3(1.0f, 1.0f, 1.0f);
    vec3 diffuse = 0.9f * diff * vec3(1.0f, 1.0f, 1.0f);
    vec3 specular = 0.8f * spec * vec3(1.0f, 1.0f, 1.0f);
    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    vec3 result = calcPointLight(pointLight, norm, FragPos, viewDir);
    result += calcDirLight(dirLight, norm, viewDir);
    // result += calcFlashLight(spotLight, norm, FragPos, viewDir);
    // vec3 result = previewModel(norm, FragPos, viewDir);
    fragColor = vec4(result, 1.0);
}
