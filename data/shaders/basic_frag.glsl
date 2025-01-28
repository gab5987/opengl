#version 330 core

out vec4 fragcolor;
uniform vec3 un_color;

void main()
{ 
    fragcolor = vec4(un_color.x + 0.5f, un_color.y, un_color.z, 1.0f);
}
