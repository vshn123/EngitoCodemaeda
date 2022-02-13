#include "title_hand.h"

title_hand::title_hand(SDL_Surface *win_srf) {

	st = st_ti;

	tit = load_srf("../res/img/tit.png", win_srf);
	str = load_srf("../res/img/start.png", win_srf);
	mus = Mix_LoadMUS("../res/snd/tit.wav");
	SDL_SetRelativeMouseMode(SDL_FALSE);

	Mix_HaltMusic();
	Mix_PlayMusic(mus, -1);

}

char title_hand::update(float dt) {

	ang += 4 * M_PI * dt; // One cycle completed every half second

	if (ang >= 2 * M_PI)
		ang -= 2 * M_PI;

	const Uint8 *keys = SDL_GetKeyboardState(nullptr);

	if (keys[SDL_SCANCODE_SPACE])
		return st_gm;

	return -1;

}

void title_hand::draw(SDL_Surface *win_srf, std::vector<float> *z_buffer) {

	// Title screen
	SDL_BlitScaled(tit, nullptr, win_srf, nullptr);

	// Start button
	int w = int(0.44f * win_wi), h = int(0.05f * win_he);
	int cx = 0.5 * win_wi, cy = 0.75 * win_he;

	SDL_Color rgb = hsv2rgb(int(0xFF * ang / (2 * M_PI)), 0xFF, 0xFF);

	SDL_Rect rect{cx - w / 2, cy - h / 2, w, h};

	for (int y = 0; y < h; ++y)
		for (int x = 0; x < w; ++x) {

			int px = int(float(x) / (w - 1) * (str->w - 1)), py = int(float(y) / (h - 1) * (str->h - 1));

			Uint8 al = 0;
			get_pix_rgba(get_pix(str, px, py), &al, nullptr, nullptr, nullptr);

			if (al >= 1)
				put_pix(win_srf, rect.x + x, rect.y + y, SDL_MapRGB(win_srf->format, rgb.r, rgb.g, rgb.b), z_buffer);

		}

}

title_hand::~title_hand() {

	Mix_FreeMusic(mus);
	mus = nullptr;

	SDL_FreeSurface(str);
	str = nullptr;

	SDL_FreeSurface(tit);
	tit = nullptr;

}