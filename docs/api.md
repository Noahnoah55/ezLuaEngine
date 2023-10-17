# API Reference

This is the lua API reference for the ezLuaEngine

## _update()

This function will be called by the engine every frame. Do all processing and frame drawing here.

## Input

### getKey(key-name)

Tests to see if the key at the position indicatd by the key name is pressed. Keys are referenced by scancode so control layout will remain constant even with non-QUERTY keyboard layouts. Key names correlate with the table at <https://wiki.libsdl.org/SDL2/SDL_Scancode>.

## Graphics

### drawRect(x, y, width, height, {r, g, b})

Draw a rectangle starting from the top left at (x,y) with the given dimensions and color.

### drawImage(path, x, y, width_scale, height_scale)

Draw the image at the provided path starting from the top left at (x,y), stretched by a factor of
width_scale and height_scale.

An image `foo.png` placed in the assets folder can be found at `/game-files/assets/foo.png`
