// Object base class
#pragma once
#include "vec.h"

class map;
class sect;

enum obj_type {

	obj_base,
	obj_player

};

// Indicies of object array
enum ind {

	ind_pl, // Player
	ind_na // NAGITO

};

class obj {
public:
	obj();
	obj(char type, vec3d pos, float rad, float ht, bool is_grav, bool is_col, bool is_mov);

	char get_type();
	void set_type(char type);

	vec3d get_pos();
	void set_pos(vec3d pos);
	
	vec3d get_vel();
	void set_vel(vec3d vel);

	float get_ang();
	void set_ang(float ang);

	float get_rad();
	void set_rad(float rad);

	float get_ht();
	void set_ht(float ht);

	bool get_is_grav();
	void set_is_grav(bool is_grav);

	bool get_is_col();
	void set_is_col(bool is_col);

	bool get_is_mov();
	void set_is_mov(bool is_mov);

	int get_cur_sect();
	void set_cur_sect(int sec);

	float get_tsin();
	float get_tcos();

	float get_tsin_l();
	float get_tcos_l();

	float get_tsin_r();
	float get_tcos_r();

	void update_pos(float dt, std::vector<obj*> objs, map *mp);

	virtual void update(float dt, std::vector<obj*> objs, map *mp);
	virtual void draw(SDL_Surface *srf, obj *pl, map *mp, std::vector<float> *z_buffer);

	virtual ~obj();
protected:
	char type; // Object type
	vec2d oldpos_2d;
	vec3d pos; // Position vector
	vec3d vel; // Velocity vector
	float vel_mag;
	float ang; // Angle (in radians)
	float rad; // Radius of object (the volume of collision is a cylinder)
	float ht; // Height of object

	// Flags
	bool is_grav; // Is affected by gravity?
	bool is_col; // Affected by collision?
	bool is_mov; // Can move?

	//inline static std::vector<obj*> objs;
	SDL_Surface *srf;
	int cur_sect;

	// sin and cos of object
	float tsin, tcos;
	float tsin_l, tcos_l, tsin_r, tcos_r;
};