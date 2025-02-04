#version 330 core

in vec2 v_TexCoord;
out vec4 fragcolor;
// uniform vec3 un_color;

uniform sampler2D u_Texture;

void main()
{ 
    vec4 texColor = texture(u_Texture, v_TexCoord);
    fragcolor = texColor;
    // fragcolor = vec4(un_color.x + 0.5f, un_color.y, un_color.z, 1.0f);
    // fragcolor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
