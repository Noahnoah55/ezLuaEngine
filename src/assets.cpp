#include "assets.hpp"
#include "singletons.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<iostream>

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
}

TTF_Font *EZLUA_AssetStore::get_font(std::string path, int ptsize)
{
    std::pair query(path, ptsize);
    if (this->fonts.find(query) != this->fonts.end()) {
        return this->fonts.at(query);
    }
    else {
        SDL_ClearError();
        TTF_Font* font = TTF_OpenFont(path.c_str(), ptsize);
        if (font == NULL) {
            auto t = TTF_GetError();
            std::cout << t << '\n';
            return nullptr;
        }
        this->fonts[query] = font;
        return font;
    }
};

Mix_Chunk *EZLUA_AssetStore::get_chunk(std::string path) {
    if (this->chunks.find(path) != this->chunks.end()) {
        return this->chunks.at(path);
    }
    else {
        Mix_Chunk *c = Mix_LoadWAV(path.c_str());
        if (c == nullptr) {
            std::cout << Mix_GetError() << "\n";
            return c;
        }
        this->chunks[path] = c;
        return c;
    }
}