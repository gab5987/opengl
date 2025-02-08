#version 330 core

out vec4 fragcolor;

// in vec2 v_TexCoord;
uniform vec3 un_color;

uniform sampler2D u_Texture;

void main()
{
    // vec4 texColor = texture(u_Texture, v_TexCoord);
    // fragcolor = texColor;
    fragcolor = vec4(un_color.x, un_color.y, un_color.z, 1.0f);
    // fragcolor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
