#pragma once
#include "obj.h"
#include "edge.h"
#include "map.h"
#include <cassert>

class sect;

struct inter_strct {

	int nx_sect;
	edge *edg;
	vec2d poi;

};

struct plane_strct {

	vec2d verts[2];
	bool is_val;

};

// Vectors for corners (0 = Bottom left, 1 = Top left, 2 = Top right, 3 = Bottom right)
enum CORNER {

	BOTTOM_LEFT,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_RIGHT

};

class sect {
public:
	sect();
	sect(Uint32 tag, float z_fl, float z_ce, int tex_fl, int tex_ce);
	float get_z_fl();
	float get_z_ce();
	std::pair<inter_strct, bool> get_poi_wall_lim(vec2d poi, vec2d look, edge *inter);
	std::pair<inter_strct, bool> get_poi_wall(vec2d poi, vec2d look, edge *inter);
	std::vector<std::pair<edge*, int>*> get_sect_edgs();
	bool is_in_sec(vec2d p);
	std::pair<int, int> get_pl_lim(vec2d p, vec2d look, float z);
	void add_edg(edge *edg, int neigh);
	// Acc working functions (hopefully)
	bool is_in_fov(vec2d ppos, vec2d look, vec2d lfov, vec2d rfov);
	plane_strct get_pois_pl(edge *ed);
	void draw_sect_walls(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, map *mp, int cs_id, int rec=0, std::vector<bool> sects_vis=std::vector<bool>());
	~sect();
private:
	Uint32 tag;
	float z_fl, z_ce;
	int tex_fl, tex_ce;
	std::vector<std::pair<edge*, int>*> sect_edgs;
};