#shader vertex
#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
   gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
}

#shader fragment
#version 460 core

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0);
}
