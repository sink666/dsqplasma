#include <SDL2/SDL.h>

class SDLVideoDriver 
{
    SDL_Window *window = nullptr;
    
public:
    void InitGraphics(int winw, int winh);
    void RenderFrame();
    void KillGraphics();
    // effect-specific stuff
    void CyclePalette();
    void ColorsToSurface(uint8_t *pixels);
    void HeightmapToRenderer();
};