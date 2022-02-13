#pragma once
// std includes
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstdarg>

// SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

// Random
#define rand_int(a, b) (rand() % (std::max((a), (b)) - std::min((a), (b)) + 1) + std::min((a), (b)))
#define lerp(a, b, p) (float(a) + float(p) * (float(b) - float(a))) // Linear interpolation
#define serp(a, b, p) (lerp(float(a), float(b), std::sin(float(p) * M_PI / 2.f))) // Sinusodal interpolation

// Defines
// Window Dimensions
#define win_wi (640)
#define win_he (480)

static int cur_win_wi = win_wi;
static int cur_win_he = win_he;

// Title
#define win_tit "Nagito's Basics in Hope And Despair | Frame time : "

// Acceleration due to gravity
#define g_acc (9.8)

// FOV
#define fov (90.f * M_PI / 180.f)

// Load surface
static SDL_Surface *load_srf(std::string fn, SDL_Surface *win_srf) {

	SDL_Surface *srf = IMG_Load(fn.c_str());

	if (srf == nullptr) {

		std::cout << "ERROR: FAILED TO LOAD \"" << fn << "\"" << std::endl;
		return nullptr;

	}

	SDL_Surface *opt = SDL_ConvertSurface(srf, win_srf->format, 0);
	
	SDL_FreeSurface(srf);
	srf = nullptr;

	return opt;

}

// Subroutines for accessing an SDL_Surface's pixels
static Uint32 get_pix(SDL_Surface *srf, int x, int y) {

	//Uint8 *p = (Uint8*)srf->pixels + y * srf->pitch + x * 32;
	//return *(Uint32*)p;
	if (srf == nullptr)
		return 0;

	if (x < 0 || x >= srf->w || y < 0 || y >= srf->h)
		return 0;
	
	Uint32 *p = (Uint32*)srf->pixels;
	p += y * srf->w + x;
	return *p;

}

static void get_pix_rgba(Uint32 col, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) {

	if (a != nullptr)
		*a = (col & 0xFF000000) >> 24;

	if (r != nullptr)
		*r = (col & 0xFF0000) >> 16;

	if (g != nullptr)
		*g = (col & 0xFF00) >> 8;

	if (b != nullptr)
		*b = col & 0xFF;

}

// Subroutines for accessing an SDL_Surface's pixels
static void put_pix(SDL_Surface *srf, int x, int y, Uint32 col, std::vector<float> *z_buffer, float z=-1.f) {

	if (x < 0 || x >= srf->w || y < 0 || y >= srf->h)
		return;

	if (z <= (*z_buffer)[y * win_wi + x] || (*z_buffer)[y * win_wi + x] < 0) {

		Uint32 *p = (Uint32*)srf->pixels;
		p += y * srf->w + x;
		*p = col;

		(*z_buffer)[y * win_wi + x] = z;

	}

}

static void clear_z_buf(std::vector<float> *z_buffer) {
	
	for (int i = 0; i < z_buffer->size(); ++i)
		(*z_buffer)[i] = 100000000;

}

static void draw_col_gbl(SDL_Surface *srf, int x, int y0, int y1, Uint32 col, std::vector<float> *z_buffer, float z=-1) {

	int temp = y0;
	y0 = std::min(y0, y1);
	y1 = std::max(temp, y1);

	for (int y = std::max(0, y0); y <= std::min(y1, win_he - 1); ++y)
		put_pix(srf, x, y, col, z_buffer, z);

}

// Convert RGB to HSV
static SDL_Color hsv2rgb(int h, int s, int v) {

	SDL_Color rgb;
	unsigned char region, remainder, p, q, t;

	if (s == 0) {

		rgb.r = v;
		rgb.g = v;
		rgb.b = v;
	
		return rgb;
	
	}

	region = h / 43;
	remainder = (h - (region * 43)) * 6;

	p = (v * (255 - s)) >> 8;
	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
	case 0:
		rgb.r = v; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = v; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = v; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = v;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = v;
		break;
	default:
		rgb.r = v; rgb.g = p; rgb.b = q;
		break;
	}

	return rgb;

}

#define tex_vec_size (456976)

// Consists of four letter codes
//inline static SDL_Surface *tex_vec[tex_vec_size];

static void add_tex(std::string nm, std::string fn, SDL_Surface *srf) {

	//tex_smile = IMG_Load("..\\res\\img\\test.png");

	/*int ind = 0;

	for (int i = 0; i < 4; ++i)
		ind += std::pow(26, i) * (nm[i] - 65);

	std::cout << nm << " " << fn << std::endl;

	tex_vec[ind] = load_srf(fn, srf);*/

}

static SDL_Surface *get_tex(int ind) {

	/*if (ind < 0 && ind >= tex_vec_size)
		return nullptr;

	return tex_vec[ind];*/

}

static void destroy_tex() {

	/*for (int i = 0; i < tex_vec_size; ++i)
		SDL_FreeSurface(tex_vec[i]);

	SDL_FreeSurface(tex_smile);*/

}