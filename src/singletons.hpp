#include<SDL2/SDL.h>
#include<set>
#include<mutex>
#include<emscripten/fetch.h>
#include"assets.hpp"

enum PHASE {
    DOWNLOAD_NOT_STARTED,
    DOWNLOAD_STARTED,
    DOWNLOAD_FINISHED,
    GAME_RUNNING
};

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern EZLUA_AssetStore ASSET_STORE;
extern Uint8 lastKeystate[SDL_NUM_SCANCODES];
extern PHASE GAME_PHASE;

extern int download_done;
extern std::set<emscripten_fetch_t*> fset;
extern std::mutex fset_mut;