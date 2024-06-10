#include <SDL2/SDL.h>

class SDLVideoDriver 
{
    SDL_Window *window = nullptr;
    
public:
    SDLVideoDriver(){};

    void InitGraphics(int winw, int winh);
    void RenderFrame();
    void KillGraphics();
    void CyclePalette();
    SDL_Surface *getRenderSurface();
    SDL_Color *getColors();
};