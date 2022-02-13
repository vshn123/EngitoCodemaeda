#pragma once
#include "handle.h"

class title_hand : public handle {
public:
	title_hand(SDL_Surface *win_srf);
	char update(float dt);
	void draw(SDL_Surface *win_srf, std::vector<float> *z_buffer);
	~title_hand();
private:
	SDL_Surface *tit;
	SDL_Surface *str;
	Mix_Music *mus;
	float ang;
};