#include "sect.h"

sect::sect() {

	tag = 0;
	z_fl = 0;
	z_ce = 0;

}

sect::sect(Uint32 tag, float z_fl, float z_ce, int tex_fl, int tex_ce) {

	this->tag = tag;
	this->z_fl = z_fl;
	this->z_ce = z_ce;

	this->tex_fl = tex_fl;
	this->tex_ce = tex_ce;

}

float sect::get_z_fl() {

	return z_fl;

}

float sect::get_z_ce() {

	return z_ce;

}

std::pair<inter_strct, bool> sect::get_poi_wall_lim(vec2d poi, vec2d look, edge *inter) {

	std::pair<inter_strct, bool> t(inter_strct{ -1, nullptr, vec2d(0, 0) }, false);
	float le = std::pow(10, 6);

	for (std::pair<edge*, int> *edg : sect_edgs) {

		vec2d fr = poi + look;
		edge l(&poi, &fr, 0, 0, 0, 0, 0, 0, false);

		std::pair<vec2d, bool> p = l.get_poi2d(edg->first, true, true);
		float nl = (p.first - poi).get_len();

		if (p.second && edg->first != inter && nl < le) {

			t = std::pair<inter_strct, bool>(inter_strct{edg->second, edg->first, p.first}, true);
			le = (p.first - poi).get_len();

		}

	}

	return t;

}

std::pair<inter_strct, bool> sect::get_poi_wall(vec2d poi, vec2d look, edge *inter) {

	std::pair<inter_strct, bool> t(inter_strct{-1, nullptr, vec2d(0, 0)}, false);
	float le = std::pow(10, 6);

	for (std::pair<edge*, int> *edg : sect_edgs) {
	
		vec2d fr = poi + look;
		edge l(&poi, &fr, 0, 0, 0, 0, 0, 0, false);

		std::pair<vec2d, bool> p = l.raycast(edg->first);
		float nl = (p.first - poi).get_len();

		if (p.second && edg->first != inter && nl < le) {
		
			t = std::pair<inter_strct, bool>(inter_strct{edg->second, edg->first, p.first}, true);
			le = (p.first - poi).get_len();

		}

	}

	return t;

}

std::vector<std::pair<edge*, int>*> sect::get_sect_edgs() {

	return sect_edgs;

}

bool sect::is_in_sec(vec2d p) {

	vec2d dir(-1, 0);
	Uint32 cnt = 0; // Edge intersect count

	for (std::pair<edge*, int> *edg : sect_edgs) {

		vec2d fr = p + dir;
		edge l(&p, &fr, 0, 0, 0, 0, 0, 0, false);

		std::pair<vec2d, bool> poi = l.raycast(edg->first);

		if (poi.second)
			++cnt;

	}

	return bool(cnt % 2);

}

std::pair<int, int> sect::get_pl_lim(vec2d p, vec2d look, float z) {

	vec2d pr_arr[2] = {vec2d(0, 0), vec2d(0, 0)};
	float le[2] = {0, 0};

	for (int j = 0; j < 2; ++j) {

		float nl = -1;

		for (int i = 0; i < sect_edgs.size(); ++i) {

			std::pair<edge*, int> *edg = sect_edgs[i];

			vec2d fr = p + look.get_norm() * ((j) ? -1 : 1);
			edge l(&p, &fr, 0, 0, 0, 0, 0, 0, false);

			std::pair<vec2d, bool> poi = l.raycast(edg->first);
			nl = (poi.first - p).get_len();

			if (poi.second && (nl < le[j] || nl == -1)) {
				
				pr_arr[j] = poi.first;
				le[j] = nl;

			}

		}

	}

	/*
	float y0 = (win_he - 1) * (1.f - ((cs->get_z_ce() - pl->get_pos().get_z()) / (z))) / 2.f;
	float y1 = (win_he - 1) * (1.f - ((cs->get_z_fl() - pl->get_pos().get_z()) / (z))) / 2.f;
	*/

	float nx0 = (pr_arr[0] - p).dot_pr(look.get_norm() * -1), nx1 = (pr_arr[1] - p).dot_pr(look.get_norm() * -1);
	int x0 = int(float(win_wi - 1) * (nx0 / z + 1.f) / 2.f), x1 = int(float(win_wi - 1) * (nx1 / z + 1.f) / 2.f);

	//std::cout << x0 << " " << x1 << std::endl;

	std::pair<int, int> pr;
	pr.first = x0;
	pr.second = x1;

	return pr;

}

void sect::add_edg(edge *edg, int neigh) {

	//std::cout << sect_edgs.size() << std::endl;
	sect_edgs.push_back(new std::pair<edge*, int>(edg, neigh));

}

// TODO: FIX FUNCTION (All sectors are visible)
// TODO: Try optimise so repeated verts aren't checked
bool sect::is_in_fov(vec2d ppos, vec2d look, vec2d lfov, vec2d rfov) {

	/*float x_pr_max = (*fl.get_v1() - ppos).crs_pr(*fr.get_v1() - ppos);
	int cnt = 0;

	for (int j = 0; j < 2; ++j) {

		edge *fo = (!j) ? &fl : &fr;

		for (std::pair<edge*, int> *edg : sect_edgs) {
			
			for (int i = 0; i < 2; ++i) {

				vec2d *vrt = (!i) ? edg->first->get_v0() : edg->first->get_v1();
				vec2d del = *vrt - ppos;

				float x_pr = del.crs_pr(*fl.get_v1() - ppos);

				if (x_pr < x_pr_max && x_pr >= 0)
					++cnt;

			}

		}

	}

	return (cnt < sect_edgs.size() * 2) ? true : false;*/

	for (std::pair<edge*, int> *edg : sect_edgs) {
	
		vec2d rel[2] = {*edg->first->get_v0() - ppos, *edg->first->get_v1() - ppos};

		for (int i = 0; i < 2; ++i)
			if (look.dot_pr(rel[i]) >= 0)
				return true;
	
	}

	return false;

}

plane_strct sect::get_pois_pl(edge *ed) {

	std::vector<std::pair<edge*, int>*> edgs;

	for (std::pair<edge*, int> *edg : sect_edgs)
		edgs.push_back(edg);

	vec2d verts[2];
	bool val = false;
	int cnt = 0;

	for (int j = 0; j < 2; ++j) {

		int ind = -1;
		float len = -1;

		for (int i = 0; i < edgs.size(); ++i) {

			std::pair<edge*, int> *edg = edgs[i];
			std::pair<vec2d, int> inter = ed->get_poi2d(edg->first, false, true);

			float nl = (inter.first - *(ed)->get_v0()).get_len();

			if (inter.second && (nl < len || len == -1)) {

				verts[j] = inter.first;
				val = true;
				ind = i;
				len = nl;
				++cnt;

			}

		}

		if (ind == -1)
			continue;

		edgs.erase(edgs.begin() + ind);

	}

	if (cnt < 2)
		val = false;

	edgs.clear();

	return {verts[0], verts[1], val};

}

#define max_rec (4)

// TODO: Make recursive
void sect::draw_sect_walls(SDL_Surface *srf, obj *pl, std::vector<float> *z_buffer, map *mp, int cs_id, int rec, std::vector<bool> sects_vis) {

	// First call
	if (rec == 0) {
	
		sects_vis.resize(mp->get_sects()->size());

		for (int i = 0; i < sects_vis.size(); ++i)
			sects_vis[i] = false;

	}

	if (rec >= max_rec)
		return;

	const int dotw = 10, doth = 10;

	// Display four corners of a plane
	vec2d look = vec2d(pl->get_tcos(), pl->get_tsin());
	vec2d fov_lim_l = vec2d(pl->get_tcos_l(), pl->get_tsin_l()), fov_lim_r = vec2d(pl->get_tcos_r(), pl->get_tsin_r());

	vec2d pl_pos2d = vec2d(pl->get_pos().get_x(), pl->get_pos().get_y());

	vec2d lv = pl_pos2d + fov_lim_l, rv = pl_pos2d + fov_lim_r;

	// Edges representing fov
	edge le(&pl_pos2d, &lv, -1, -1, -1, -1, -1, -1, false);
	edge re(&pl_pos2d, &rv, -1, -1, -1, -1, -1, -1, false);

	for (std::pair<edge*, int> *ed : sect_edgs) {

		// Already rendered surface

		vec2d v0 = *(ed->first->get_v0()), v1 = *(ed->first->get_v1());
		edge temp(&v0, &v1, -1, -1, -1, -1, -1, -1, false);

		// Edge behind player
		if ((v0 - pl_pos2d).dot_pr(look) <= 0 && (v1 - pl_pos2d).dot_pr(look) <= 0)
			continue;

		// Swap verts if player is facing the back side of the wall
		if ((v1 - v0).get_nrm2d().dot_pr(v0 - pl_pos2d) >= 0) {

			vec2d temp = v0;
			v0 = v1;
			v1 = temp;

		}

		// Switch verts
		if ((v0 - pl_pos2d).dot_pr(look) <= 0 || (v1 - pl_pos2d).dot_pr(look) <= 0) {

			// Clip edge
			std::pair<vec2d, bool> li = le.raycast(&temp);
			std::pair<vec2d, bool> ri = re.raycast(&temp);

			if (li.second)
				v0 = li.first;

			if (ri.second)
				v1 = ri.first;
			
			if (!li.second && !ri.second)
				continue;

		}

		// Vectors for corners (0 = Bottom left, 1 = Top left, 2 = Top right, 3 = Bottom right)
		vec3d cart_corn[4] = {
							
			vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), z_fl),\
			vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), z_ce),\
			vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), z_ce),\
			vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), z_fl)
		
		};

		vec2d cart_corn2d[4];

		for (int i = 0; i < 4; ++i)
			cart_corn2d[i] = vec2d(cart_corn[i].get_x(), cart_corn[i].get_y());

		// Pixel coordinates
		vec2d pix_corn[4];

		for (int i = 0; i < 4; ++i) {

			vec3d rel = cart_corn[i] - pl->get_pos();
			vec2d rel2d = vec2d(rel.get_x(), rel.get_y());
			vec2d rel2dnorm = rel2d.get_norm();

			float px = 0, py = 0;

			// Corner behind camera (clip it with the FOV)
			if (rel2d.dot_pr(look) < 0)
				continue;

			px = (1.f - (((rel2dnorm.crs_pr(look)) * (fov_lim_l.dot_pr(look))) / ((rel2dnorm.dot_pr(look)) * (fov_lim_l.crs_pr(look))))) / 2.f * (win_wi - 1);
			py = (1.f - rel.get_z() / (rel2d.get_len() * (rel2dnorm.dot_pr(look)))) / 2.f * (win_he - 1);
			
			//SDL_Rect bb{px - dotw / 2, py - doth / 2, dotw, doth};
			//SDL_FillRect(srf, &bb, SDL_MapRGB(srf->format, 0xFF, 0, 0xFF));

			pix_corn[i] = vec2d(px, py);

		}

		if (ed->second == -1) { // No neighbor

			int min_x = pix_corn[BOTTOM_LEFT].get_x(), max_x = pix_corn[TOP_RIGHT].get_x();

			for (int i = std::max(0, min_x); i < std::min(win_wi, max_x); ++i) {

				float ra = (i - min_x) / float(max_x - min_x);

				int cur_y0 = lerp(pix_corn[TOP_LEFT].get_y(), pix_corn[TOP_RIGHT].get_y(), ra);
				int cur_y1 = lerp(pix_corn[BOTTOM_LEFT].get_y(), pix_corn[BOTTOM_RIGHT].get_y(), ra);

				float z_min = (cart_corn2d[BOTTOM_LEFT] - pl_pos2d).get_len(), z_max = (cart_corn2d[BOTTOM_RIGHT] - pl_pos2d).get_len();

				draw_col_gbl(srf, i, cur_y0, cur_y1, 0xFFFFFFFF, z_buffer, lerp(z_min, z_max, ra));

			}

		} else { // There is a neighbour
		
			// Has not been visited and is in view
			if (!sects_vis[ed->second] && mp->get_sect_in(ed->second)->is_in_fov(pl_pos2d, look, fov_lim_l, fov_lim_r)) {

				sects_vis[cs_id] = true;

				// Recurse to render sectors in front
				mp->get_sect_in(ed->second)->draw_sect_walls(srf, pl, z_buffer, mp, ed->second, rec + 1, sects_vis);

				// Upper
				if (mp->get_sect_in(ed->second)->get_z_ce() < z_ce) {

					// Vectors for corners (0 = Bottom left, 1 = Top left, 2 = Top right, 3 = Bottom right)
					cart_corn[0] = vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), mp->get_sect_in(ed->second)->z_ce);
					cart_corn[1] = vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), z_ce);
					cart_corn[2] = vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), z_ce);
					cart_corn[3] = vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), mp->get_sect_in(ed->second)->z_ce);

					// Pixel coordinates
					for (int i = 0; i < 4; ++i) {

						vec3d rel = cart_corn[i] - pl->get_pos();
						vec2d rel2d = vec2d(rel.get_x(), rel.get_y());
						vec2d rel2dnorm = rel2d.get_norm();

						float px = 0, py = 0;

						// Corner behind camera (clip it with the FOV)
						if (rel2d.dot_pr(look) < 0)
							continue;

						px = (1.f - (((rel2dnorm.crs_pr(look)) * (fov_lim_l.dot_pr(look))) / ((rel2dnorm.dot_pr(look)) * (fov_lim_l.crs_pr(look))))) / 2.f * (win_wi - 1);
						py = (1.f - rel.get_z() / (rel2d.get_len() * (rel2dnorm.dot_pr(look)))) / 2.f * (win_he - 1);

						//SDL_Rect bb{px - dotw / 2, py - doth / 2, dotw, doth};
						//SDL_FillRect(srf, &bb, SDL_MapRGB(srf->format, 0xFF, 0, 0xFF));

						pix_corn[i] = vec2d(px, py);

					}

					int min_x = pix_corn[BOTTOM_LEFT].get_x(), max_x = pix_corn[TOP_RIGHT].get_x();

					for (int i = std::max(0, min_x); i < std::min(win_wi, max_x); ++i) {

						float ra = (i - min_x) / float(max_x - min_x);

						int cur_y0 = lerp(pix_corn[TOP_LEFT].get_y(), pix_corn[TOP_RIGHT].get_y(), ra);
						int cur_y1 = lerp(pix_corn[BOTTOM_LEFT].get_y(), pix_corn[BOTTOM_RIGHT].get_y(), ra);

						float z_min = (cart_corn2d[BOTTOM_LEFT] - pl_pos2d).get_len(), z_max = (cart_corn2d[BOTTOM_RIGHT] - pl_pos2d).get_len();

						draw_col_gbl(srf, i, cur_y0, cur_y1, 0xFFFF0000, z_buffer, lerp(z_min, z_max, ra));

					}

				}

				// Lower
				if (mp->get_sect_in(ed->second)->get_z_fl() > z_fl) {

					// Vectors for corners (0 = Bottom left, 1 = Top left, 2 = Top right, 3 = Bottom right)
					cart_corn[0] = vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), z_fl);
					cart_corn[1] = vec3d(temp.get_v0()->get_x(), temp.get_v0()->get_y(), mp->get_sect_in(ed->second)->z_fl);
					cart_corn[2] = vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), mp->get_sect_in(ed->second)->z_fl);
					cart_corn[3] = vec3d(temp.get_v1()->get_x(), temp.get_v1()->get_y(), z_fl);

					// Pixel coordinates
					for (int i = 0; i < 4; ++i) {

						vec3d rel = cart_corn[i] - pl->get_pos();
						vec2d rel2d = vec2d(rel.get_x(), rel.get_y());
						vec2d rel2dnorm = rel2d.get_norm();

						float px = 0, py = 0;

						// Corner behind camera (clip it with the FOV)
						if (rel2d.dot_pr(look) < 0)
							continue;

						px = (1.f - (((rel2dnorm.crs_pr(look)) * (fov_lim_l.dot_pr(look))) / ((rel2dnorm.dot_pr(look)) * (fov_lim_l.crs_pr(look))))) / 2.f * (win_wi - 1);
						py = (1.f - rel.get_z() / (rel2d.get_len() * (rel2dnorm.dot_pr(look)))) / 2.f * (win_he - 1);

						//SDL_Rect bb{px - dotw / 2, py - doth / 2, dotw, doth};
						//SDL_FillRect(srf, &bb, SDL_MapRGB(srf->format, 0xFF, 0, 0xFF));

						pix_corn[i] = vec2d(px, py);

					}

					int min_x = pix_corn[BOTTOM_LEFT].get_x(), max_x = pix_corn[TOP_RIGHT].get_x();

					for (int i = std::max(0, min_x); i < std::min(win_wi, max_x); ++i) {

						float ra = (i - min_x) / float(max_x - min_x);

						int cur_y0 = lerp(pix_corn[TOP_LEFT].get_y(), pix_corn[TOP_RIGHT].get_y(), ra);
						int cur_y1 = lerp(pix_corn[BOTTOM_LEFT].get_y(), pix_corn[BOTTOM_RIGHT].get_y(), ra);

						float z_min = (cart_corn2d[BOTTOM_LEFT] - pl_pos2d).get_len(), z_max = (cart_corn2d[BOTTOM_RIGHT] - pl_pos2d).get_len();

						draw_col_gbl(srf, i, cur_y0, cur_y1, 0xFF0000FF, z_buffer, lerp(z_min, z_max, ra));

					}

				}

			}

		}

	}

}

sect::~sect() {

	for (auto *el : sect_edgs)
		delete el;

	sect_edgs.clear();

}