# Cineris
Cineris is the name of the game engine I’m developing for Ashmoor Case. It’s a custom-built engine designed to give me full control over the rendering pipeline, game architecture, and overall development process. Cineris means “ashes” in Latin, for me, the name represents the idea of building something new from previous experiences

# Ashmoor Case

Ashmoor Case is my first game development project. A personal journey into the world of game creation.
I’ve always wanted to learn how games are made, and this project marks the beginning of that dream.

Inspired by one of my favorite films, Shutter Island, Ashmoor Case will explore mystery, psychological tension, and atmosphere.


# About the Project

This project is both a learning experience and the foundation for a full game.
I’m building everything from scratch using C++, OpenGL, and GLFW, to understand how rendering, shaders, and game architecture truly work.

The goal of The Ashmoor Case is to create a dark, grounded world that combines a low-poly visual style with a sense of realism. The focus is on mood, lighting, and subtle environmental storytelling.

## References
If the description above doesn’t fully capture the mood, here are some visual references I generated to explore the atmosphere in more detail.

> Visual inspiration only — these are not in-game screenshots or final previews.

| Ambient 1 | Ambient 2 | Main Character |
|---|---|---|
| ![ASHMOOR REFERENCE 1](resources/references/image.png) | ![ASHMOOR REFERENCE 2](resources/references/image2.png) | ![ASHMOOR REFERENCE 3](resources/references/image3.png) |

> Generated with ChatGPT and used only as visual inspiration.

I plan to create some of the textures, models and also music for this game.

## Setup
If you want to build Cineris locally, make sure you have the following installed:
- Git
- CMake 4.0 or newer
- C++20-compatible compiler

Clone the repository together with its submodules:
```
git clone --recursive https://github.com/GitScorz/cineris.git
cd cineris
```

If you already cloned the repository without submodules, run:
```
git submodule update --init --recursive
```
Cineris uses vcpkg to install and manage dependencies.

### macOS
Bootstrap vcpkg:
```
./vendor/vcpkg/bootstrap-vcpkg.sh -disableMetrics
```

Configure and build the Debug version:

```
cmake --preset debug
cmake --build --preset debug
```

For a Release build trade the word "debug" with "release".

### Windows
Bootstrap vcpkg:

```
.\vendor\vcpkg\bootstrap-vcpkg.bat -disableMetrics
```

Configure and build the Debug version:

```
cmake --preset debug
cmake --build --preset debug
```

For a Release build trade the word "debug" with "release".

The required dependencies are installed automatically during the first CMake configuration. OpenGL is provided by the operating system and does not need to be installed through vcpkg.

The compiled executable will be placed inside the corresponding build directory:
```
build/debug/
build/release/
```

## License

This project is available for personal and educational use under the Cineris Non-Commercial License.

You may study, modify, and share the project for non-commercial purposes.

You may not sell this project, sell modified versions, include it in paid products, or otherwise use it commercially without explicit permission from the author.

<br>

---

#### June 15 2026

I’m not completely happy with how this scene looks yet. The basic layout, textures, water, and lighting are in place, but the screenshot still feels darker and less atmospheric than what I have in mind. 
It’s a start, but I’ll need to keep working on the lighting, composition, and overall mood until it feels closer to the vision for Ashmoor Case.
![First screenshot lol](journey/june_15_2026.png)
