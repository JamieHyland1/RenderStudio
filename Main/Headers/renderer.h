#ifndef renderer_h
#define renderer_h

int setup_renderer();
bool init_opengl();
void process_input();
void update();
void renderUI();
void render();
void free_resources();
void imgui_process_event(const SDL_Event* e);

#endif