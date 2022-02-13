#include "map.h"

map::map(bool hard, SDL_Surface *srf) {

	if (hard) {

		verts.push_back(new vec2d(-5, -5));
		verts.push_back(new vec2d(-5, 5));
		verts.push_back(new vec2d(0, 5));
		verts.push_back(new vec2d(5, 5));
		verts.push_back(new vec2d(0, 0));
		verts.push_back(new vec2d(5, -5));
		verts.push_back(new vec2d(0, -5));

		edgs.push_back(new edge(verts[1], verts[0], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[2], verts[1], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[3], verts[2], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[4], verts[3], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[5], verts[4], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[6], verts[5], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[0], verts[6], 0, 0, 0, 0, 0, 0, true));

		edgs.push_back(new edge(verts[4], verts[2], 0, 0, 0, 0, 0, 0, true));
		edgs.push_back(new edge(verts[6], verts[4], 0, 0, 0, 0, 0, 0, true));

		sects.push_back(new sect(0, -1, 1, 0, 0));
		sects.push_back(new sect(0, -1.5, 0.5, 0, 0));
		sects.push_back(new sect(0, -0.5, 1.5, 0, 0));

		sects[0]->add_edg(edgs[0], -1);
		sects[0]->add_edg(edgs[1], -1);
		sects[0]->add_edg(edgs[7], 1);
		sects[0]->add_edg(edgs[8], 2);
		sects[0]->add_edg(edgs[6], -1);

		sects[1]->add_edg(edgs[7], 0);
		sects[1]->add_edg(edgs[2], -1);
		sects[1]->add_edg(edgs[3], -1);

		sects[2]->add_edg(edgs[8], 0);
		sects[2]->add_edg(edgs[4], -1);
		sects[2]->add_edg(edgs[5], -1);

	}

	tex_smile = load_srf("../res/img/test.png", srf);

}

void map::add_vert(vec2d vec) {

	verts.push_back(new vec2d(vec.get_x(), vec.get_y()));

}

void map::add_edge(int tex_upp_fr, int tex_fr, int tex_low_fr, int tex_upp_bk, int tex_bk, int tex_low_bk, Uint32 v0, Uint32 v1, bool is_wal) {

	edgs.push_back(new edge(verts[v0], verts[v1], tex_upp_fr, tex_fr, tex_low_fr, tex_upp_bk, tex_bk, tex_low_bk, is_wal));

}

std::vector<edge*> *map::get_edges() {

	return &edgs;

}

void map::add_sect(Uint32 tag, float z_fl, float z_ce, int tex_fl, int tex_ce, std::vector<std::pair<int, int>> edgs) {

	sects.push_back(new sect(tag, z_fl, z_ce, tex_fl, tex_ce));

	for (auto el : edgs)
		sects[sects.size() - 1]->add_edg(this->edgs[el.first], el.second);

}

std::vector<sect*> *map::get_sects() {

	return &sects;

}

sect *map::get_sect_in(int in) {

	if (in < 0 || in >= sects.size())
		return nullptr;

	return sects[in];

}

#define MAX_DEPTH (64)
#define vfov (M_PI / 2.f)

/* TODO: Rework so it recursively calls for each neighbour
1 - Recursively search sectors until intersect/none is found
2 - Recursively search left and right of point of intersection until non level/neighbourless sector found (edge) 
*/
void map::rend_row(SDL_Surface *srf, int y, obj *pl, vec3d look, std::vector<int> *vis_sects, std::vector<float> *z_buffer) {

	// Recursively draw neighbours
	vec2d plookl(pl->get_tcos_l(), pl->get_tsin_l());
	vec2d plookr(pl->get_tcos_r(), pl->get_tsin_r());
	vec2d look2d(look.get_x(), look.get_y());

	vec2d ppos(pl->get_pos().get_x(), pl->get_pos().get_y());
	vec2d lpos = ppos + plookl;
	vec2d rpos = ppos + plookr;

	edge fl(&ppos, &lpos, 0, 0, 0, 0, 0, 0, false);
	edge fr(&ppos, &rpos, 0, 0, 0, 0, 0, 0, false);

	//bool no_rec = true;

	for (int cs_id : *vis_sects) {

		// Draw plane
		float z_pl = (look.get_z() > 0) ? sects[cs_id]->get_z_ce() : sects[cs_id]->get_z_fl();
		float la = (z_pl - pl->get_pos().get_z()) / look.get_z(); // Scale factor

		if (la > 0) {

			vec2d pl_in2d = ppos + look2d * la;
			float in_len = (pl_in2d - ppos).get_len();

			vec2d nrm = look2d.get_nrm2d().get_norm() /* -1*/;
			vec2d spl_vec = pl_in2d + nrm;

			edge sp(&pl_in2d, &spl_vec, 0, 0, 0, 0, 0, 0, false); // Screen plane

			plane_strct ints = sects[cs_id]->get_pois_pl(&sp);

			if (ints.is_val) {

				float ar = 1.f;//(float)win_wi / win_he;

				float nrm_pos_l = ((ints.verts[0] - pl_in2d).dot_pr(nrm) * ar) /* 2.f*/, nrm_pos_r = ((ints.verts[1] - pl_in2d).dot_pr(nrm) * ar) /* 2.f*/;
				int lx = float(win_wi - 1) * (nrm_pos_l / in_len + 1.f) / 2.f, rx = float(win_wi - 1) * (nrm_pos_r / in_len + 1.f) / 2.f;

				int temp = rx;
				rx = std::max(lx, rx);

				if (rx != temp) {

					vec2d tmp = ints.verts[1];
					ints.verts[1] = ints.verts[0];
					ints.verts[0] = tmp;

				}

				lx = std::min(lx, temp);

				if (!((lx < 0 && rx < 0) || (lx >= win_wi && rx >= win_wi)))
					for (int x = std::max(0, lx); x < std::min(win_wi, rx); ++x) {

						vec2d ip = ints.verts[0] + (ints.verts[1] - ints.verts[0]) * (float(x - lx) / float(rx - lx));

						float xra = ((ip.get_x() - (int)ip.get_x() < 0) ? float(1.f - std::abs(ip.get_x() - (int)ip.get_x())) : float(std::abs(ip.get_x() - (int)ip.get_x()))) * 255.f \
							, yra = ((ip.get_y() - (int)ip.get_y() < 0) ? float(1.f - std::abs(ip.get_y() - (int)ip.get_y())) : float(std::abs(ip.get_y() - (int)ip.get_y()))) * 255.f;

						/*const int MN = 1.f;
						float mul = 1.f / (in_len * MN + 1.f);

						Uint8 r = 0, g = 0, b = 0;
						Uint32 col = get_pix(tex_smile, xra, yra);
						SDL_GetRGB(col, tex_smile->format, &r, &g, &b);

						r = r * mul;
						g = g * mul;
						b = b * mul;*/
						
						//col = SDL_MapRGB(tex_smile->format, r, g, b);
						
						Uint32 col = get_pix(tex_smile, xra, yra);

						put_pix(srf, (int)x, (int)y, col, z_buffer, in_len);

					}

			}

		}

	}

}

void map::rend_planes(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, std::vector<int> *vis_sects, float ang_off) {

	for (int y = 0; y < win_he; ++y) {

		//std::cout << pl->get_tcos() << " " << pl->get_tsin() << "\n";

		vec2d plook(pl->get_tcos(), pl->get_tsin());
		vec3d look = vec3d(plook.get_x(), plook.get_y(), 1.f - 2.f * (float(y) / (win_he - 1)) /*+ std::tan(ang_off)*/); // Raycast
		rend_row(srf, y, pl, look, vis_sects, z_buffer);

	}

}

void map::rend_col(SDL_Surface *srf, int x, obj *pl, vec2d look, int cs_id, vec2d *poi, edge *inter, std::vector<float> *z_buffer, int n_rec=0) {

	if (n_rec >= MAX_DEPTH)
		return;

	vec2d pl_2d(pl->get_pos().get_x(), pl->get_pos().get_y());

	if (poi == nullptr)
		poi = &pl_2d;

	sect *cs = get_sect_in(cs_id);

	std::pair<inter_strct, bool> edg_inter_pr = cs->get_poi_wall(*poi, look, inter);
	inter_strct edg_inter = edg_inter_pr.first;

	if (edg_inter_pr.second) {

		float z = (edg_inter.poi - pl_2d).get_len() * std::cos(std::abs(fov / 2.f - ((float(x) / (win_wi - 1)) * fov)));

		// Neighbourless edge/wall
		if (edg_inter.nx_sect == -1) {

			float y0 = (win_he - 1) * (1.f - ((cs->get_z_ce() - pl->get_pos().get_z()) / (z))) / 2.f;
			float y1 = (win_he - 1) * (1.f - ((cs->get_z_fl() - pl->get_pos().get_z()) / (z))) / 2.f;

			if (y0 >= 0)
				draw_col_gbl(srf, x, 0, y0, SDL_MapRGB(srf->format, 0x10, 0x0, 0x10), z_buffer, z);

			edg_inter.edg->draw_col(srf, cs->get_z_ce(), cs->get_z_fl(), edg_inter.poi, edg_inter.nx_sect, pl_2d, x, y0, y1, 0, z_buffer);

			if (y1 < win_he)
				draw_col_gbl(srf, x, y1, win_he - 1, SDL_MapRGB(srf->format, 0, 0, 0xFF), z_buffer, z);

			return;

		}

		// Recursion time
		rend_col(srf, x, pl, look, edg_inter.nx_sect, &edg_inter.poi, edg_inter.edg, z_buffer, n_rec + 1);

		// Render portal edge
		// Upper
		if (cs->get_z_ce() > get_sect_in(edg_inter.nx_sect)->get_z_ce()) {

			float y0 = (win_he - 1) * (1.f - ((get_sect_in(edg_inter.nx_sect)->get_z_ce() - pl->get_pos().get_z()) / (z))) / 2.f;
			float y1 = (win_he - 1) * (1.f - ((cs->get_z_ce() - pl->get_pos().get_z()) / (z))) / 2.f;

			draw_col_gbl(srf, x, y0, y1, SDL_MapRGB(srf->format, 0xFF, 0x0, 0x0), z_buffer, z);

			if (y0 >= 0)
				draw_col_gbl(srf, x, 0, y0, SDL_MapRGB(srf->format, 0xFF, 0xFF, 0xFF), z_buffer, z);

		}

		// Lower
		if (cs->get_z_fl() < get_sect_in(edg_inter.nx_sect)->get_z_fl()) {

			float y0 = (win_he - 1) * (1.f - ((cs->get_z_fl() - pl->get_pos().get_z()) / (z))) / 2.f;
			float y1 = (win_he - 1) * (1.f - ((get_sect_in(edg_inter.nx_sect)->get_z_fl() - pl->get_pos().get_z()) / (z))) / 2.f;

			draw_col_gbl(srf, x, y0, y1, SDL_MapRGB(srf->format, 0x0, 0xFF, 0x0), z_buffer, z);

			if (y1 < win_he)
				draw_col_gbl(srf, x, y1, win_he - 1, SDL_MapRGB(srf->format, 0xA0, 0xA0, 0xA0), z_buffer, z);

		}

	}

}

// TODO: Make iteration common with both walls and floors
void map::rend_walls(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, std::vector<int> *vis_sects, int cs_id) {

	//SDL_Surface *srf, int x, obj *pl, vec2d look, int cs_id, vec2d *poi, edge *inter, std::vector<float> *z_buffer, int n_rec = 0
	/*for (int x = 0; x < win_wi; ++x) {
	
		vec2d look(std::cos((pl->get_ang() + fov / 2.f - (float(x) / (win_wi - 1) * fov))), std::sin((pl->get_ang() + fov / 2.f - (float(x) / (win_wi - 1) * fov))));
		rend_col(srf, x, pl, look, cs_id, nullptr, nullptr, z_buffer);
	
	}*/

	sect *cur_sec = get_sect_in(cs_id);
	cur_sec->draw_sect_walls(srf, pl, z_buffer, this, cs_id);

}

map::~map() {

	SDL_FreeSurface(tex_smile);

	for (vec2d *vert : verts)
		delete vert;
	
	verts.clear();

	for (edge *edg : edgs)
		delete edg;

	edgs.clear();

	for (sect *sec : sects)
		delete sec;

	sects.clear();

}