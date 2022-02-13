#include "edge.h"

edge::edge() {

	v0 = nullptr;
	v1 = nullptr;
	is_wal = false;

	this->tex_upp_fr = 0;
	this->tex_fr = 0;
	this->tex_low_fr = 0;
	this->tex_upp_bk = 0;
	this->tex_bk = 0;
	this->tex_low_bk = 0;

}

edge::edge(vec2d *v0, vec2d *v1, int tex_upp_fr, int tex_fr, int tex_low_fr, int tex_upp_bk, int tex_bk, int tex_low_bk, bool is_wal) {

	this->v0 = v0;
	this->v1 = v1;
	this->is_wal = is_wal;

	this->tex_upp_fr = tex_upp_fr;
	this->tex_fr = tex_fr;
	this->tex_low_fr = tex_low_fr;
	this->tex_upp_bk = tex_upp_bk;
	this->tex_bk = tex_bk;
	this->tex_low_bk = tex_low_bk;

	//tex = IMG_Load("../res/img/test.png");

}

int edge::get_tex_upp_fr() {

	return tex_upp_fr;

}

int edge::get_tex_fr() {

	return tex_fr;

}

int edge::get_tex_low_fr() {

	return tex_low_fr;

}

int edge::get_tex_upp_bk() {

	return tex_upp_bk;

}


int edge::get_tex_bk() {

	return tex_bk;

}

int edge::get_tex_low_bk() {

	return tex_low_bk;

}

// Get 2D point of intersection (xy)
std::pair<vec2d, bool> edge::get_poi2d(edge *oth, bool s_lim, bool oth_lim) {

	vec2d a = *v0;
	vec2d b = *v1 - *v0;
	vec2d c = *oth->v0;
	vec2d d = *oth->v1 - *oth->v0;

	// Lies on the same plane
	if (b.crs_pr(d) == 0)
		return std::pair<vec2d, bool>(vec2d(0, 0), false);

	float la = (d.get_x() * (c.get_y() - a.get_y()) + d.get_y() * (a.get_x() - c.get_x())) / (d.get_x() * b.get_y() - b.get_x() * d.get_y());
	float mu = .0f;

	if (d.get_x() != 0)
		mu = (a.get_x() - c.get_x() + la * b.get_x()) / d.get_x();
	else
		mu = (a.get_y() - c.get_y() + la * b.get_y()) / d.get_y();

	// Range check
	if (((la >= 0 && la <= 1) || !s_lim) && ((mu >= 0 && mu <= 1) || !oth_lim))
		return std::pair<vec2d, bool>(vec2d(a.get_x() + la * b.get_x(), a.get_y() + la * b.get_y()), true);

	return std::pair<vec2d, bool>(vec2d(0, 0), false);

}

std::pair<vec2d, bool> edge::raycast(edge *oth) {

	vec2d a = *v0;
	vec2d b = *v1 - *v0;
	vec2d c = *oth->v0;
	vec2d d = *oth->v1 - *oth->v0;

	// Lies on the same plane
	if (b.get_x() * d.get_y() - d.get_x() * b.get_y() == 0)
		return std::pair<vec2d, bool>(vec2d(0, 0), false);

	float la = .0f;
	float mu = .0f;

	la = (d.get_x() * (c.get_y() - a.get_y()) + d.get_y() * (a.get_x() - c.get_x())) / (d.get_x() * b.get_y() - b.get_x() * d.get_y());
	
	if (d.get_x() != 0)
		mu = (a.get_x() - c.get_x() + la * b.get_x()) / d.get_x();
	else
		mu = (a.get_y() - c.get_y() + la * b.get_y()) / d.get_y();

	// Range check
	if (la >= 0 && mu >= 0 && mu <= 1)
		return std::pair<vec2d, bool>(vec2d(a.get_x() + la * b.get_x(), a.get_y() + la * b.get_y()), true);

	return std::pair<vec2d, bool>(vec2d(0, 0), false);

}

vec2d *edge::get_v0() {

	return v0;

}

vec2d *edge::get_v1() {

	return v1;

}

void edge::draw_col(SDL_Surface *srf, float z_ce, float z_fl, vec2d poi, Uint32 nx_sect, vec2d pl_pos, int x, int y0, int y1, int mod, std::vector<float> *z_buffer) {

	vec2d norm = (*v1 - *v0).get_norm();
	vec2d rel_pos = poi - pl_pos;
	int tex = 0;

	if (rel_pos.dot_pr(norm) > 0)
		tex = tex_bk;
	else
		tex = tex_fr;
		
	//SDL_Surface *tex_srf = /*get_tex(tex)*/tex_smile;

	//std::cout << (tex_srf == nullptr) << " " << tex << std::endl;

	if (tex_smile == nullptr) {

		draw_col_gbl(srf, x, y0, y1, 0xFFFFFFFF, z_buffer);
		return;

	}

	float zm = std::fabsf(z_ce - z_fl) / 2.f;

	//std::cout << zm << std::endl;

	for (int y = std::max(y0, 0); y < std::min(y1, win_he - 1); ++y) {
	
		float xra = std::abs((poi - *v0).dot_pr((*v1 - *v0) / (*v1 - *v0).get_len()));
		xra = (xra - int(xra)) * 0xFF;
		
		float yra = ((float)y - y0) / ((float)(y1 - y0) / zm);
		yra = (yra - int(yra)) * float(tex_smile->h - 1);

		Uint32 col = get_pix(tex_smile, xra, yra);
		
		put_pix(srf, (int)x, (int)y, col, z_buffer);
	
	}

}

edge::~edge() {

	

}

void edge::load(SDL_Surface *srf) {

	tex_smile = load_srf("../res/img/test.png", srf);

}

void edge::destroy() {

	SDL_FreeSurface(tex_smile);

}