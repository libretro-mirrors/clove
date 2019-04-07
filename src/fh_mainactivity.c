/*
#   clove
#
#   Copyright (C) 2019 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "include/fh_mainactivity.h"

/*
 * TODO:
 * 1) make use of macro "USE_NATIVE"
 * 2) make use of clove_error from utils.h
 */

#include "fhapi/keyboard.h"
#include "fhapi/mouse.h"

typedef struct {
    bool running;
    bool called_quit;
    struct fh_program *prog;
    struct fh_value delta;
    struct fh_value focus;
} MainLoopData;

MainLoopData loopData;

static void quit_function(void) {
    if (fh_call_function(loopData.prog, "love_quit", NULL, 0, NULL) < 0) {
        clove_error("Errro: %s\n", fh_get_error(loopData.prog));
    }
}


static void focus_function(void) {
    loopData.focus.data.b = graphics_hasFocus();
    fh_call_function(loopData.prog, "love_focus", &loopData.focus, 1, NULL);
}


static void main_clean(void) {
  //joystick_close();
  graphics_destroyWindow();
  filesystem_free();
  fh_free_program(loopData.prog);
  audio_close();
}

void fh_main_loop(void) {
    timer_step();
    focus_function();
    matrixstack_origin();
    loopData.delta.data.num = (double)timer_getDelta();
    fh_call_function(loopData.prog, "love_update", &loopData.delta, 1, NULL);

    graphics_clear();

    fh_call_function(loopData.prog, "love_draw", NULL, 0, NULL);

    graphics_swap();

    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT) {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_ENTER:
                graphics_setMouseFocus(1);
                break;
            case SDL_WINDOWEVENT_LEAVE:
                graphics_setMouseFocus(0);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                graphics_setFocus(0);
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                graphics_setFocus(1);
                break;
            default:
                break;
            }
        }
        switch(event.wheel.type) {
        case SDL_MOUSEWHEEL:
            /*mouse_mousewheel(event.wheel.y);
            int _what = event.wheel.y == 1 ? SDL_MOUSEBUTTONUP : SDL_MOUSEBUTTONDOWN;
            mouse_mousepressed(event.button.x, event.button.y, _what);
            mouse_setButton(event.button.button);*/
            break;
        default:
            break;
        }
        switch(event.type) {
        case SDL_KEYDOWN:
            keyboard_keypressed(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            keyboard_keyreleased(event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT:
            keyboard_textInput(event.text.text);
            break;
        case SDL_MOUSEMOTION:
            /*mouse_mousemoved(event.motion.x, event.motion.y);*/
            break;
        case SDL_MOUSEBUTTONDOWN:
            /*
            mouse_mousepressed(event.button.x, event.button.y, event.button.button);
            mouse_setButton(event.button.button);*/
            break;
        case SDL_MOUSEBUTTONUP:
            /*mouse_mousereleased(event.button.x, event.button.y,
                                event.button.button);
            mouse_setButton(0);*/
            break;
        case SDL_JOYDEVICEADDED:
           // joystick_added(event.jdevice.which);
            break;
        case SDL_JOYDEVICEREMOVED:
           // joystick_remove(event.jdevice.which);
            break;
        case SDL_JOYAXISMOTION:
            break;
        case SDL_JOYBUTTONDOWN:
           // joystick_buttonDown(event.jbutton.which, event.jbutton.button, event.jbutton.state);
            break;
        case SDL_JOYBUTTONUP:
          //  joystick_buttonUp(event.jbutton.which, event.jbutton.button, event.jbutton.state);
            break;
#ifdef CLOVE_DESKTOP
        case SDL_QUIT:
            loopData.called_quit = true;
            quit_function();
            loopData.running = false;
            break;
#endif
        }
    }
    audio_updateStreams();
}

void fh_main_activity_load(int argc, char* argv[]) {
    loopData.running = true;
    loopData.called_quit = false;
    loopData.prog = fh_new_program();
    if (! loopData.prog) {
        printf("ERROR: out of memory for FH\n");
        return;
    }

    keyboard_init();
    //joystick_init();
    timer_init();

    //love_Config config;

    filesystem_init(argv[0], 1/*config.window.stats*/);

    graphics_particlesystem_init();

    audio_init(1/*config.window.stats*/);
    //filesystem_setIdentity("./");

    if (1/*config.window.stats*/)
        printf("%s %s \n", "Debug: Platform:", filesystem_getOS());

    graphics_init(800/*config.window.width*/, 600/*config.window.height*/, 1/*config.window.resizable*/, 1/*config.window.stats*/, 1/*config.window.window*/);
    /*
    * When we do not have a visible window we can't put
    * these propieties
    */
    if (1/*config.window.window*/)
    {
        graphics_setTitle("CLove"/*config.window.title*/);
        graphics_setBordless(1/*config.window.bordless*/);
        graphics_setMinSize(800, 600/*config.window.minwidth, config.window.minheight*/);
        graphics_setVsync(1/*config.window.vsync*/);
        graphics_setPosition(-1,-1/*config.window.x, config.window.y*/);
    }
    loopData.running = 1;

    FILE* icon = fopen("icon.png", "r");
    if (icon)
    {
        fclose(icon);
        image_ImageData* img = malloc(sizeof(image_ImageData));
        image_ImageData_new_with_filename(img, "icon.png");
        graphics_setIcon(img);
        free(img);
    }

    love_Version const * version = love_getVersion();
    if (1/*config.window.stats > 0*/)
        printf("%s %s %d.%d.%d \n", "CLove version - ",
               version->codename,version->major,version->minor,version->revision);

    fh_keyboard_register(loopData.prog);
    int ret = fh_run_script_file(loopData.prog, 0, "main.fh", argv, argc);
    if (ret < 0) {
        printf("ERROR: %s\n", fh_get_error(loopData.prog));
        return;
    }

    loopData.delta = fh_new_number(1);
    loopData.focus = fh_new_bool(true);

#ifdef CLOVE_WEB
    emscripten_set_main_loop(lua_main_loop, 60, 1);
#else
    while (loopData.running) {
        fh_main_loop();
    }
#endif
    if (!loopData.called_quit) {
        quit_function();
    }
    main_clean();
}
