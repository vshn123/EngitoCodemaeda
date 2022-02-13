#pragma once
#include "global.h"
#include "vec.h"

class edge {
public:
	edge();
	edge(vec2d *v0, vec2d *v1, int tex_upp_fr, int tex_fr, int tex_low_fr, int tex_upp_bk, int tex_bk, int tex_low_bk, bool is_wal);
	int get_tex_upp_fr();
	int get_tex_fr();
	int get_tex_low_fr();
	int get_tex_bk();
	int get_tex_low_bk();
	int get_tex_upp_bk();
	std::pair<vec2d, bool> get_poi2d(edge *oth, bool s_lim, bool oth_lim);
	std::pair<vec2d, bool> raycast(edge *oth);
	vec2d *get_v0();
	vec2d *get_v1();
	void draw_col(SDL_Surface *srf, float z_ce, float z_fl, vec2d poi, Uint32 nx_sect, vec2d pl_pos, int x, int y0, int y1, int mod, std::vector<float> *z_buffer); // mod = Mode: 0 = Wall, 1 = Upper, 2 = Bottom
	~edge();

	static void load(SDL_Surface *srf);
	static void destroy();
protected:
	inline static SDL_Surface *tex_smile;
	vec2d *v0, *v1;
	bool is_wal;
	int tex_upp_fr, tex_fr, tex_low_fr, tex_upp_bk, tex_bk, tex_low_bk;
};