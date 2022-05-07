#include "sh_win.h"

void sh_dwaw_win_cpu::dr_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b) { buf[index].r = r; buf[index].g = g; buf[index].b = b; }
void sh_dwaw_win_cpu::add_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b) { buf[index].r += r; buf[index].g += g; buf[index].b += b; }
void sh_dwaw_win_cpu::mix_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t a) { buf[index].r = ((255 - a) * buf[index].r + r * a) * 0.00392156f; buf[index].g = ((255 - a) * buf[index].g + g * a) * 0.00392156f; buf[index].b = ((255 - a) * buf[index].g + g * a) * 0.00392156f; }
void sh_dwaw_win_cpu::clip(int32_t& x, int32_t& y) {
	x = (x < 0) ? 0 : ((x > w) ? w : x);
	y = (y < 0) ? 0 : ((y > h) ? h : y);
}

uint8_t* sh_dwaw_win_cpu::get_buf() { return (uint8_t*)buf; }
uint16_t sh_dwaw_win_cpu::get_w() { return w; }
uint16_t sh_dwaw_win_cpu::get_h() { return h; }

void sh_dwaw_win_cpu::fill_rect(int32_t x0, int32_t  y0, int32_t x1, int32_t y1, uint8_t r, uint8_t g, uint8_t b) {
	clip(x0, y0); clip(x1, y1);
	for (uint32_t x(x1 - x0); x--;)
		for (uint32_t y(y1 - y0); y--;)
			draw_pix(x0 + x, y0 + y, r, g, b);
}

void sh_dwaw_win_cpu::draw_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) { if (x >= 0 && x < w && y >= 0 && y < h) dr_in_buf(y * w + x, r, g, b); }
void sh_dwaw_win_cpu::add_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b) { if (x >= 0 && x < w && y >= 0 && y < h) add_in_buf(y * w + x, r, g, b); }
void sh_dwaw_win_cpu::mix_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) { if (x >= 0 && x < w && y >= 0 && y < h) mix_in_buf(y * w + x, r, g, b, a); }
void sh_dwaw_win_cpu::draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t r, uint8_t g, uint8_t b) {
	int32_t x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x1 - x0; dy = y1 - y0;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1) {
		if (dx >= 0) {
			x = x0;
			y = y0;
			xe = x1;
		}
		else {
			x = x1;
			y = y1;
			xe = x0;
		}

		draw_pix(x, y, r, g, b);

		for (i = 0; x < xe; i++) {
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			draw_pix(x, y, r, g, b);
		}
	}
	else {
		if (dy >= 0) {
			x = x0;
			y = y0;
			ye = y1;
		}
		else {
			x = x1;
			y = y1;
			ye = y0;
		}

		draw_pix(x, y, r, g, b);

		for (i = 0; y < ye; i++) {
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			draw_pix(x, y, r, g, b);
		}
	}
}

void sh_dwaw_win_cpu::fill_triang(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, uint8_t r, uint8_t g, uint8_t b) {
	auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) draw_pix(i, ny, r, g, b); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
									 // Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}

void sh_dwaw_win_cpu::draw_circ(int32_t x0, int32_t y0, int32_t rad, uint8_t r, uint8_t g, uint8_t b) {
	if (!rad) return;
	int32_t x = 0;
	int32_t y = rad;
	int32_t p = 3 - 2 * rad;

	while (y >= x) { // only formulate 1/8 of circle
		draw_pix(x0 - x, y0 - y, r, g, b);//upper left left
		draw_pix(x0 - y, y0 - x, r, g, b);//upper upper left
		draw_pix(x0 + y, y0 - x, r, g, b);//upper upper right
		draw_pix(x0 + x, y0 - y, r, g, b);//upper right right
		draw_pix(x0 - x, y0 + y, r, g, b);//lower left left
		draw_pix(x0 - y, y0 + x, r, g, b);//lower lower left
		draw_pix(x0 + y, y0 + x, r, g, b);//lower lower right
		draw_pix(x0 + x, y0 + y, r, g, b);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}
void sh_dwaw_win_cpu::fill_circ(int32_t x0, int32_t y0, int32_t rad, uint8_t r, uint8_t g, uint8_t b) {
	int32_t x = 0;
	int32_t y = rad;
	int32_t p = 3 - 2 * rad;
	if (!r) return;

	auto drawline = [&](int sx, int ex, int ny) {
		for (int32_t i(sx); i <= ex; i++)
			draw_pix(i, ny, r, g, b);
	};

	while (y >= x) {
		drawline(x0 - x, x0 + x, y0 - y);
		drawline(x0 - y, x0 + y, y0 - x);
		drawline(x0 - x, x0 + x, y0 + y);
		drawline(x0 - y, x0 + y, y0 + x);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}