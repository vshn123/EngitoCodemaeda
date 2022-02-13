// Game handler, where all the gaming happens
#pragma once
#include "handle.h"
#include "obj.h"
#include "player.h"

enum game_state {

	gm_pl, // Main game play (FPS mode)
	gm_qu, // Questions
	gm_ex  // Exit question

};

class game_hand : public handle {
public:
	game_hand(SDL_Surface *win_srf);
	char update(float dt);
	void draw(SDL_Surface *win_srf, std::vector<float> *z_buffer);
	~game_hand();
private:
	SDL_Surface *esc;
	std::vector<obj*> objs;
	map *mp;
};