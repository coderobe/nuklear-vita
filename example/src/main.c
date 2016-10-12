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
        debugNetPrintf(DEBUG, "Reading input\n", ret);
        /* Input */
        SDL_Event evt;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&evt)) {
            if (evt.type == SDL_QUIT) goto cleanup;
            nk_sdl_handle_event(&evt);
        }
        nk_input_end(ctx);

        debugNetPrintf(DEBUG, "Entering GUI scope\n", ret);
        /* GUI */
        {struct nk_panel layout;
        if (nk_begin(ctx, &layout, "Demo", nk_rect(50, 50, 210, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;
            
            debugNetPrintf(DEBUG, "Spawning items\n", ret);
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "button"))
                fprintf(stdout, "button pressed\n");
            nk_layout_row_dynamic(ctx, 30, 2);
            if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx, 25, 1);
            nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

            debugNetPrintf(DEBUG, "Spawning combo panel\n", ret);
            {struct nk_panel combo;
            nk_layout_row_dynamic(ctx, 20, 1);
            nk_label(ctx, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx, 25, 1);
            if (nk_combo_begin_color(ctx, &combo, background, nk_vec2(nk_widget_width(ctx),400))) {
                nk_layout_row_dynamic(ctx, 120, 1);
                background = nk_color_picker(ctx, background, NK_RGBA);
                nk_layout_row_dynamic(ctx, 25, 1);
                background.r = (nk_byte)nk_propertyi(ctx, "#R:", 0, background.r, 255, 1,1);
                background.g = (nk_byte)nk_propertyi(ctx, "#G:", 0, background.g, 255, 1,1);
                background.b = (nk_byte)nk_propertyi(ctx, "#B:", 0, background.b, 255, 1,1);
                background.a = (nk_byte)nk_propertyi(ctx, "#A:", 0, background.a, 255, 1,1);
                nk_combo_end(ctx);
            }}
        }
        debugNetPrintf(DEBUG, "Nuklear end\n", ret);
        nk_end(ctx);}

        debugNetPrintf(DEBUG, "Drawing image\n", ret);
        /* Draw */
        nk_color_fv(bg, background);
        nk_sdl_render(nk_rgb(30,30,30));
    }

cleanup:
    debugNetPrintf(DEBUG, "Cleaning up\n", ret);
    debugNetFinish();
    nk_sdl_shutdown();
    SDL_Quit();
    return 0;
}

