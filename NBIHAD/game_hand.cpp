#include "game_hand.h"
#include "map.h"

game_hand::game_hand(SDL_Surface *win_srf) {

	st = st_gm;

	st = 0;
	
	esc = load_srf("../res/img/escape.png", win_srf);

	Mix_HaltMusic();

	//objs.push_back(new player(vec3d(-1.f, 0, 0)));
	mp = new map(false, win_srf);

	//FILE *fp = fopen("../res/map.gmap", "r");
	std::ifstream fp;
	fp.open("../res/map.gmap");

	std::vector<std::string> acc;
	char cur = 0;
	char prev = 0;

	enum map_st {

		st_fi, // waiting for command character
		st_co, // comment (#)
		st_te, // texture (t)
		st_ve, // vertex (v)
		st_ed, // edge (e)
		st_se, // sector (s)
		st_pl, // player (p)

	};

	int state = st_fi;

	do {

		prev = cur;
		cur = fp.get();

		if (cur == '\n' || fp.eof()) {

			if (acc.size() > 0) {

				auto ind_la = [](std::string el) -> int {

					if (el[0] == 'n')
						return -1;

					int ind = 0;

					for (int i = 0; i < 4; ++i)
						ind += std::pow(26, i) * (el[i] - 65);

					return ind;

				};

				if (state == st_te)
					add_tex(acc[0], acc[1], win_srf);
				else if (state == st_pl)
					objs.push_back(new player(vec3d(std::atof(acc[0].c_str()), std::atof(acc[1].c_str()), std::atof(acc[2].c_str())), std::atof(acc[3].c_str())));
				else if (state == st_ve)
					mp->add_vert(vec2d(std::atof(acc[0].c_str()), std::atof(acc[1].c_str())));
				else if (state == st_ed)
					mp->add_edge(ind_la(acc[0]), ind_la(acc[1]), ind_la(acc[2]), ind_la(acc[3]), ind_la(acc[4]), ind_la(acc[5]), std::atoi(acc[6].c_str()), std::atoi(acc[7].c_str()), (acc[8] == "1") ? true : false);
				else if (state == st_se) {

					// tag, z_fl, z_ce, tex_fl, tex_ce, edges (edg, neighbour)
					Uint32 tag = std::atoi(acc[0].c_str());
					float z_fl = std::atof(acc[1].c_str());
					float z_ce = std::atof(acc[2].c_str());
					int tex_fl = ind_la(acc[3]);
					int tex_ce = ind_la(acc[4]);

					acc.erase(acc.begin(), acc.begin() + 5);

					std::vector<std::pair<int, int>> edges;

					for (int i = 0; i < acc.size() / 2; ++i)
						edges.push_back(std::pair<int, int>(std::atoi(acc[2 * i].c_str()), (acc[2 * i + 1][0] == 'n') ? -1 : std::atoi(acc[2 * i + 1].c_str())));

					mp->add_sect(tag, z_fl, z_ce, tex_fl, tex_ce, edges);

				}

			}

			state = st_fi;
			acc.clear();

		} else if (cur == ' ' && prev != cur && state != st_fi && state != st_co)
			acc.push_back("");
		else if (cur == '#')
			state = st_co;
		else if (cur == 't' && state == st_fi)
			state = st_te;
		else if (cur == 'v' && state == st_fi)
			state = st_ve;
		else if (cur == 'e' && state == st_fi)
			state = st_ed;
		else if (cur == 's' && state == st_fi)
			state = st_se;
		else if (cur == 'p' && state == st_fi)
			state = st_pl;
		else
			if (acc.size() > 0)
				acc[acc.size() - 1] += cur;

	} while (!fp.eof());

	fp.close();

}

char game_hand::update(float dt) {

	const Uint8 *keys = SDL_GetKeyboardState(nullptr);

	// Playing game
	if (st == gm_pl) {

		if (keys[SDL_SCANCODE_ESCAPE]) {
		
			st = gm_ex;
			return -1;
		
		}

		for (obj *el : objs)
			el->update(dt, objs, mp);

	} else if (st == gm_qu) {
	
		
	
	} else {
	
		if (keys[SDL_SCANCODE_N]) {

			st = gm_pl;
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_CaptureMouse(SDL_TRUE);
			return -1;

		}

		if (keys[SDL_SCANCODE_Y])
			return st_ti;

	}

	return -1;

}

void game_hand::draw(SDL_Surface *win_srf, std::vector<float> *z_buffer) {

	if (st == gm_pl) {

		for (obj *el : objs)
			el->draw(win_srf, objs[ind_pl], mp, z_buffer);

	} else if (st == gm_qu) {



	} else
		SDL_BlitScaled(esc, nullptr, win_srf, nullptr);

}

game_hand::~game_hand() {

	SDL_CaptureMouse(SDL_FALSE);
	destroy_tex();

	delete mp;

	for (obj *el : objs)
		delete el;

	objs.clear();

	SDL_FreeSurface(esc);
	esc = nullptr;

}