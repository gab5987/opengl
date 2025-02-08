#version 330 core

layout (location = 0) in vec3 apos;
// layout(location = 1) in vec2 texcoord;

// out vec2 v_TexCoord;

uniform mat4 u_mvp;

void main()
{
    // gl_Position = u_mvp * vec4(apos, 1.0f);
    // gl_Position = vec4(apos.x, apos.y, apos.z, 1.0);
    // v_TexCoord = gl_Position;
    // v_TexCoord = texcoord;
}
