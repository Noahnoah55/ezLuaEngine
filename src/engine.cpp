#include"engine.hpp"
#include"shader.hpp"

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_opengl.h>

#include<GLES3/gl3.h>
#include<GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

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

    shader = new shader_program();

    generate_shapes();

    std::cout << "Shaders compiled!\n";

    int success = 0;

    success = success | _init_lua();

    return success;
}

void ezlua::engine::generate_shapes(){
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(static_cast<int>(_SHAPE_TOTAL), vaos);
    glBindVertexArray(vaos[RECTANGLE]);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    const float rect_verts[] = {
        1.0f, 1.0f, 0.0f, // TOP RIGHT
        1.0f, 0.0f, 0.0f, // BOTTOM RIGHT
        0.0f, 0.0f, 0.0f, // BOTTOM LEFT
        0.0f, 1.0f, 0.0f, // TOP LEFT
    };
    const unsigned int rect_indices[] = {
        0, 1, 3, // TRI ONE
        1, 2, 3, // TRI TWO
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_verts), rect_verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
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

void ezlua::engine::draw_rect(float x, float y, float width, float height, sol::table color) {
    shader->use();
    glBindVertexArray(vaos[RECTANGLE]);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(x, y, 0));
    model = glm::scale(model, glm::vec3(width, height, 1));
    auto proj = glm::ortho(0.0f, 600.0f, 300.0f, 0.0f);

    glm::vec4 outColor((float)color[1]/255.0f, (float)color[2]/255.0f, (float)color[3]/255.0f, 1);

    unsigned int modelLoc = glGetUniformLocation(shader->id, "model");
    unsigned int projLoc = glGetUniformLocation(shader->id, "proj");
    unsigned int colorLoc = glGetUniformLocation(shader->id, "color");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniform4fv(colorLoc, 1, glm::value_ptr(outColor));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
