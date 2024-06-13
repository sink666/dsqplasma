#include <random>
#include <iostream>
#include <SDL2/SDL.h>

class Heightmap_DSQ
{
    int rndmin;
    int rndmax;
    float AverageSquare(int x, int y, int side_length);
    float AverageDiamond(int x, int y, int half_side);
public:
    Heightmap_DSQ(){};

    void InitRandom(int rmin, int rmax);
    void SeedCorners();
    void MakeHeightmap(double vh);

    void HeightmapToRenderer(SDL_Surface *renderSurface, SDL_Color *colors);
};