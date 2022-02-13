#include "player.h"

player::player() {

	pos = vec3d(0, 0, 0);
	type = obj_player;
	vel = vec3d(0, 0, 0);

	rad = .1f;
	ht = 1.f;

	is_grav = false;
	is_col = false;
	is_mov = false;

	ang = .0f;
	ang_y = .0f;
	cur_sect = 0;

	SDL_GetMouseState(&mx, &my);
	ms = -1;

}

player::player(vec3d pos, float ang) {

	this->pos = pos;
	type = obj_player;
	vel = vec3d(0, 0, 0);

	rad = .1f;
	ht = 1.f;

	is_grav = false;
	is_col = false;
	is_mov = false;

	this->ang = ang;
	cur_sect = 0;

	SDL_GetMouseState(&mx, &my);
	ms = -1;

	ang_y = .0f;

}

void player::update(float dt, std::vector<obj*> objs, map *mp) {

	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);

	omx = mx;
	omy = my;

	SDL_GetMouseState(&mx, &my);
	
	int dmx = mx - omx;
	int dmy = my - omy;

	ang -= (float)dmx * std::pow(1.5f, ms) * dt;
	//ang_y -= (float)dmy * std::pow(1.5f, ms) * dt;

	if (ang_y < -ANG_MAX)
		ang_y = -ANG_MAX;

	if (ang_y > ANG_MAX)
		ang_y = ANG_MAX;

	oldpos_2d = vec2d(pos.get_x(), pos.get_y());

	const Uint8 *keys = SDL_GetKeyboardState(nullptr);

	float vel_mag = 4.f;
	vel = vec3d(0, 0, 0);

	if (keys[SDL_SCANCODE_D])
		vel = vel + vec3d(tsin, -tcos, 0);

	if (keys[SDL_SCANCODE_A])
		vel = vel + vec3d(-tsin, tcos, 0);

	if (keys[SDL_SCANCODE_W])
		vel = vel + vec3d(tcos, tsin, 0);

	if (keys[SDL_SCANCODE_S])
		vel = vel + vec3d(-tcos, -tsin, 0);

	if (keys[SDL_SCANCODE_RIGHT])
		ang -= M_PI / 2.f * dt;

	if (keys[SDL_SCANCODE_LEFT])
		ang += M_PI / 2.f * dt;

	//obj::update(dt, objs, mp);
	//std::cout << cur_sect << std::endl;

	//this->update_pos(dt, objs, mp);
	vel = vel.get_norm() * vel_mag;
	pos = pos + vel * dt;
	pos = vec3d(pos.get_x(), pos.get_y(), mp->get_sect_in(cur_sect)->get_z_fl() + ht);

	tsin = std::sin(ang);
	tcos = std::cos(ang);

	tsin_l = std::sin(ang + fov / 2.f), tcos_l = std::cos(ang + fov / 2.f), tsin_r = std::sin(ang - fov / 2.f), tcos_r = std::cos(ang - fov / 2.f);

	// TODO: Fix bug involving sector transfer whilst parralel to sector edge
	if (mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).second &&\
		mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect != -1)
		cur_sect = mp->get_sect_in(cur_sect)->get_poi_wall_lim(oldpos_2d, vec2d(vel.get_x() * dt, vel.get_y() * dt), nullptr).first.nx_sect;

	//this->update_pos(dt, objs, mp);

}

void player::draw(SDL_Surface *srf, obj *pl, map *mp, std::vector<float> *z_buffer) {

	/*
	Rough overview of render process:
	Draw Floors (raytrace)
	Draw Walls (raycast)
	Draw Sprites (Z-buf)
	*/

	std::vector<int> vis_sects;

	vec2d ppos = vec2d(pl->get_pos().get_x(), pl->get_pos().get_y());
	vec2d plookl(tcos_l, tsin_l);
	vec2d plookr(tcos_r, tsin_r);

	vec2d look(tcos, tsin);

	// Determine visible sectors
	for (int i = 0; i < mp->get_sects()->size(); ++i)
		if ((*mp->get_sects())[i]->is_in_fov(ppos, look, plookl, plookr))
			vis_sects.push_back(i);

	//std::cout << vis_sects.size() << "\n";

	mp->rend_planes(srf, this, z_buffer, &vis_sects, ang_y);
	clear_z_buf(z_buffer);
	mp->rend_walls(srf, this, z_buffer, &vis_sects, cur_sect);

}

player::~player() {

	

}