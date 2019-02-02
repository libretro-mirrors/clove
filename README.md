![Alt text](opt/CLoveLogo.png?raw=true "CLove")

CLove
=====
CLove is a simple, easy to learn and use 2D game framework tested on
Mac/Linux/Windows/Web, made in C with OpenGLES 2 which uses Lua as scripting language.

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
- Go inside CLove/ and call ./build_linux.sh or ./build_osx.sh 
- If you have problems building on Linux(Debian based) install:
sudo apt-get install freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev
libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libasound2-dev
libaudio-dev libesd0-dev libpulse-dev libroar-dev


For Web: 
- Install emscripten(1.38.15 (commit 7a0e27441eda6cb0e3f1210e6837cae4b080ab4c)) and add it to your path.
- Run ./build_web.sh .When it's done you will have "clove.bc" and other files. The .bc file is used 
for linking with your C/C++ programs, the rest is used for Lua.
- For testing you have the following options: 
    1. Copy generated files into /var/www/html/ and run localhost (you must have installed apache2)
    1. Open a terminal and run: python3 -m http.server . Run index.html and go to localhost:8000


NOTE:
CLove might work with other version of emscripten but I haven't tested!
Inside the build scripts you can disable/enable Lua and Physfs.
When you want to release a project you must modify build_web.sh to preload your files.

Features
========
- Lua.
- Can be used as a shared or static library.
- Native C/C++ support.
- Easy to learn and use api.
- Cross platform (Linux, MacOS, Web and Windows).
- Custom package format.
- Powerful Batch system.
- Powerful Particle system.
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

Planned work
============
1. Fix/Improve Threads module and Networking overall(plus support for Windows)
1. Android

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

Lua:
```lua
-- Example of playing music
local ogg_music = love.audio.newSource("music.ogg")
function love.load()
	ogg_music:play()
end
```


Contact
-------

Freenode: murii
Email: muresanvladmihail@gmail.com


CLove pictures
-----------------------------

![Image 1:](opt/data/4.png?raw=true "See examples folder")
![Image 2:](opt/data/1.png?raw=true "Web")
![Image 3:](opt/data/2.png?raw=true "Linux")
![Image 4:](opt/data/3.png?raw=true "Os X")

License
-------

The MIT License (MIT)

Copyright (c) 2015-2019 Muresan Vlad

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
