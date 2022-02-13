// Class for handling game states
#pragma once
#include "global.h"

enum handle_state {

	st_ti, // Title
	st_ac, // Anti-cheat ;D
	st_gm, // Game
	st_go, // Game over
	st_ge // Game end

};

class handle {
public:
	handle();

	char get_type();

	virtual char update(float dt); // Returns -1 if there is no change in game state
	virtual void draw(SDL_Surface *win_srf, std::vector<float> *z_buffer);
	
	virtual ~handle();

	static void init(SDL_Window *win, SDL_Surface *win_srf);
	static char get_cur_type();
	static void update_hnds(float dt, SDL_Surface *win_srf);
	static void draw_hnds(SDL_Surface *win_srf, std::vector<float> *z_buffer);
	static void destroy();
protected:
	char st;
	inline static handle* hnd; // Handlers (index corresponds to type)
};