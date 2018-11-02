 emcc  -DUSE_LUA=ON src/3rdparty/slre/slre.c \
 src/include/stb_image.c src/include/stb_vorbis.c \
 src/3rdparty/noise/simplexnoise.c src/3rdparty/microtar/microtar.c \
 src/3rdparty/CMath/cmath.c src/3rdparty/lua/lapi.c src/3rdparty/lua/lauxlib.c \
 native/game.c src/3rdparty/lua/lbaselib.c src/3rdparty/lua/lcode.c \
 src/3rdparty/lua/ldblib.c src/3rdparty/lua/ldebug.c src/3rdparty/lua/ldo.c \
 src/3rdparty/lua/ldump.c \
 src/3rdparty/lua/lundump.c src/3rdparty/lua/lfunc.c src/3rdparty/lua/lgc.c \
 src/3rdparty/lua/linit.c src/3rdparty/lua/liolib.c src/3rdparty/lua/llex.c \
 src/3rdparty/lua/lmathlib.c src/3rdparty/lua/lmem.c src/3rdparty/lua/loadlib.c \
 src/3rdparty/lua/lobject.c src/3rdparty/lua/lopcodes.c \
 src/3rdparty/lua/loslib.c src/3rdparty/lua/lparser.c src/3rdparty/lua/lstate.c \
 src/3rdparty/lua/lstring.c src/3rdparty/lua/lstrlib.c \
 src/3rdparty/lua/ltable.c src/3rdparty/lua/ltablib.c \
 src/3rdparty/lua/ltm.c src/3rdparty/lua/lua.c \
 src/3rdparty/lua/luac.c src/3rdparty/lua/lvm.c src/3rdparty/lua/lzio.c \
 src/3rdparty/lua/print.c  src/main.cpp src/mouse.c src/system.c src/lua_mainactivity.c \
 src/love.c src/keyboard.c src/joystick.c src/audio/audio.c src/audio/source.c \
 src/audio/staticsource.c src/audio/streamsource.c src/audio/vorbis_decoder.c \
 src/audio/wav_decoder.c src/filesystem/filesystem.c src/graphics/batch.c \
 src/graphics/bitmap_font.c src/graphics/font.c src/graphics/geometry.c \
 src/graphics/gltools.c src/graphics/graphics.c src/graphics/image.c \
 src/graphics/matrixstack.c src/graphics/mesh.c src/graphics/quad.c \
 src/graphics/shader.c src/image/imagedata.c src/luaapi/audio.c \
 src/luaapi/boot.c src/luaapi/errorhandler.c src/luaapi/event.c \
 src/luaapi/filesystem.c src/luaapi/graphics_batch.c \
 src/luaapi/graphics_bitmapfont.c src/luaapi/graphics_font.c \
 src/luaapi/graphics_geometry.c src/luaapi/graphics_image.c \
 src/luaapi/graphics_mesh.c src/luaapi/graphics_quad.c \
 src/luaapi/graphics_shader.c src/luaapi/graphics_window.c \
 src/luaapi/graphics.c src/luaapi/image.c src/luaapi/joystick.c \
 src/luaapi/keyboard.c src/luaapi/love.c src/luaapi/math.c src/luaapi/mouse.c \
 src/luaapi/net.c src/luaapi/system.c src/luaapi/thread.c src/luaapi/timer.c \
 src/luaapi/tools.c src/math/minmax.c src/math/random.c \
 src/math/randomgenerator.c src/math/vector.c src/net/net.c src/timer/timer.c \
 src/tools/utf8.c src/tools/utils.c \
 -s ASSERTIONS=1 -s WASM=1 -lGL -lGLEW -s USE_FREETYPE=1 -s USE_SDL=2 -s FULL_ES2=1 \
-s ALLOW_MEMORY_GROWTH=1  \
 --memory-init-file 0 -DFT2_BUILD_LIBRARY -I src/3rdparty/lua/  \
 -I src/3rdparty/openal/include/ -I src/tools/ -I src/3rdparty/freetype/src/  \
 -I src/3rdparty/glew/include/GL -I src/3rdparty/CMath -I src/3rdparty/microtar \
 -I src/3rdparty/noise -I src/3rdparty/freetype/include/ -I src/include/ -O1 \
 --preload-file main.lua --preload-file hey.png \
 -o index.html
