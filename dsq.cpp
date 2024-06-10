#include "dsq.h"

uint8_t field[513*513];
size_t sz = 512;
size_t real_size = 513;

int getIndex(int x, int y)
{
    return x + y * 513; // since its square
}

SDL_Rect targetRect { 130, 30, 512, 512 }; // x:130 y:30 512x512

int rnd(int min, int max)
{
    using Dist = std::uniform_int_distribution<int>;

    static std::random_device rd;
    static std::default_random_engine re(rd());
  	static Dist uid {};

    // operator() is overloaded, it takes the random engine
    // and clamps it to min/max
  	return uid(re, Dist::param_type{min,max});
}

void Heightmap_DSQ::InitRandom(int rmin, int rmax)
{
    rndmin = rmin;
    rndmax = rmax;
}

void Heightmap_DSQ::SeedCorners()
{
    field[getIndex(0, 0)] = rnd(rndmin, rndmax);
    field[getIndex(512, 0)] = rnd(rndmin, rndmax);
    field[getIndex(0, 512)] = rnd(rndmin, rndmax);
    field[getIndex(512, 512)] = rnd(rndmin, rndmax);
}

float Heightmap_DSQ::AverageSquare(int x, int y, int side_length)
{
    return field[getIndex(x, y)] +
           field[getIndex(x + side_length, y)] +
           field[getIndex(x, y + side_length)] +
           field[getIndex(x + side_length, y + side_length)];
}

float Heightmap_DSQ::AverageDiamond(int x, int y, int half_side)
{
    float acc = 0.0;

    int loc = (x - half_side); // left of center
    int roc = (x + half_side); // right of center
    int dfc = (y + half_side); // down from center
    int ufc = (y - half_side); // up from center

    if(loc >= 0)
        acc += field[getIndex(loc, y)];
    
    if(roc < real_size)
        acc += field[getIndex(roc, y)];

    if(dfc >= 0)
        acc += field[getIndex(x, dfc)];
    
    if(ufc < real_size)
        acc += field[getIndex(x, ufc)];

    return acc;
}

void Heightmap_DSQ::MakeHeightmap(double vh)
{
    double val_height = vh;
    for(int side_length = sz; side_length >= 2; side_length /= 2, val_height /= 2.0)
    {
        int half_side = side_length / 2;

        // generate square values
        for(int x = 0; x < sz; x += side_length) {
            for(int y = 0; y < sz; y += side_length) {
                float avg = AverageSquare(x, y, side_length);
                avg /= 4.0f;

                avg = avg + (rnd(rndmin, rndmax) * 2 * val_height) - val_height;
                if(avg > rndmax)
                    avg = rndmin;

                field[getIndex(x + half_side, y + half_side)] = avg;
            }
        }

        // generate diamond values
        for(int x = 0; x < real_size; x += half_side) {
            for(int y = (x + half_side) % side_length; y < real_size; y += side_length) {
                float avg = AverageDiamond(x, y, half_side);
                avg /= 4.0f;

                avg = avg + (rnd(rndmin, rndmax) * 2 * val_height) - val_height;
                if(avg > rndmax)
                    avg = rndmin;

                field[getIndex(x, y)] = avg;
            }
        }
    }
}

void Heightmap_DSQ::HeightmapToRenderer(SDL_Surface *renderSurface, SDL_Color *colors)
{
    SDL_Surface *heightmap = SDL_CreateRGBSurfaceWithFormatFrom(&field, 513, 513, 
                                                                8, 513, SDL_PIXELFORMAT_INDEX8);

    SDL_SetPaletteColors( heightmap->format->palette, colors, 0, 47);
    SDL_BlitSurface(heightmap, nullptr, renderSurface, &targetRect);
    SDL_FreeSurface(heightmap);
}