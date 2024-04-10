#include "vid.h"
#include "dsq.h"

int main() 
{
    SDL_Event ev;
    SDLVideoDriver vd;
    Heightmap_DSQ hmp(0, 46);

    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) == -1) 
        return -1;

    vd.InitGraphics(800, 600);

    hmp.Seed_Corners();
    hmp.Make_Heightmap(1.333);

    vd.HeightmapToRenderer();

    while(1) 
    {
        if(SDL_PollEvent(&ev)) 
        {
            if((ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE) || 
               (ev.key.keysym.scancode == SDL_SCANCODE_Q) ||
               (ev.type == SDL_QUIT)) {
                break;
            }
        }

        vd.CyclePalette();
        vd.RenderFrame();
        SDL_Delay(30);
    }

    vd.KillGraphics();

    return 0;
}