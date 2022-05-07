#include "sh_win.h"
#include "sprite.h"
//////SPRITES//////

////GRAY////

//NORMAL//
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptG* spt) {
	uint8_t g;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;) {
			g = *(spt->get_buf() + j * spt->get_w() + i);
			draw_pix(x + i, y + j, g, g, g);
		}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptG* spt, float go) {
	uint8_t g;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;) {
			g = *(spt->get_buf() + j * spt->get_w() + i) * go;
			draw_pix(x + i, y + j, g, g, g);
		}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, int32_t w, int32_t h, sptG* spt) {
	float aw = ((float)spt->get_w()) / ((float)w);
	float ah = ((float)spt->get_h()) / ((float)h);
	uint8_t* l;
	for (uint16_t i(w); i--;)
		for (uint16_t j(h); j--;) {
			l = spt->get_buf() + spt->get_w() * ((uint32_t)(i * aw)) + (uint32_t)(j * ah);
			draw_pix(x + i, y + j, *(l++), *(l++), *l);
		}
}
//SCALED//
void sh_dwaw_win_cpu::draw_spt_scaled(int32_t x, int32_t y, float scale, sptG* spt) {}
void sh_dwaw_win_cpu::draw_spt_offseted(int32_t x, int32_t y, int16_t o_x, int16_t o_y, sptG* spt) {}
//LAMBDA//
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptG* spt, std::function<bool(uint16_t, uint16_t)> a) {}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptG* spt, std::function<bool(uint16_t, uint16_t)> a, float g) {}
void sh_dwaw_win_cpu::draw_spt_scaled(int32_t x, int32_t y, float scale, sptG* spt, std::function<bool(uint16_t, uint16_t)> a, float g) {}


//RGB//
int32_t scaled(int32_t x, int32_t y, float inv_y) {
	return (x - y * (int32_t)(x * inv_y - 1)) % y;
}

void sh_dwaw_win_cpu::draw_spt_offseted(int32_t x, int32_t y, int16_t o_x, int16_t o_y, sptRGB* spt) {
	//if (!spt) return;
	uint32_t g;
	float inv_w = 1.0f / spt->get_w();
	float inv_h = 1.0f / spt->get_h();
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;) {
			g = (scaled(j + o_y, spt->get_h(), inv_h) * spt->get_w() + scaled(i + o_x, spt->get_w(), inv_w)) * 3;
			draw_pix(x + i, y + j, (spt->get_buf())[g], (spt->get_buf())[g + 1], (spt->get_buf())[g + 2]);
		}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptRGB* spt) {
	//if (!spt) return;
	uint32_t g;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;) {
			g = (j * spt->get_w() + i) * 3;
			draw_pix(x + i, y + j, (spt->get_buf())[g], (spt->get_buf())[g + 1], (spt->get_buf())[g + 2]);
		}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptRGB* spt, float r, float g, float b) {
	//if (!spt) return;
	uint32_t l;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;) {
			l = (j * spt->get_w() + i) * 3;
			draw_pix(x + i, y + j, (spt->get_buf())[l] * r, (spt->get_buf())[l + 1] * g, (spt->get_buf())[l + 2] * b);
		}

}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a) {
	//if (!spt) return;
	uint32_t g;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;)
			if (a(i, j)) {
				g = (j * spt->get_w() + i) * 3;
				draw_pix(x + i, y + j, (spt->get_buf())[g], (spt->get_buf())[g + 1], (spt->get_buf())[g + 2]);
			}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a, float r, float g, float b) {
	//if (!spt) return;
	uint32_t l;
	for (uint16_t i(spt->get_w()); i--;)
		for (uint16_t j(spt->get_h()); j--;)
			if (a(i, j)) {
				l = (j * spt->get_w() + i) * 3;
				draw_pix(x + i, y + j, (spt->get_buf())[l] * r, (spt->get_buf())[l + 1] * g, (spt->get_buf())[l + 2] * b);
			}
}
void sh_dwaw_win_cpu::draw_spt_scaled(int32_t x, int32_t y, float scale, sptRGB* spt) {
	//if (!spt || scale <= 0) return;
	uint32_t l, w = spt->get_w() * scale, h = spt->get_h() * scale;
	scale = 1.0f / scale;
	for (uint16_t i(w); i--;)
		for (uint16_t j(h); j--;) {
			l = 3 * (uint32_t)(spt->get_w() * ((uint32_t)(j * scale)) + i * scale);
			draw_pix(x + i, y + j,
				(spt->get_buf())[l],
				(spt->get_buf())[l + 1],
				(spt->get_buf())[l + 2]
			);
		}
}
void sh_dwaw_win_cpu::draw_spt_scaled(int32_t x, int32_t y, float scale, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a, float r, float g, float b) {
	//if (!spt || scale <= 0) return;
	uint32_t l;
	if ((scale > 0.9999f) && (scale < 1.0001f)) {
		for (uint16_t i(spt->get_w()); i--;)
			for (uint16_t j(spt->get_h()); j--;)
				if (a(i, j)) {
					l = 3 * (spt->get_w() * j + i);
					draw_pix(x + i, y + j,
						(spt->get_buf())[l] * r,
						(spt->get_buf())[l + 1] * g,
						(spt->get_buf())[l + 2] * b);
				}
	}
	else if (scale < 1) {
		uint16_t w = spt->get_w() * scale, h = spt->get_h() * scale;
		scale = 1.0f / scale;
		for (uint16_t i(w); i--;)
			for (uint16_t j(h); j--;)
				if (a(i * scale, j * scale)) {
					l = 3 * (uint32_t)(spt->get_w() * ((uint32_t)(j * scale)) + i * scale);
					draw_pix(x + i, y + j,
						(spt->get_buf())[l] * r,
						(spt->get_buf())[l + 1] * g,
						(spt->get_buf())[l + 2] * b);
				}
	}
	else {
		uint16_t w = spt->get_w() * scale, h = spt->get_h() * scale;
		for (uint16_t i(spt->get_w()); i--;)
			for (uint16_t j(spt->get_h()); j--;)
				if (a(i, j)) {
					l = 3 * (spt->get_w() * j + i);
					fill_rect(
						x + i * scale, y + j * scale,
						x + (i + 1) * scale, y + (j + 1) * scale,
						(spt->get_buf())[l] * r,
						(spt->get_buf())[l + 1] * g,
						(spt->get_buf())[l + 2] * b);
				}
	}
}
void sh_dwaw_win_cpu::draw_spt(int32_t x, int32_t y, int32_t w, int32_t h, sptRGB* spt) {
	float aw = ((float)spt->get_w()) / ((float)w);
	float ah = ((float)spt->get_h()) / ((float)h);
	uint32_t l;
	for (uint16_t i(w); i--;)
		for (uint16_t j(h); j--;) {
			l = (spt->get_w() * ((uint32_t)(i * aw)) + j * ah) * 3;
			draw_pix(x + i, y + j, (spt->get_buf())[l], (spt->get_buf())[l + 1], (spt->get_buf())[l + 2]);
		}
}