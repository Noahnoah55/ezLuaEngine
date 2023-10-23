#include<SDL2/SDL.h>
#include"assets.hpp"

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern EZLUA_AssetStore ASSET_STORE;
extern Uint8 lastKeystate[SDL_NUM_SCANCODES];