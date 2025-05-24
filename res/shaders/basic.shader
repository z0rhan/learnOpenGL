#shader vertex
#version 460 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texture;

out vec2 v_texCoord;

void main()
{
   gl_Position = vec4(position, 1.0, 1.0);
   v_texCoord = texture;
}

#shader fragment
#version 460 core

out vec4 FragColor;
in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
    vec4 texColor = texture(u_texture, v_texCoord);
    FragColor = texColor;
}
