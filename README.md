# ezLuaEngine

WIP web game engine using emscripten and LUA

<https://noahnoah55.github.io/ezLuaEngine/>

## Demos

- Pong <https://noahnoah55.github.io/ezLuaEngine/demos/pong/>

## Building

Currently only building for Linux, Windows builds coming next time I work on this at my PC

After cloning the repository, populate the submodule directories with
```git submodule update --init```

Then load the included emsdk to your path with
```source configure```

Finally, build the engine and host the demo game with
```make run```

The engine starts by loading ```src/main.lua```, and will call ```_update()``` every frame

API is unfinished, see examples for help
