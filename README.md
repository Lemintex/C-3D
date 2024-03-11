# C-3D
## What is C-3D
C-3D is a 3D rendering engine using few libraries, meaning no OpenGL or Vulcan for easy rendering. I did this as a challenge and to allow me to gain a better undersanding of the rendering pipeline.

## Features
C-3D has the following features:
- Movable camera
- Entire 2D -> 3D rendering pipeline
- Triangle clipping, triangle culling, and other triangle drawing optimisations
- Triangle fill algorithm
- Scanline algorithm facilitating a solid fill
- Scanline algorithm adapted for texturing triangles
- .obj reading from file
- Reading algorithm without texture data
- Reading algorithm with texture data
- U,V mapping systen ensuring textures are properly mapped to meshes

## Install
C-3D can be run by doing the follwing:
- Install SDL2, SDL2_image, and math by running ```sudo apt-get install SDL2 SDL2_image math```
- Move obj/png file and texture into appropriate folders and update the filenames in main.c
- cd into the root directory of the cloned repository and run ```gcc src/*.c -o build/main -lSDL2 -lm -lSDL2_image```
- run ```./build/main```

### 1.1
- cd into the root directory and run the command ```chmod +x install_dependencies.sh``` and ```chmod +x compile.sh```
- run ```./install_dependencies.sh``` to install the required libraries
- put the .obj file and texture .png file in the res/ folder
- run ```./compile.sh``` to compile and run C-3D

## libraries
As well as some standard libraries, C-3D uses the following 3rd party libraries:
- SDL2
- SDL_image

## Results
![image](https://github.com/Lemintex/C-3D/assets/57079368/d269200f-8ed7-4620-89e7-867e55976c12)
![image](https://github.com/Lemintex/C-3D/assets/57079368/e7ef3a40-a334-42a7-a858-e6458ddcd36b)

![image](https://github.com/Lemintex/C-3D/assets/57079368/a10a4c75-d7e1-4ca4-bc68-efcdc6fe7577)
![image](https://github.com/Lemintex/C-3D/assets/57079368/fa993878-6958-434c-b2a4-355bfd89748e)

## Version Log
### 1.0
- Initial implementation of rendering engine

### 1.1
- Improved camera directional movement
- Added modifiable speed to camera movement ```press up/down arrow keys!```
- Added render mode control to options ```press left tab!```
- Improved camera look by adding mouse compatability
- Improved camera look by adding pitch
- Added dynamic .obj/.png file reading (no more hard coded filenames!)
- Added helper scripts for debian users

## Future Development
Depending on future demand, development on C-3D may continue with the following improvements in mind
- Bug fixes
- ~~Improved camera movement~~
- Dynamic movement/rotation/scaling of meshes
- ~~Drawing settings as settings~~
- Animations
