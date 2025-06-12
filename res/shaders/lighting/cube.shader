#shader vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCoords;

out vec3 Normal;
out vec3 fragmentPosition;
out vec2 textureCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
   Normal = mat3(transpose(inverse(u_model))) * aNormal;
   fragmentPosition = vec3(u_model * vec4(aPosition, 1.0));
   textureCoord = aTextureCoords;

   gl_Position = u_projection * u_view * u_model * vec4(aPosition, 1.0);
}

#shader fragment
#version 460 core
in vec3 Normal;
in vec3 fragmentPosition;
in vec2 textureCoord;

out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct Torch
{
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Torch torch;
vec3 calculateSpotlight(Torch light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform vec3 u_viewPosition;
uniform Material material;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(u_viewPosition - fragmentPosition);

    // Directional lighting
    vec3 result = calculateDirLight(dirLight, normal, viewDir);

    // Point light
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(pointLights[i], normal, fragmentPosition, viewDir);
    }

    // Spotlight
    result += calculateSpotlight(torch, normal, fragmentPosition, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calculateDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoord));

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(light.position - fragPos);

    float diff = max(dot(lightDir, normal), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotlight(Torch light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-light.direction, normal);
    float distance = length(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;

    float diff = max(dot(lightDir, normal), 0.0);
    float spec = pow(max(dot(viewDir, lightDir), 0.0), material.shininess);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, textureCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, textureCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, textureCoord));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
