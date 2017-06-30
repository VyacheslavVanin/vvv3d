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
- simple GUI
    - text widget
    - image widget
    - button
    - editbox
    - panel
    - layouts
        - vertical
        - horizontal

## Build
### Dependencies
- freetype
- libglew
- boost-locale (for utf8 -> utf32 conversion)
- boost-gil    (for image loading)
- sdl2         (for windows, input)

### Build
```
mkdir your-build-dir
cd your-build-dir
cmake {path-to-CMakeLists.txt}
make
```

## Example

See [wiki](https://github.com/VyacheslavVanin/vvv3d/wiki)
