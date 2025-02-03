

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../include/GL/glew.h"
#include <C:\SDL2\include\SDL2\SDL.h>
#include "../Headers/camera.h"
#include "../Headers/input.h"
#include "../Headers/nuklear_container.h"
bool mouse_button_down;
bool keyStates[SDL_NUM_SCANCODES] = {false};
bool is_running = false;
///////////////////////////////////////////////////////////////////////////////
// Poll system events and handle keyboard input
///////////////////////////////////////////////////////////////////////////////
void process_input(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        nk_sdl_handle_event(&event);
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                keyStates[event.key.keysym.scancode] = true;
                break;
            case SDL_KEYUP:
                keyStates[event.key.keysym.scancode] = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_RIGHT)mouse_button_down = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_RIGHT)mouse_button_down = false;
                break;
            case SDL_MOUSEMOTION:
                    if(mouse_button_down){
                        process_mouse_move((float)event.motion.xrel,(float)event.motion.yrel,1);
                    }
                break;
            case SDL_MOUSEWHEEL:
                fov -= (float) event.wheel.y;
                if (fov < 1.0f)
                    fov = 1.0f;
                if (fov > 45.0f)
                    fov = 45.0f;
        }
    }
}