#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<string>
#include<iostream>
#include"api.hpp"
#include"singletons.hpp"

// HELPERS

void set_color_from_table(sol::table color) {
    SDL_SetRenderDrawColor(renderer, color[1], color[2], color[3], 255);
}

// API CALLS

// - INPUT

bool get_key(std::string keyname) {
    auto scan = SDL_GetScancodeFromName(keyname.c_str());
    return SDL_GetKeyboardState(nullptr)[scan];
}

bool get_key_just_pressed(std::string keyname) {
    auto scan = SDL_GetScancodeFromName(keyname.c_str());
    auto last = lastKeystate[scan];
    auto current = SDL_GetKeyboardState(nullptr)[scan];
    return current && !last;
}

// - GFX
void draw_rect(float x, float y, float w, float h, sol::table color) {
    set_color_from_table(color);
    SDL_FRect r = {.x=x, .y=y, .w=w, .h=h};
    SDL_RenderFillRectF(renderer, &r);
}

void draw_image(std::string path, float x, float y, float wscale, float hscale) {
    auto tex = ASSET_STORE.get_texture(path);
    if (tex == NULL) {
        std::cout << "[ERROR] Could not find texture at path " << path << "\n";
    }
    int w; int h;
    auto res = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_FRect rect; rect.x=x; rect.y=y; rect.w=wscale*w; rect.h=hscale*h;
    SDL_RenderCopyF(renderer, tex, NULL, &rect);
}

void draw_line(float x1, float y1, float x2, float y2, sol::table color) {
    set_color_from_table(color);
    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
}

void draw_text(std::string text, float x, float y, int ptsize, sol::table color, std::string fpath) {
    auto font = ASSET_STORE.get_font(fpath, ptsize);
    SDL_Color sdlc; sdlc.r=color[1]; sdlc.g=color[2]; sdlc.b=color[3];
    if (font == nullptr) {
        std::cerr << "[Error] Could not find font at path " << fpath << "\n";
        return;
    }
    auto surf = TTF_RenderText_Solid(font, text.c_str(), sdlc);
    auto tex = SDL_CreateTextureFromSurface(renderer, surf);
    int w; int h;
    auto res = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_FRect drect; drect.x=x; drect.y=y; drect.w=w; drect.h=h;
    SDL_RenderCopyF(renderer, tex, NULL, &drect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

// SOUND
void play_sound_vol(std::string path, int vol) {
    auto chunk = ASSET_STORE.get_chunk(path);
    Mix_VolumeChunk(chunk, vol);
    Mix_PlayChannel(-1, chunk, 0);
}

void play_sound(std::string path) {
    play_sound_vol(path, MIX_MAX_VOLUME);
}

void play_music(std::string path) {
    Mix_PlayMusic(ASSET_STORE.get_music(path), 0);
}

bool music_playing() {
    return Mix_PlayingMusic();
}

bool music_paused() {
    return Mix_PausedMusic();
}

int init_api(sol::state &lua) {
    lua.set_function("getKey", get_key);
    lua.set_function("getKeyJustPressed", get_key_just_pressed);

    lua.set_function("drawRect", draw_rect);
    lua.set_function("drawImage", draw_image);
    lua.set_function("drawLine", draw_line);
    lua.set_function("drawText", draw_text);

    lua.set_function("playSound", sol::overload(play_sound, play_sound_vol));
    lua.set_function("playMusic", play_music);
    lua.set_function("musicPlaying", music_playing);
    lua.set_function("musicPaused", music_paused);
    lua.set_function("pauseMusic", Mix_PauseMusic);
    lua.set_function("resumeMusic", Mix_ResumeMusic);
    lua.set_function("setMusicVol", Mix_VolumeMusic);

    return 0;
}