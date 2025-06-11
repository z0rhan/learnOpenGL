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

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Torch
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutoff;
};

uniform vec3 u_viewPosition;
uniform Material material;
uniform Light light;
uniform Torch torch;

void main()
{
    // First for the point light
    vec3 lightDir = normalize(light.position - fragmentPosition);
    // Ambient lighting
    vec3 pointAmbient = light.ambient * vec3(texture(material.diffuse, textureCoord));

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 pointDiffuse = diff * light.diffuse * vec3(texture(material.diffuse, textureCoord));

    // Specular lighting
    vec3 viewDir = normalize(u_viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm); // lightDir is poiting from obj to light source
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 pointSpecular = spec * light.specular * vec3(texture(material.specular, textureCoord));

    // attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    pointAmbient *= attenuation;
    pointDiffuse *= attenuation;
    pointSpecular *= attenuation;
    vec3 pointResult = pointAmbient + pointDiffuse + pointSpecular;

    // Now for the light spotlight [torch like]
    vec3 torchDir = normalize(torch.position - fragmentPosition);
    // Diffuse lighting
    float torchdiff = max(dot(norm, torchDir), 0.0);
    vec3 torchdiffuse = torchdiff * light.diffuse * vec3(texture(material.diffuse, textureCoord));

    // Specular lighting
    vec3 torchreflectDir = reflect(-torchDir, norm); // lightDir is poiting from obj to light source
    float torchspec = pow(max(dot(viewDir, torchreflectDir), 0.0), material.shininess);
    vec3 torchSpecular = torchspec * light.specular * vec3(texture(material.specular, textureCoord));

    // attenuation
    float torchdistance = length(torch.position - fragmentPosition);
    float torchattenuation = 1.0 / (light.constant + light.linear * torchdistance + light.quadratic * (torchdistance * torchdistance));

    torchdiffuse *= torchattenuation;
    torchSpecular *= torchattenuation;

    // spotlight
    float theta = dot(torchDir, normalize(-torch.direction));
    float epsilon = torch.cutoff - torch.outerCutoff;
    float intensity = clamp((theta - torch.outerCutoff) / epsilon, 0.0, 1.0);
    torchdiffuse *= intensity;
    torchSpecular *= intensity;

    vec3 torchresult = torchdiffuse + torchSpecular;


    vec3 resultingColor = pointResult + torchresult;
    FragColor = vec4(resultingColor, 1.0);
}
