#ifndef CUBOID_H
#define CUBOID_H

typedef struct Cuboid
{
	double x;
	double y;
	double z;
} Cuboid;

void set_size(Cuboid* cuboid, double x, double y, double z);

double calc_surface(const Cuboid* cuboid);
double calc_volume(const Cuboid* cuboid);

#endif
