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

uniform vec3 u_objColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPosition;
uniform vec3 u_viewPosition;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = u_lightColor * ambientStrength;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_lightPosition - fragmentPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_viewPosition - fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 resultingColor = (ambient + diffuse + specular) * u_objColor;

    FragColor = vec4(resultingColor, 1.0);
}
