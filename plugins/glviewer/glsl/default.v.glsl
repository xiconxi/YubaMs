#version 410
layout (location = 0) in vec3 vert;
layout (location = 1) in vec3 norm;

uniform mat4 model;
uniform mat4 camera_vp;
out vec3 vnorm;

void main()
{
    vnorm = normalize(mat3(model)*norm);
    gl_Position = camera_vp* model * vec4(vert, 1.0f);
}
