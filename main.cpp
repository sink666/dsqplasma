#include "vid.h"
#include "dsq.h"

class Env
{
    SDLVideoDriver vd;
    Heightmap_DSQ hmp;

public:
    Env(){};
    void startup()
    {
        vd.InitGraphics(800, 600);
        hmp.InitRandom(0, 46);
    }

    bool run()
    {
        SDL_Event ev;
        if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)) == -1) 
            return -1;

        hmp.SeedCorners();
        hmp.MakeHeightmap(1.333);
        
        hmp.HeightmapToRenderer(vd.getRenderSurface(), vd.getColors());

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

        return 0;
    }

    void shutdown()
    {
        vd.KillGraphics();
    }
};

int main() 
{
    Env plaz;

    plaz.startup();

    bool effresult;
    effresult = plaz.run();

    plaz.shutdown();

    return effresult;
}