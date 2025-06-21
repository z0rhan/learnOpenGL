#shader vertex
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoords;

out vec2 textureCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
   gl_Position = u_projection * u_view * u_model * vec4(a_Position, 1.0);
   textureCoord = a_TextureCoords;
}

#shader fragment
#version 460 core

in vec2 textureCoord;

out vec4 FragColor;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

void main()
{
    vec3 ambient = vec3(texture(material.texture_diffuse1, textureCoord));
    vec3 specular = vec3(texture(material.texture_specular1, textureCoord));

    vec3 result = ambient + specular;
    FragColor = vec4(result, 1.0);
}
