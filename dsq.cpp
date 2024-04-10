#include "dsq.h"

int field[513][513];
size_t real_size = arrlen(field);
size_t sz = arrlen(field) - 1;

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

void Heightmap_DSQ::Seed_Corners()
{
    field[0][0] = rnd(rndmin, rndmax);
    field[sz][0] = rnd(rndmin, rndmax);
    field[0][sz] = rnd(rndmin, rndmax);
    field[sz][sz] = rnd(rndmin, rndmax);
}

float Heightmap_DSQ::Average_Square(int x, int y, int side_length)
{
    return field[x][y] +
           field[x + side_length][y] +
           field[x][y + side_length] +
           field[x + side_length][y + side_length];
}

float Heightmap_DSQ::Average_Diamond(int x, int y, int half_side)
{
    float acc = 0.0;

    int loc = (x - half_side); // left of center
    int roc = (x + half_side); // right of center
    int dfc = (y + half_side); // down from center
    int ufc = (y - half_side); // up from center

    if(loc >= 0)
        acc += field[loc][y];
    
    if(roc < real_size)
        acc += field[roc][y];

    if(dfc >= 0)
        acc += field[x][dfc];
    
    if(ufc < real_size)
        acc += field[x][ufc];

    return acc;
}

void Heightmap_DSQ::Make_Heightmap(double vh)
{
    double val_height = vh;
    for(int side_length = sz; side_length >= 2; side_length /= 2, val_height /= 2.0)
    {
        int half_side = side_length / 2;

        // generate square values
        for(int x = 0; x < sz; x += side_length) {
            for(int y = 0; y < sz; y += side_length) {
                float avg = Average_Square(x, y, side_length);
                avg /= 4.0f;

                avg = avg + (rnd(rndmin, rndmax) * 2 * val_height) - val_height;
                if(avg > rndmax)
                    avg = rndmin;

                field[x + half_side][y + half_side] = avg;
            }
        }

        // generate diamond values
        for(int x = 0; x < real_size; x += half_side) {
            for(int y = (x + half_side) % side_length; y < real_size; y += side_length) {
                float avg = Average_Diamond(x, y, half_side);
                avg /= 4.0f;

                avg = avg + (rnd(rndmin, rndmax) * 2 * val_height) - val_height;
                if(avg > rndmax)
                    avg = rndmin;

                field[x][y] = avg;
            }
        }
    }
}