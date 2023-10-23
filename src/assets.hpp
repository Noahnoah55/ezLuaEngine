#pragma once
#include<unordered_map>
#include<map>
#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>

class EZLUA_AssetStore {
    public:
        SDL_Texture* get_texture(std::string text);
        TTF_Font* get_font(std::string path, int ptsize);
        Mix_Chunk* get_chunk(std::string path);
        Mix_Music* get_music(std::string path);

    private:
        std::unordered_map<std::string, SDL_Texture*> textures;
        // TODO: write a hashing function so i can use unordered_map for fonts
        std::map<std::pair<std::string, int>, TTF_Font*> fonts;
        std::unordered_map<std::string, Mix_Chunk*> chunks;
        std::unordered_map<std::string, Mix_Music*> musics;
};