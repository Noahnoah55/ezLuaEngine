#include "shader.hpp"
#include<GLES3/gl3.h>
#include<GL/glew.h>
#include<fstream>
#include<string>
#include<iostream>
#include<spdlog/spdlog.h>

ezlua::shader_program::shader_program(const char* vertex_path, const char* fragment_path) {
    _valid_flag = true;
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
        spdlog::critical("May have failed to read vertex stream");
    }
    buff[0] = '\0';

    std::ifstream fragment_stream(fragment_path);
    while (!(fragment_stream.fail())) {
        fragment_stream.getline(buff, 512);
        fragment_code_str.append(buff);
        fragment_code_str.append("\n");
    }
    if (!fragment_stream.eof()) {
        spdlog::critical("May have failed to read fragment stream");
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
        spdlog::error("Failed to compile vertex shader from {}, error: {}\n{}",
            vertex_path, buff, vertex_code);
        _valid_flag = false;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_code, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, buff);
        spdlog::error("Failed to compile fragment shader from {}, error: {}\n{}",
            fragment_path, buff, fragment_code);
        _valid_flag = false;
    }

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &compile_status);
    if (!compile_status) {
        glGetProgramInfoLog(id, 512, NULL, buff);
        spdlog::error("Failed to link shader program from sources {} and {}, error: {}",
            vertex_path, fragment_path, buff);
        _valid_flag = false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ezlua::shader_program::use() {
    if (valid()) {
        glUseProgram(id);
    }
    else {
        spdlog::error("Tried to use an invalid shader program");
    }
}

bool ezlua::shader_program::valid() {
    return _valid_flag;
}
