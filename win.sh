#!/bin/bash

i686-w64-mingw32-g++ -Iinclude -Llib -static-libgcc -static-libstdc++ -o Stereolizer src/main.cpp SDL2.dll -lmingw32 -lSDL2main -lSDL2 -mwindows