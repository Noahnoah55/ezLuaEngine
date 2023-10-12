#include "assets.hpp"
#include "singletons.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

SDL_Texture* EZLUA_AssetStore::get_texture(std::string path)
{
    if (this->textures.find(path) != this->textures.end()) {
        return this->textures.at(path);
    }
    else {
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (surf == NULL) {
            return NULL;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        this->textures[path] = tex;
        return tex;
    }
};