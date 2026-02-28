# Made tetris again award
![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=flat&logo=linux&logoColor=black)
![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)

A tetris game in C with sokol gp inspired by Doom 3 architecture

## How to build
**Requirements:**
1. Zig compiler

**Steps:**

1. After cloning, run `git submodule update --init --recursive` to include [sokol_gp](https://github.com/edubart/sokol_gp)

## Features
- Seperate game core and client implementation
- Doom 3 style input queue
- Custom bitmap font renderer (on top of stb)
- Custom ogg player (on top of stb)
- Menu stack
- Multi threading (render & audio)

### Goals
1. Doom 3 style server-client bots
2. Multiplayer via the network input channel