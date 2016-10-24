#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <debugnet.h>
#include <psp2/kernel/processmgr.h>

#define ip_server "192.168.11.144"
#define port_server 18194

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_IMPLEMENTATION
#include "../../nuklear.h"
#include "../../nuklear-sdl-vita.h"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 500

int ret;

int main(int argc, char **argv)
{
    ret = debugNetInit(ip_server,port_server,DEBUG);
    debugNetPrintf(DEBUG, "Debug level %d\n", ret);
    debugNetPrintf(ERROR, "Error level %d\n", ret);
    debugNetPrintf(INFO, "Info level %d\n", ret);
    debugNetPrintf(DEBUG, "\n", ret); 
    
    debugNetPrintf(DEBUG, "Initializing variables\n", ret);
    static SDL_Surface *screen_surface;
    struct nk_color background;
    int running = 1;
    struct nk_context *ctx;
    float bg[4];

    /* SDL setup */
    debugNetPrintf(DEBUG, "SDL Init\n", ret);
    if( SDL_Init(SDL_INIT_VIDEO) == -1) {
        debugNetPrintf(ERROR, "Can't init SDL:  %s\n", SDL_GetError( ) );
        return 1;
    }
    debugNetPrintf(DEBUG, "SDL SetVideoMode\n", ret);
    screen_surface = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 24, SDL_FULLSCREEN);
    if(screen_surface == NULL) {
        debugNetPrintf(ERROR, "Can't set video mode: %s\n", SDL_GetError( ) );
        return 1;
    }

    debugNetPrintf(DEBUG, "Nuklear SDL init\n", ret);
    ctx = nk_sdl_init(screen_surface);
    background = nk_rgb(28,48,62);

    debugNetPrintf(INFO, "Entering event loop\n", ret);
    while (running)
    {
        debugNetPrintf(DEBUG, "Drawing background\n", ret);
        /* Draw */
        nk_color_fv(bg, background);
        nk_sdl_render(background);
    }

cleanup:
    debugNetPrintf(DEBUG, "Cleaning up\n", ret);
    debugNetFinish();
    nk_sdl_shutdown();
    SDL_Quit();
    return 0;
}

