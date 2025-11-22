// Headers/ui.h
#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <SDL2/SDL.h>


void init_gui(SDL_Window* window, SDL_GLContext context);
void renderer_imgui_process_event(const SDL_Event* e);
void imgui_process_event(const SDL_Event* e);
void renderUI(void);
void free_gui(void);

#endif
