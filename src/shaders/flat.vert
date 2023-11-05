#version 300 es
layout (location = 0) in vec3 aPos;
uniform highp mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}