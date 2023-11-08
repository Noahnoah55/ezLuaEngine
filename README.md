# ezLuaEngine

WIP web game engine using emscripten and LUA

<https://noahnoah55.github.io/ezLuaEngine/>

## Demos

- Pong <https://noahnoah55.github.io/ezLuaEngine/demos/pong/>

## Building

This project uses submodules, clone with ```$ git clone --recursive-submodules {URL}```
to automatically include submodules. You can also populate these submodules after cloning
with ```$ git submodule update -init```

Before building, activate emsdk with ```$ source configure```

Finally, to build the project and run the test game:

```bash
mkdir build
cd build
emcmake cmake ..
make
emrun engine.html
```

The engine starts by loading ```src/main.lua```, and will call ```_update()``` every frame

API is unfinished, see examples for help
