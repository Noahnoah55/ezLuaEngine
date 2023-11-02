#include"gfx.hpp"
#include"shader.hpp"

#include<GLES3/gl3.h>
#include<GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<SDL2/SDL.h>

#include<stb_image.h>

#include<unordered_map>
#include<string>
#include<iostream>


enum PRIMATIVE {
    RECTANGLE_FLAT,
    RECTANGLE_TEX,
    LINE,
    _PRIMATIVE_END
};

struct render_ctx {
    ezlua::gfx::shader_program *program;
    GLuint VAO;
};

render_ctx CONTEXTS[static_cast<int>(_PRIMATIVE_END)];
std::unordered_map<std::string, GLuint> TEXTURES;
SDL_Window *WINDOW;
SDL_GLContext gl_context;

int ezlua::gfx::init_gfx() {
    // MAKE WINDOW AND CONTEXT
    WINDOW = SDL_CreateWindow("", 0, 0, 600, 300, SDL_WINDOW_OPENGL);
    if (WINDOW == NULL) {
        std::cout << "SDL failed to create a window, error: " << SDL_GetError() << "\n";
        return -1;
    }
    gl_context = SDL_GL_CreateContext(WINDOW);
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

    // INITIALIZE PRIMATIVES
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(1, &CONTEXTS[RECTANGLE_FLAT].VAO);
    glBindVertexArray(CONTEXTS[RECTANGLE_FLAT].VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    const float rect_verts[] = {
         0.5f,  0.5f, 0.0f, // TOP RIGHT
         0.5f, -0.5f, 0.0f, // BOTTOM RIGHT
        -0.5f, -0.5f, 0.0f, // BOTTOM LEFT
        -0.5f,  0.5f, 0.0f, // TOP LEFT
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
    CONTEXTS[RECTANGLE_FLAT].program = new shader_program("/shaders/flat.vert", "/shaders/flat.frag");
    return 0;
}


GLuint get_texture(std::string path)
{
    if (TEXTURES.find(path) != TEXTURES.end()) {
        return TEXTURES.at(path);
    }
    else {
        int w, h;
        unsigned char *data = stbi_load(path.c_str(), &w, &h, NULL, 4);
        if (data == NULL) {
            std::cout << "Failed to load texture, error: " << stbi_failure_reason() << "\n";
            return GL_FALSE; // 0 should be a special ID which is never assigned by opengl
        }
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        TEXTURES[path] = id;
        stbi_image_free(data);
        return id;
    }
}

void ezlua::gfx::draw_rect(transform pos, color col) {
    shader_program *shader = CONTEXTS[RECTANGLE_FLAT].program;
    shader->use();
    glBindVertexArray(CONTEXTS[RECTANGLE_FLAT].VAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos.x+pos.width/2, pos.y+pos.height/2, 0));
    model = glm::rotate(model, pos.rotation, glm::vec3(0.0f,0.0f,1.0f));
    model = glm::scale(model, glm::vec3(pos.width, pos.height, 1));
    auto proj = glm::ortho(0.0f, 600.0f, 300.0f, 0.0f);

    glm::vec4 outColor(col.r/255.0f, col.g/255.0f, col.b/255.0f, col.a/255.0f);

    unsigned int modelLoc = glGetUniformLocation(shader->id, "model");
    unsigned int projLoc = glGetUniformLocation(shader->id, "proj");
    unsigned int colorLoc = glGetUniformLocation(shader->id, "color");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniform4fv(colorLoc, 1, glm::value_ptr(outColor));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}