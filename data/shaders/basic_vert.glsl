#version 330 core
layout (location = 0) in vec3 apos;
layout(location = 1) in vec2 texcoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(apos, 1.0f);
    // gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    v_TexCoord = texcoord;
}
