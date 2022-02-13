#pragma once
#include "handle.h"

#define img_w (640)
#define img_h (480)

#define mus_size (326533)

class ac_hand : public handle {
public:
	ac_hand(SDL_Surface *win_srf);
	char update(float dt);
	void draw(SDL_Surface *win_srf, std::vector<float> *z_buffer);
	~ac_hand();
private:
	float ang;
	float y_off;
	float x_off;

	float rev_cnt; // Reverse rotation
	int rev_wait; // (in ms)
	Uint8 rev;

	Mix_Music *mus;
	SDL_Surface *cursed;
	SDL_Surface *warn;

#define chi_size (6)
	SDL_Surface *chis[chi_size];
	int chi;
	float chis_tim;
	int chis_wait;
	Uint8 chis_bool;
};