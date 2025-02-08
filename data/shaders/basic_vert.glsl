#version 330 core

layout (location = 0) in vec3 apos;
layout (location = 1) in vec2 texcoord;

out vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    gl_Position = u_proj * u_view * u_model * vec4(apos, 1.0f);
    v_texcoord = texcoord;
}
