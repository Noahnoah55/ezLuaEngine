#version 300 es
layout (location = 0) in vec3 aPos;
uniform highp mat4 proj;
uniform highp mat4 model;
void main()
{
    gl_Position = proj * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}