![Alt text](opt/CLoveLogo.png?raw=true "CLove")

CLove
=====
CLove is a simple, easy to learn and use 2D game framework tested on
Mac/Linux/PC, made in C with OpenGLES 2 which uses Lua or Aria as scripting language.

How to build
============
On Windows:
- Download and install mingw and let the setup configure the path for you.
 Open up the CMD and type gcc if you get an error then type this: setx PATH "%PATH%;C:\MinGW\bin;" . That command will add
 the bin folder to the path.
 After that check if you got gcc & g++ installed.
- Download and install CMake and let the setup configure the path for you.
- Make a new directory called build inside CLove and call:
 cmake ../ -DCMAKE_C_COMPILER:PATH=C:/MinGW/bin/gcc.exe -DCMAKE_CXX_COMPILER:PATH=C:/MinGW/bin/g++.exe . If this command
 does not work then go in C:\Program Files\CMake\bin and open up cmake-gui.exe. Tell it where is CLove and where you want to build the   project. Also do not forget to use custom compiler that is set to GCC as C compiler and G++ as the C++ compiler.
 After that use the make command and that's it( if you get errors that make does not exist then paste this into CMD terminal:
  copy c:\MinGW\bin\mingw32-make.exe c:\MinGW\bin\make.exe )
- Download DX SDK 2010 only if you get errors from SDL when building: http://www.microsoft.com/en-us/download/details.aspx?id=6812
- After you have succesfuly build CLove go into src/3rdparty directory and copy: SDL2, openal .dll in the same directory where clove.exe is.

On Linux and OS X:
- Download and install cmake,gcc,g++(optional git)(on OSX you can install these using brew)
- Make a new directory inside CLove, eg: build. Go there and call: cmake ../ &&
  make
- If you have problems building on Linux(Debian based) install:
sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev
libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libasound2-dev
libaudio-dev libesd0-dev libpulse-dev libroar-dev

Use:
cmake ../ -DUSE_PHYSFS=OFF to use native functions for filesystem.

Features
========
- Lua
- Aria
- Native C/C++ support.
- Easy to learn and use api.
- Cross platform.
- Custom package format.
- Powerful Batch system.
- Image loading and drawing.
- Image creation from scratch or from a template & save(png,bmp,tga).
- Meshes.
- Sound loading and playing (Vorbis and Wav).
- Streaming support for Vorbis files.
- Primitive drawing.
- Filesystem functions.
- OpenGL ES 2.0.
- Networking, unix only (TCP,IPv4 or IPv6)
- Powerful font loading and drawing using batch system.
- Support for image fonts.
- Keyboard, mouse and joystick support.
- Threads.
- Error handling.

Contributing
========
If you want to contribute to the project, feel free to work on what you please. I want to do a lot of the other stuff myself too.
Please try to replicate the code style of the existing code by using tab width of 4 spaces and expand tabs to spaces, I will refuse to pull your changes otherwise.

Planned work
============
1. Stable physics system
1. Networking for Windows
1. Web build system
1. Android
1. Bug fixes

Examples (see examples folder)
--------

Lua:
```lua
-- Example of drawing an image
local image = love.graphics.newImage("image.png")

function love.draw()
	love.graphics.draw(image, 200, 200)
end
```

Aria:
```cl
; Example of drawing an image
(do

(= image (love:graphics-newImage "image.png"))

(= love-draw
    (function ()

	(love:graphics-drawImage image 200 200)
    ))
)
```

Lua:
```lua
-- Example of drawing some primitives
function love.draw()
	love.graphics.rectangle("fill", 100, 100, 32, 16)
	love.graphics.rectangle("line", 200, 200, 32, 32)
	love.graphics.circle("fill", 270, 200, 32, 16)
	love.graphics.circle("line", 300, 100, 32, 8)
end
```

Aria:
```cl
; Example of drawing some primitives

(do

(= love-draw
    (function ()

	(love:graphics-rectangle "fill" 100 100 32 16)
	(love:graphics-rectangle "line" 200 200 32 16)
	(love:graphics-circle "fill" 300 200 32 16)
	(love:graphics-circle "line" 380 300 32 8)
    ))
)
```

Lua:
```lua
-- Example of playing music
local ogg_music = love.audio.newSource("music.ogg")
function love.load()
	ogg_music:play()
end
```

Aria:
```cl
(do

; Example of playing music
(= ogg_music (love:audio-newSource "music.ogg"))
(love:audio-play ogg_music)
)
```

CLove pictures
-----------------------------

![Image 1:](opt/data/4.png?raw=true "See examples folder")
![Image 2:](opt/data/1.png?raw=true "Web")
![Image 3:](opt/data/2.png?raw=true "Linux")
![Image 4:](opt/data/3.png?raw=true "Os X")

License
-------

The MIT License (MIT)

Copyright (c) 2015-2018 Muresan Vlad

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


