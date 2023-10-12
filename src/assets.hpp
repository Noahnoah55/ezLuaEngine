#pragma once
#include<unordered_map>
#include<string>
#include<SDL2/SDL.h>

class EZLUA_AssetStore {
    public:
        SDL_Texture* get_texture(std::string);

    private:
        std::unordered_map<std::string, SDL_Texture*> textures;
};