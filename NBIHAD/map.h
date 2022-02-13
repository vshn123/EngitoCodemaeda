//NOTE: 1 map unit = 256 pixels
#pragma once
#include "obj.h"
#include "vec.h"
#include "edge.h"
#include "sect.h"

class map {
public:
	map(bool hard, SDL_Surface *srf);
	void add_vert(vec2d vec);
	void add_edge(int tex_upp_fr, int tex_fr, int tex_low_fr, int tex_upp_bk, int tex_bk, int tex_low_bk, Uint32 v0, Uint32 v1, bool is_wal); // (tex_upp_fr, tex_fr, tex_low_fr, tex_upp_bk, tex_bk, tex_low_bk, v1, v2, is_wal)
	std::vector<edge*> *get_edges();
	void add_sect(Uint32 tag, float z_fl, float z_ce, int tex_fl, int tex_ce, std::vector<std::pair<int, int>> edgs); // (tag, z_fl, z_ce, tex_fl, tex_ce, edges (edg, neighbour))
	std::vector<sect*> *get_sects();
	sect *get_sect_in(int in);
	// Floors + ceilings (planes) (y-z plane)
	void rend_row(SDL_Surface *srf, int y, obj *pl, vec3d look, /*int cs*/std::vector<int> *vis_sects, std::vector<float> *z_buffer);
	void rend_planes(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, std::vector<int> *vis_sects, float ang_off);
	// Walls
	void rend_col(SDL_Surface *srf, int x, obj *pl, vec2d look, int cs, vec2d *poi, edge *inter, std::vector<float> *z_buffer, int n_rec);
	void rend_walls(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, std::vector<int> *vis_sects, int cs_id);
	~map();
private:
	SDL_Surface *tex_smile;
	std::vector<vec2d*> verts;
	std::vector<edge*> edgs;
	std::vector<sect*> sects;
};