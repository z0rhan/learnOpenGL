#shader vertex
#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture;

out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * vec4(position, 1.0, 1.0);
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
