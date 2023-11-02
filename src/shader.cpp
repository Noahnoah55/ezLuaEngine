#include "shader.hpp"
#include<GLES3/gl3.h>
#include<GL/glew.h>

const char *defaultVertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform highp mat4 proj;\n"
    "uniform highp mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *defaultFragmentShaderSource = "#version 300 es\n"
    "precision highp float;\n"
    "uniform vec4 color;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = color;\n"
    "}\0";

ezlua::shader_program::shader_program() {
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &defaultVertexShaderSource, NULL);
    glCompileShader(vertexShader); // TODO CHECK SHADER COMPILATION SUCCESS

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &defaultFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ezlua::shader_program::use()
{
    glUseProgram(id);
}
