#include "ac_hand.h"

ac_hand::ac_hand(SDL_Surface *win_srf) {

	st = st_ac;

	ang = .0f;
	y_off = .0f;
	x_off = .0f;

	rev_cnt = .0f;
	rev_wait = rand_int(8000, 10000);
	rev = false;

	chis_tim = .0f;
	chis_wait = rand_int(5000, 7500);
	chis_bool = 0;

	cursed = load_srf("../res/img/cursed.png", win_srf);
	warn = load_srf("../res/img/warn.png", win_srf);
	chis[0] = load_srf("../res/img/chis.png", win_srf);
	chis[1] = load_srf("../res/img/chi2.png", win_srf);
	chis[2] = load_srf("../res/img/chik.png", win_srf);
	chis[3] = load_srf("../res/img/stco.png", win_srf);
	chis[4] = load_srf("../res/img/clas.png", win_srf);
	chis[5] = load_srf("../res/img/naeg.png", win_srf);

	mus = Mix_LoadMUS("../res/snd/cursed.mp3");

	Mix_HaltMusic();
	Mix_PlayMusic(mus, -1);

	chi = rand_int(0, chi_size - 1);

}

char ac_hand::update(float dt) {

	rev_cnt += dt;
	chis_tim += dt;

	// Make into class, abstraction damn you
	if (rev_cnt >= rev_wait / 1000.f) {

		rev_cnt -= rev_wait / 1000.f;
		rev ^= 1;
		rev_wait = rand_int((rev) ? 1000 : 8000, (rev) ? 3000 : 10000);

	}

	if (chis_tim >= chis_wait / 1000.f) {

		chis_tim -= chis_wait / 1000.f;
		chis_bool ^= 1;
		chis_wait = rand_int((chis_bool) ? 500 : 5000, (chis_bool) ? 1500 : 7500);
		chi = rand_int(0, chi_size - 1);

	}

	//std::cout << (int)chis_bool << std::endl;

	ang += M_PI * dt * std::pow(-3, int(rev));
	y_off += ((M_PI / 4) / (2 * M_PI) * (img_h - 1)) * dt * std::pow(-3, int(rev));
	x_off += 25.f * dt * std::pow(-3, int(rev));

	return -1;

}

void ac_hand::draw(SDL_Surface *srf, std::vector<float> *z_buffer) {

	for (int y = 0; y < img_h; ++y) {

		float z = std::abs(std::cos(/*((1 + std::sin(ang)) * 180.f*/(66.f * M_PI / 180.f) * std::abs((win_he / 2.f - float(y)) / (win_he / 2.f))));
		int rr = rand_int(5, 9);

		for (int x = 0; x < win_wi; ++x) {

			int px = int((img_w - 1) * (x - win_wi / 2.f + (win_wi / (z * 2.f))) / (win_wi / (z))) % img_w;
			int prx = int(px + x_off + rr) % img_w, pry = int(y + y_off) % img_h; // projected coordinates
			
			Uint32 col = (get_pix(cursed, prx, pry)) & 0xFFFF0000;
			
			if ((((get_pix(warn, x, y) & 0xFF00) >> 8) < 0x7F) && !chis_bool)
				put_pix(srf, x, y, col, z_buffer);
			else if (chis_bool && (((get_pix(warn, x, y) & 0xFF00) >> 8) < 0x7F))
				put_pix(srf, x, y, get_pix(chis[chi], (prx + rand_int(25, 30)) % img_w, (pry + rand_int(25, 30)) % img_h) & 0xFFFF0000, z_buffer);
				//put_pix(srf, x, y, SDL_MapRGB(srf->format, float(0xFF) * serp(((get_pix(srf, x, y) & 0xFFFF0000) >> 16) & 0xFF, (((get_pix(chis, prx, pry) & 0xFFFF0000)) >> 16) & 0xFF, 1.f - 1.f / (1.f + (((chis_wait / 1000.f) - chis_tim) * rand_int(5, 10) / 10.f * 25000.f / (chis_wait)))/*(chis_tim * 1000.f / (chis_wait)) */ /*rand_int(5, 7) / 10.f*/), 0, 0));
			else /*if(((get_pix(warn, x, y) & 0xFF00) >> 8) > 0x7F)*/
				put_pix(srf, x, y, get_pix(warn, x, y), z_buffer);

		}

	}

}

ac_hand::~ac_hand() {

	Mix_FreeMusic(mus);
	mus = nullptr;

	for (int i = 0; i < chi_size; ++i) {

		SDL_FreeSurface(chis[i]);
		chis[i] = nullptr;

	}

	SDL_FreeSurface(cursed);
	cursed = nullptr;

	SDL_FreeSurface(warn);
	warn = nullptr;

}