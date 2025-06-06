#shader vertex
#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;

out vec2 v_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
   gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
   v_texCoord = texture;
}

#shader fragment
#version 460 core

out vec4 FragColor;
in vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main()
{
    FragColor = mix(texture(u_texture1, v_texCoord), texture(u_texture2, v_texCoord), 0.2);
}
