#version 400 core

in vec3 tex_coord;

uniform samplerCube tex1;

out vec4 fColor;

void main()
{
    fColor = texture(tex1, tex_coord);
}
