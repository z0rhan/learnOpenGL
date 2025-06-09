#shader vertex
#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 fragmentPosition;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
   fragmentPosition = vec3(u_model * vec4(aPosition, 1.0));
   Normal = mat3(transpose(inverse(u_model))) * aNormal;

   gl_Position = u_projection * u_view * u_model * vec4(aPosition, 1.0);
}

#shader fragment
#version 460 core
in vec3 Normal;
in vec3 fragmentPosition;

out vec4 FragColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_viewPosition;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - fragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // Specular lighting
    vec3 viewDir = normalize(u_viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 resultingColor = ambient + diffuse + specular;

    FragColor = vec4(resultingColor, 1.0);
}
