#include<sound.hpp>

#include<spdlog/spdlog.h>
#include<SDL2/SDL_mixer.h>
#include<map>
#include<string>

class ezlua::soundimpl {
    public:
        std::map<std::string, Mix_Chunk *> chunks;
        std::map<std::string, Mix_Music *> musics;

        void print_a_thing() {
            spdlog::info("Test!");
        }

        void play_sound(std::string path) {
            if (auto chunk = get_chunk(path); chunk != NULL) {
                Mix_PlayChannel(-1, chunk, 0);
            }
        }

        void play_music(std::string path, int loops) {
            if (auto mus = get_music(path); mus != NULL) {
                Mix_PlayMusic(mus, 0);
            }
        }

        Mix_Chunk *get_chunk(std::string path)
        {
            if (auto existing_chunk = chunks.find(path); existing_chunk != chunks.end()) {
                return existing_chunk->second;
            }
            else {
                Mix_Chunk *new_chunk = Mix_LoadWAV(path.c_str());
                if (new_chunk == NULL) {
                    spdlog::error("Failed to load sound ({}), error {}", path, Mix_GetError());
                    return NULL;
                }
                chunks.emplace(path, new_chunk);
                return new_chunk;
            }
        }

        Mix_Music *get_music(std::string path)
        {
            if (auto existing_music = musics.find(path); existing_music != musics.end()) {
                return existing_music->second;
            }
            else {
                Mix_Music *new_music = Mix_LoadMUS(path.c_str());
                if (new_music == NULL) {
                    spdlog::error("Failed to load music ({}), error {}", path, Mix_GetError());
                    return NULL;
                }
                musics.emplace(path, new_music);
                return new_music;
            }
        }
};

const char* snd_module_name = "Sound";

ezlua::sound::sound() {
    this->internal = new ezlua::soundimpl();
}
ezlua::sound::~sound() {
    delete this->internal;
}

int ezlua::sound::initialize(sol::state *lua) {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    lua->set_function("playSound", &soundimpl::play_sound, this->internal);
    lua->set_function("playMusic", &soundimpl::play_music, this->internal);
    lua->set_function("pauseMusic", Mix_PauseMusic);
    lua->set_function("resumeMusic", Mix_ResumeMusic);
    return 0;
}

int ezlua::sound::on_tick(sol::state *lua) {
    return 0;
}

const char* ezlua::sound::get_name(int *len) {
    if (len != NULL) {
        *len = strlen(snd_module_name);
    }
    return snd_module_name;
}