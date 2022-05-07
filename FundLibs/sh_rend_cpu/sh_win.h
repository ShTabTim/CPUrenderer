#pragma once
#include <Windows.h>
#include <cinttypes>
#include <functional>
#include"sprite.h"

typedef struct sh_dwaw_win_cpu {
private:
//draw//
	BITMAPINFO buf_info;
	typedef struct col { uint8_t b, g, r; }col;

	void dr_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b);
	void add_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b);
	void mix_in_buf(uint32_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void clip(int32_t& x, int32_t& y);
	col* buf;
	uint16_t dr_width, dr_height;
//wind//
	HWND hwnd;
	uint32_t scr_width, scr_height;
protected:
	std::wstring AppName;
//user//
	virtual bool sh_init() = 0;
	virtual bool sh_loop(double dt) = 0;
	virtual bool sh_finit() = 0;
public:
	bool init(HINSTANCE hInst, uint16_t dr_width = 128, uint16_t dr_height = 128, uint32_t scr_width = 128, uint32_t scr_height = 128);
	void run();

	HWND get_hwnd() const;

	uint16_t get_dr_w() const;
	uint16_t get_dr_h() const;
	uint32_t get_scr_w() const;
	uint32_t get_scr_h() const;

	void show(bool Maximized);

	uint8_t* get_buf() const;

	void fill_rect(int32_t x0, int32_t  y0, int32_t w1, int32_t h1, uint8_t r, uint8_t g, uint8_t b);

	void draw_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
	void add_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b);
	void mix_pix(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint8_t r, uint8_t g, uint8_t b);
	void fill_triang(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint8_t r, uint8_t g, uint8_t b);

	void draw_circ(int32_t x, int32_t y, int32_t rad, uint8_t r, uint8_t g, uint8_t b);
	void fill_circ(int32_t x, int32_t y, int32_t rad, uint8_t r, uint8_t g, uint8_t b);

	//NORMAL//
	void draw_spt(int32_t x, int32_t y, sptG* spt);
	void draw_spt(int32_t x, int32_t y, sptG* spt, float g);
	void draw_spt(int32_t x, int32_t y, int32_t w, int32_t h, sptG* spt);
	//SCALED//
	void draw_spt_scaled(int32_t x, int32_t y, float scale, sptG* spt);
	void draw_spt_offseted(int32_t x, int32_t y, int16_t o_x, int16_t o_y, sptG* spt);
	//LAMBDA//
	void draw_spt(int32_t x, int32_t y, sptG* spt, std::function<bool(uint16_t, uint16_t)> a);
	void draw_spt(int32_t x, int32_t y, sptG* spt, std::function<bool(uint16_t, uint16_t)> a, float g);
	void draw_spt_scaled(int32_t x, int32_t y, float scale, sptG* spt, std::function<bool(uint16_t, uint16_t)> a, float g);

	void draw_spt(int32_t x, int32_t y, sptRGB* spt);
	void draw_spt_offseted(int32_t x, int32_t y, int16_t o_x, int16_t o_y, sptRGB* spt);
	void draw_spt(int32_t x, int32_t y, sptRGB* spt, float r, float g, float b);
	void draw_spt(int32_t x, int32_t y, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a);
	void draw_spt_scaled(int32_t x, int32_t y, float scale, sptRGB* spt);
	void draw_spt(int32_t x, int32_t y, int32_t w, int32_t h, sptRGB* spt);
	void draw_spt_scaled(int32_t x, int32_t y, float scale, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a, float r, float g, float b);
	void draw_spt(int32_t x, int32_t y, sptRGB* spt, std::function<bool(uint16_t, uint16_t)> a, float r, float g, float b);

	void draw_spt(int32_t x, int32_t y, sptRGBA* spt);
	void draw_spt(int32_t x, int32_t y, int32_t w, int32_t h, sptRGBA* spt);
}ShWin;