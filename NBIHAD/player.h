#pragma once
#include "obj.h"
#include "map.h"

#define ANG_MAX (60.f * M_PI / 180.f)

class player : public obj {
public:
	player();
	player(vec3d pos, float ang);
	void update(float dt, std::vector<obj*> objs, map *mp);
	void draw(SDL_Surface *srf, obj *pl, map *mp, std::vector<float> *z_buffer);
	~player();
private:
	int mx, my, omx, omy;
	float ang_y;
	int ms; // Multiplier controlling mouse sensitivity
};