#include <random>
#include <iostream>

#define arrlen(a) (sizeof(a) / sizeof(*a))

extern int field[513][513];
extern size_t real_size;
extern size_t sz;

class Heightmap_DSQ
{
    int rndmin;
    int rndmax;
    float Average_Square(int x, int y, int side_length);
    float Average_Diamond(int x, int y, int half_side);
public:
    Heightmap_DSQ(int rmin, int rmax): rndmin(rmin), rndmax(rmax) {}
    void Seed_Corners();
    void Make_Heightmap(double vh);
};