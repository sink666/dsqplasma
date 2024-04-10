#include "vid.h"
#include "dsq.h"

SDL_Surface *renderSurface;
SDL_Surface *rgbaSurface;
SDL_Renderer *renderer;
SDL_Texture *sdltex;
SDL_Color *colors;
SDL_Rect destrect;

void SDLVideoDriver::InitGraphics(int winw, int winh) 
{
    destrect = { 0, 0, winw, winh };

    // set up colors
    colors = new SDL_Color[47];
    colors[0].r = colors[0].g = colors[0].b = 0;
    int range[] = {32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
    int i = 1;
    for(int rr = 0; i < 16; ++i, ++rr) 
        colors[i].r = (Uint8)range[rr];

    for(int gr = 0; i < 31; ++i, ++gr) 
        colors[i].g = (Uint8)range[gr];

    for(int br = 0; i < 46; ++i, ++br)
        colors[i].b = (Uint8)range[br];

    window = SDL_CreateWindow(
        "bad plasma", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        winw, winh, 
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
    sdltex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, winw, winh);
    renderSurface = SDL_CreateRGBSurfaceWithFormat(0, winw, winh, 0, SDL_PIXELFORMAT_INDEX8);
    rgbaSurface = SDL_CreateRGBSurfaceWithFormat(0, winw, winh, 0, SDL_PIXELFORMAT_RGBA32);

    // set logical size and hinting; SDL will do scaling behind the scenes to make it fit right regardless of dpi
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, winw, winh);

    // set initial palette
    SDL_SetPaletteColors(renderSurface->format->palette, colors, 0, 47);
}

void SDLVideoDriver::RenderFrame() 
{
    // surface to surface to convert format, surface to texture to render
    SDL_BlitSurface(renderSurface, nullptr, rgbaSurface, nullptr);
    SDL_UpdateTexture(sdltex, nullptr, rgbaSurface->pixels, rgbaSurface->pitch);
    SDL_RenderCopy(renderer, sdltex, nullptr, &destrect);
    SDL_RenderPresent(renderer);
}

void SDLVideoDriver::KillGraphics() 
{
    if(sdltex) 
    {
        SDL_DestroyTexture(sdltex);
        sdltex = nullptr;
    }
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_QuitSubSystem(SDL_INIT_VIDEO|SDL_INIT_TIMER);
}

void SDLVideoDriver::CyclePalette()
{
    SDL_Color save = colors[1];
    for (int i = 1; i < 47; ++i) 
    {
        colors[i] = colors[i + 1];
    }
    colors[46] = save;

    SDL_SetPaletteColors(renderSurface->format->palette, colors, 0, 47);
}

void SDLVideoDriver::ColorsToSurface(uint8_t *pixels)
{
    uint8_t *dest = (pixels);

    for(int fy = 0; fy < real_size; ++fy) {
        for(int fx = 0; fx < real_size; ++fx) {
            *dest = field[fx][fy];
            dest += 1;
        }
        dest += (800 - real_size);
    }
}

void SDLVideoDriver::HeightmapToRenderer()
{
    SDL_LockSurface(renderSurface);
    uint8_t *rsurf_px = static_cast<uint8_t *>(renderSurface->pixels);
    ColorsToSurface(rsurf_px);
    SDL_UnlockSurface(renderSurface);
}