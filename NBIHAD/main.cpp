// TODO: Implement loading all data from file

#include "global.h"
#include "vec.h"
#include "edge.h"
#include "obj.h"
#include "handle.h"

#define TARGET_FPS (60.f)

int main(int argc, char **argv) {

	std::vector<float> *z_buffer = new std::vector<float>();

	for (int i = 0; i < win_wi * win_he; ++i)
		z_buffer->push_back(-1.f);

	srand(time(nullptr));

	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_MID);

	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 640);

	SDL_Window *win = SDL_CreateWindow(win_tit, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, cur_win_wi, cur_win_he, SDL_WINDOW_SHOWN);
	//SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);

	//SDL_Surface *win_srf = SDL_GetWindowSurface(win); // Window surface, stores raster (Software render)
	SDL_Surface *win_srf = SDL_CreateRGBSurface(0, win_wi, win_he, 32, 0xFF0000, 0xFF00, 0xFF, 0xFF000000);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	edge::load(win_srf);

	bool quit = false;
	SDL_Event e;

	float acc = .0f;

	Uint32 prev = SDL_GetTicks(), cur = SDL_GetTicks();
	float dt = .0f; // Delta time

	//std::cout << z_buffer->size() << std::endl;

	handle::init(win, win_srf);

	bool full = false;

	while (!quit) {
		
		/*if (handle::get_cur_type() == st_gm)
			SDL_WarpMouseInWindow(win, win_wi / 2, win_he / 2);*/

		// Clear z-buffer
		for (int i = 0; i < z_buffer->size(); ++i)
			(*z_buffer)[i] = -1.f;

		prev = cur;
		cur = SDL_GetTicks();
		dt = (cur - prev) / 1000.f;

		acc += dt;

		if (acc >= 1.f) {
		
			std::string tit = win_tit + std::to_string(dt * 1000.f) + "ms";
			SDL_SetWindowTitle(win, tit.c_str());
			acc -= 1.f;
		
		}

		while (SDL_PollEvent(&e)) {
		
			if (e.type == SDL_QUIT)
				quit = true;

			if (e.type == SDL_KEYUP)
				if (e.key.keysym.sym == SDLK_F5 && handle::get_cur_type() != st_ac) {

					cur_win_wi = (full) ? 1920 : 640;
					cur_win_he = (full) ? 1080 : 480;

					//SDL_SetWindowSize(win, cur_win_wi, cur_win_he);
					SDL_SetWindowFullscreen(win, (int)(full & SDL_WINDOW_FULLSCREEN_DESKTOP));

					full = !full;

				}

		}

		// Update stuff
		handle::update_hnds(dt, win_srf);

		SDL_FillRect(win_srf, nullptr, 0);

		// Draw stuff
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 0xFF);
		SDL_RenderClear(ren);

		handle::draw_hnds(win_srf, z_buffer);

		// I dare you not to vomit while looking at this garbage
		SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, win_srf);

		SDL_RenderCopy(ren, tex, nullptr, nullptr);

		SDL_DestroyTexture(tex);
		tex = nullptr;

		SDL_RenderPresent(ren);

	}

	z_buffer->clear();
	delete z_buffer;

	edge::destroy();
	handle::destroy();

	destroy_tex();

	SDL_DestroyRenderer(ren);
	ren = nullptr;

	SDL_FreeSurface(win_srf);
	win_srf = nullptr;

	SDL_DestroyWindow(win);
	win = nullptr;

	Mix_HaltMusic();
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;

}