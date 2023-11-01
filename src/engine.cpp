#include"engine.hpp"

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_opengl.h>

#include<GLES3/gl3.h>
#include<GL/glew.h>
unsigned int VAO;
unsigned int VBO;
unsigned int EBO;
unsigned int shaderProgram;

const char *vertexShaderSource = "#version 300 es\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 300 es\n"
    "precision highp float;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

float verts[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

int ezlua::engine::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL failed to initialize, error: " << SDL_GetError() << "\n";
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cout << "SDL_mixer failed to initialize, error: " << Mix_GetError() << "\n";
        return -1;
    }

    if (TTF_Init() != 0) {
        std::cout << "SDL_ttf failed to initialize, error: " << TTF_GetError() << "\n";
        return -1;
    }
    
    int img_flags = IMG_INIT_PNG;

    if (IMG_Init(img_flags) != img_flags) {
        std::cout << "SDL_Image failed to initialize, error: " << IMG_GetError() << "\n";
        return -1;
    }

    window = SDL_CreateWindow("", 0, 0, 600, 300, SDL_WINDOW_OPENGL);

    if (window == NULL) {
        std::cout << "SDL failed to create a window, error: " << SDL_GetError() << "\n";
        return -1;
    }

    gl_context = SDL_GL_CreateContext(window);

    if (gl_context == NULL) {
        std::cout << "SDL failed to initialize an OpenGL context, error: " << SDL_GetError() << "\n";
        return -1;
    }

    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        std::cout << "GLEW failed to initialize, error: " << glewGetErrorString(glewError) << "\n";
    }

    std::cout << glGetString(GL_VERSION) << "\n";

    std::cout << "Compiling shaders!\n";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader); // TODO CHECK SHADER COMPILATION SUCCESS

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    std::cout << "Shaders compiled!\n";

    int success = 0;

    success = success | _init_lua();

    return success;
}

int ezlua::engine::_init_lua() {
    lua_state.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::package);
    lua_state.script(
        R"(
            function __handler (message)
                print("[LUA-ERROR]" .. message)
            end
        )");
    auto pfr = lua_state.safe_script_file("/src/main.lua", sol::script_pass_on_error);
    if (!pfr.valid()) {
        sol::error err = pfr;
        std::cout << err.what() << "\n";
        return -1;
    }
    lua_ontick = sol::protected_function(lua_state["_update"], lua_state["__handler"]);

    lua_state.set_function("drawRect", &ezlua::engine::draw_rect, this);
    return 0;
}

void ezlua::engine::tick() {
    lua_ontick();
}

void ezlua::engine::draw_rect(int x, int y, int width, int height, sol::table color)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
