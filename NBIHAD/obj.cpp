#include "obj.h"
#include "sect.h"
#include "map.h"

obj::obj() {

	type = 0;

	pos = vec3d(0, 0, 0);
	vel = vec3d(0, 0, 0);

	rad = .0f;
	ht = .0f;

	is_grav = false;
	is_col = false;
	is_mov = false;

	ang = .0f;

	vel_mag = .0f;

}

obj::obj(char type, vec3d pos, float rad, float ht, bool is_grav, bool is_col, bool is_mov) {

	this->type = type;

	this->pos = pos;
	this->vel = vec3d(0, 0, 0);

	this->rad = rad;
	this->ht = ht;

	this->is_grav = is_grav;
	this->is_col = is_col;
	this->is_mov = is_mov;

	ang = .0f;
	vel_mag = .0f;

}

char obj::get_type() {

	return type;

}

void obj::set_type(char type) {

	this->type = type;

}

vec3d obj::get_pos() {

	return pos;

}

void obj::set_pos(vec3d pos) {

	this->pos = pos;

}

vec3d obj::get_vel() {

	return vel;

}

void obj::set_vel(vec3d vel) {

	this->vel = vel;

}

float obj::get_ang() {

	return ang;

}

void obj::set_ang(float ang) {

	this->ang = ang;

}

float obj::get_rad() {

	return rad;

}

void obj::set_rad(float rad) {

	this->rad = rad;

}

float obj::get_ht() {

	return ht;

}

void obj::set_ht(float ht) {

	this->ht = ht;

}

bool obj::get_is_grav() {

	return is_grav;

}

void obj::set_is_grav(bool is_grav) {

	this->is_grav = is_grav;

}

bool obj::get_is_col() {

	return is_col;

}

void obj::set_is_col(bool is_col) {

	this->is_col = is_col;
	
}

bool obj::get_is_mov() {

	return is_mov;

}

void obj::set_is_mov(bool is_mov) {

	this->is_mov = is_mov;

}

int obj::get_cur_sect() {

	return cur_sect;

}

void obj::set_cur_sect(int sec){

	this->cur_sect = sec;

}

float obj::get_tsin() {

	return tsin;

}

float obj::get_tcos() {

	return tcos;

}

float obj::get_tsin_l() {

	return tsin_l;

}

float obj::get_tcos_l() {

	return tcos_l;

}

float obj::get_tsin_r() {

	return tsin_r;

}

float obj::get_tcos_r() {

	return tcos_r;

}

void obj::update_pos(float dt, std::vector<obj*> objs, map *mp) {

	/*vel = vel.get_norm() * vel_mag;
	pos = pos + vel * dt;
	pos = vec3d(pos.get_x(), pos.get_y(), mp->get_sect_in(cur_sect)->get_z_fl() + ht);
	*/
	tsin = std::sin(ang);
	tcos = std::cos(ang);

	tsin_l = std::sin(ang + fov / 2.f), tcos_l = std::cos(ang + fov / 2.f), tsin_r = std::sin(ang - fov / 2.f), tcos_r = std::cos(ang - fov / 2.f);

	/*if (mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).second &&\
		mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect != -1)
		cur_sect = mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect;
		*/

	vel = vel.get_norm() * vel_mag;
	pos = pos + vel * dt;
	pos = vec3d(pos.get_x(), pos.get_y(), mp->get_sect_in(cur_sect)->get_z_fl() + ht);

	// TODO: Fix bug involving sector transfer whilst parralel to sector edge
	if (mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).second &&\
		mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect != -1)
		cur_sect = mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect;
}

void obj::update(float dt, std::vector<obj*> objs, map *mp) {

	

}

void obj::draw(SDL_Surface *srf, obj *pl, map *mp, std::vector<float> *z_buffer) {

	

}

obj::~obj() {

	

}