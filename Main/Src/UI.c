#include <SDL2/SDL.h>
#include "../../include/cglm/cglm.h"
#include "../../include/GL/glew.h"

#include "../Headers/globals.h"
#include "../Headers/stb_image.h"
#include "../Headers/camera.h"
#include "../Headers/renderer.h"

#include "../Headers/model.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "cimgui_impl.h"

#include "../Headers/globals.h"
#include "../Headers/UI.h"


// ImGui state
static bool g_imgui_initialized = false;
static ImGuiIO* io = NULL; 



///////////////////////////////////////////////////////////////////////////////
// Render function to draw user interface (ImGui)
///////////////////////////////////////////////////////////////////////////////

void init_gui(SDL_Window* window, SDL_GLContext context) {
    // ---------------- ImGui / cimgui init ----------------
    igCreateContext(NULL);                              // create ImGui context
    ImGui_ImplSDL2_InitForOpenGL(window, context);      // SDL2 backend
    ImGui_ImplOpenGL3_Init("#version 420");             // GL 4.2 core
    g_imgui_initialized = true;
    // -----------------------------------------------------

    io = igGetIO_Nil();

    // you can set io->ConfigFlags etc here if you want
}
//////////////////////////////////////////////////////////////////////////////
// (Optional) helper so you can call this from your event loop if you want
///////////////////////////////////////////////////////////////////////////////
void renderer_imgui_process_event(const SDL_Event* e) {
    if (g_imgui_initialized) {
        ImGui_ImplSDL2_ProcessEvent(e);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Render function to draw user interface (ImGui)
///////////////////////////////////////////////////////////////////////////////

void renderUI(void) {
    if (!g_imgui_initialized) return;

    // --- Begin ImGui frame ---
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    igNewFrame();

    // ------ Build your UI ------

    igBegin("RenderLab / cimgui test", NULL, 0);
    igText("Basic test window");
    igEnd();

    igSetNextWindowPos((ImVec2){ 0.0f, 0.0f }, ImGuiCond_Always, (ImVec2){ 0.0f, 0.0f });
    igSetNextWindowSize(
        (ImVec2){ window_width / 5.0f, window_height / 7.0f },
        ImGuiCond_Always
    );

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
                           | ImGuiWindowFlags_NoResize;

    igBegin("Hello from Render Studio!", NULL, flags);

    igText("Render Studio Stats");
    igSeparator();
    igText("FPS: %.1f", fps);
    igText("Delta time: %.3f ms", delta_time * 1000.0f);
    igText("Window: %d x %d", window_width, window_height);

    igColorEdit3("Clear color", clear_color, 0);
    igEnd();

    if (igBeginTabBar("Tabs", ImGuiTabBarFlags_None)) {
        if (igBeginTabItem("Info", NULL, ImGuiTabItemFlags_None)) {
            igText("This is a sample info tab.");
            igEndTabItem();
        }

        if (igBeginTabItem("Settings", NULL, ImGuiTabItemFlags_None)) {
            igText("This is a sample settings tab.");
            igEndTabItem();
        }

        igEndTabBar();
    }

    // ------ End / Render frame ------
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}


  void free_gui(){
  
    if (g_imgui_initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        igDestroyContext(NULL);
        g_imgui_initialized = false;
    }

  }


 void imgui_process_event(const SDL_Event* e) {
    if (!g_imgui_initialized) return;
    ImGui_ImplSDL2_ProcessEvent((SDL_Event*)e);
} 
