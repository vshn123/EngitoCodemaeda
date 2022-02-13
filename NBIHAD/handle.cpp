#include "handle.h"
#include "title_hand.h"
#include "game_hand.h"
#include "ac_hand.h"

handle::handle() {

	

}

char handle::get_type() {

	return st;

}

char handle::update(float dt) {

	return -1;

}

void handle::draw(SDL_Surface *win_srf, std::vector<float> *z_buffer) {

	

}

handle::~handle() {

	

}

/*struct hash {

	Uint32 bl[4];

};

#define no_fi (3)
std::pair<std::string, hash> tab[no_fi] = {

	std::pair<std::string, hash>("../res/map.gmap", {0xC66C05EC, 0xADAAD1F4, 0x411142A9, 0x618AE5A1}),
	
};

static hash md5_fi(std::string fn) {

	std::vector<Uint8> bytes;

	// Load bytes
	FILE *fp = fopen(fn.c_str(), "rb");
	
	if (fp == nullptr)
		return {0, 0, 0, 0};
		
	Uint8 cur;

	do {
	
		cur = fgetc(fp);

		if (feof(fp))
			break;

		bytes.push_back(cur);
	
	} while (!feof(fp));

	fclose(fp);
	fp = nullptr;

	Uint32 s[64] = {

		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
		5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
		4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
		6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21

	};
	
	Uint32 K[64] = {
	
		0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
		0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
		0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
		0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
		0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
		0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
		0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
		0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
		0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
		0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
		0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
		0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
		0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
		0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
		0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391

	};

	Uint32 a0 = 0x67452301;
	Uint32 b0 = 0xefcdab89;
	Uint32 c0 = 0x98badcfe;
	Uint32 d0 = 0x10325476;

	Uint64 og_len = bytes.size() * 8;

	bytes.push_back(0x80000000);

	while ((bytes.size() * 8) % 512 != 448)
		bytes.push_back(0);

	og_len = og_len % 0xFFFFFFFFFFFFFFFF;

	for (int j = 7; j >= 0; --j)
		bytes.push_back((og_len & (0xFF00000000000000 >> ((7 - j) * 8))) >> (j * 8));

	for (int j = 0; j < bytes.size(); j += 64) {

		Uint32 A = a0;
		Uint32 B = b0;
		Uint32 C = c0;
		Uint32 D = d0;

		Uint32 M[16];

		// Break up data into chunks
		for (int k = 0; k < 16; k += 4)
			M[k / 4] = bytes[j + k] | (bytes[j + k + 1] << 8) | (bytes[j + k + 2] << 16) | (bytes[j + k + 3] << 24);

		for (Uint32 i = 0; i < 64; ++i) {
		
			Uint32 F = 0, g = 0;
		
			if (i < 16) {

				F = (B & C) | ((~B) & D);
				g = i;

			} else if (i < 32) {
			
				F = (D & B) | ((~D) & C);
				g = (5 * i + 1) % 16;
			
			} else if (i < 48) {
			
				F = B ^ C ^ D;
				g = (3 * i + 5) % 16;
			
			} else {
			
				F = C ^ (B | (~D));
				g = (7 * i) % 16;
			
			}

			F = F + A + K[i] + M[g];  // M[g] must be a 32-bits block
			A = D;
			D = C;
			C = B;
			B = B + (F << s[i]) | (F >> (32 - s[i]));

		}
	
		a0 = a0 + A;
		b0 = b0 + B;
		c0 = c0 + C;
		d0 = d0 + D;

	}

	bytes.clear();

	hash out = {a0, b0, c0, d0};

	return out;

}*/

void handle::init(SDL_Window *win, SDL_Surface *win_srf) {

	//std::cout << z_buffer->size() << std::endl;

	// TODO: Write file integrity checks
	bool im = true;

	if (im) {
	
		SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN);
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_Delay(3000);

		hnd = new ac_hand(win_srf);

	} else
		hnd = new title_hand(win_srf);

}

char handle::get_cur_type() {

	return hnd->get_type();

}

void handle::update_hnds(float dt, SDL_Surface *win_srf) {

	int r_st = hnd->update(dt);

	if (r_st != -1) {
		
		delete hnd;

		switch (r_st) {
		case st_ti:
			hnd = new title_hand(win_srf);
			break;
		case st_ac:
			hnd = new ac_hand(win_srf);
			break;
		case st_gm:
			hnd = new game_hand(win_srf);
			break;
		default:
			hnd = new title_hand(win_srf);
			break;
		}
	
	}

}

void handle::draw_hnds(SDL_Surface *win_srf, std::vector<float> *z_buffer) {

	hnd->draw(win_srf, z_buffer);

}

void handle::destroy() {
		
	delete hnd;
	hnd = nullptr;

}