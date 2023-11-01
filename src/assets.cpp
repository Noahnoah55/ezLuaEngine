#include "assets.hpp"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<iostream>

SDL_Texture* ezlua::asset_store::get_texture(std::string path)
{
    if (this->textures.find(path) != this->textures.end()) {
        return this->textures.at(path);
    }
    else {
        SDL_Surface* surf = IMG_Load(path.c_str());
        if (surf == NULL) {
            std::cout << IMG_GetError() << "\n";
            return NULL;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(NULL, surf);
        this->textures[path] = tex;
        return tex;
    }
}

TTF_Font *ezlua::asset_store::get_font(std::string path, int ptsize)
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

Mix_Chunk *ezlua::asset_store::get_chunk(std::string path) {
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

Mix_Music *ezlua::asset_store::get_music(std::string path) {
    if (this->musics.find(path) != this->musics.end()) {
        return this->musics.at(path);
    }
    else {
        Mix_Music *m = Mix_LoadMUS(path.c_str());
        if (m == nullptr) {
            std::cout << Mix_GetError() << "\n";
            return m;
        }
        this->musics[path] = m;
        return m;
    }
}