#version 400 core

layout(location = 0) in vec4 vposition;

uniform mat4 ProjModelView;
uniform mat4 TCRotate;

out vec3 tex_coord;

void main()
{
    tex_coord = vec3(vposition);
    gl_Position = ProjModelView * TCRotate * vposition;
}

