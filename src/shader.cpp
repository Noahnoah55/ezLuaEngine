#include "shader.hpp"
#include<GLES3/gl3.h>
#include<GL/glew.h>
#include<fstream>
#include<string>
#include<iostream>

ezlua::shader_program::shader_program(const char* vertex_path, const char* fragment_path) {
    std::string vertex_code_str;
    std::string fragment_code_str;

    std::ifstream vertex_stream(vertex_path);
    char buff[512];
    while (!(vertex_stream.fail())) {
        vertex_stream.getline(buff, 512);
        vertex_code_str.append(buff);
        vertex_code_str.append("\n");
    }
    if (!vertex_stream.eof()) {
        std::cout << "May have failed to read vertex stream\n";
    }
    buff[0] = '\0';

    std::ifstream fragment_stream(fragment_path);
    while (!(fragment_stream.fail())) {
        fragment_stream.getline(buff, 512);
        fragment_code_str.append(buff);
        fragment_code_str.append("\n");
    }
    if (!fragment_stream.eof()) {
        std::cout << "May have failed to read fragment stream\n";
    }
    buff[0] = '\0';

    const char* vertex_code = vertex_code_str.c_str();
    const char* fragment_code = fragment_code_str.c_str();
    int compile_status;

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_code, NULL);
    glCompileShader(vertexShader); // TODO CHECK SHADER COMPILATION SUCCESS
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        glGetShaderInfoLog(vertexShader, 512, NULL, buff);
        printf("\nFAILED to compile vertex shader!\n---\n%s\n---\n%s---\n", vertex_code, buff);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_code, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, buff);
        printf("\nFAILED to compile fragment shader!\n---\n%s\n---\n%s---\n", fragment_code, buff);
    }

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
