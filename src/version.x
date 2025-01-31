#include <stdio.h>
#include <C:\SDL2\include\SDL.h>

int main(int argc, char *args[])
{
    SDL_version compiled;
    SDL_version linked;

    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    printf("Compiled SDL version: %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
    printf("Linked SDL version: %d.%d.%d\n", linked.major, linked.minor, linked.patch);

    return 0;
}
