# vvv3d
vvv3d is c++ basic framework for creating 3d applications

## Features
- texture management
- geometry management
- shader programs management
- font management (ttf-fonts)
- simple text render (from utf strings)
- camera control (othographic, perspective projections)
- vector and matrices math
- frame buffer objects to render to textures
- texture atlasses
- input handling (keyboard, mouse)

## Build
### Dependencies
- freeglut3
- libglew
- boost-locale (for utf8 -> utf32 conversion)
- boost-gil    (for image loading)
- sdl2         (for windows, input)

### Build
```
qmake vvv3dnew.pro
make
```

## Example

// TODO
