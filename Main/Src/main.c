#include "../Headers/camera.h"
#include "../Headers/renderer.h"
#include "../Headers/input.h"

///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* args[]) {
    setup_renderer();
    is_running = init_opengl();
    while (is_running) {
        process_input();
        update();
        render();
    }
    free_resources();
    return 0;
}