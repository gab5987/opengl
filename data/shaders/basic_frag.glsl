#version 330 core

out vec4 fragcolor;
in vec2 v_texcoord;

// uniform vec3 un_color;
uniform sampler2D u_texture;

void main()
{
    vec4 texColor = texture(u_texture, v_texcoord);
    fragcolor = texColor;
    //fragcolor = vec4(un_color.x, un_color.y, un_color.z, 1.0f);
}
