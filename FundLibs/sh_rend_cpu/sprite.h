#pragma once
#include <cstdio>
#include <string>

template <uint8_t depth>
struct sprite {
protected:
	uint8_t* buf = 0;
	uint16_t width;
	uint16_t height;
public:
	sprite(uint16_t l = 0) { width = l; height = l; }
	void init(uint16_t width, uint16_t height) {
		this->width = width;
		this->height = height;
		buf = new uint8_t[width * height * depth];
	}

	void finit() { if (buf) delete[] buf; buf = NULL; }

	uint8_t* get_buf() const { return buf; }
	uint16_t get_w() const { return width; }
	uint16_t get_h() const { return height; }
	uint8_t get_d() const { return depth; }

	bool save_to_file(std::wstring sFile) {
		FILE* f = NULL;

		_wfopen_s(&f, sFile.c_str(), L"wb");
		if (!f) return 0;

		fwrite(&depth, sizeof(uint8_t), 1, f);
		fwrite(&width, sizeof(uint16_t), 1, f);
		fwrite(&height, sizeof(uint16_t), 1, f);
		fwrite(buf, sizeof(uint8_t), width * height * depth, f);;

		fclose(f);

		return 1;
	}

	bool load_from_file(std::wstring sFile) {
		if (buf) delete[] buf;

		FILE* f = NULL;

		_wfopen_s(&f, sFile.c_str(), L"rb");
		if (!f) return 0;

		uint8_t d;
		fread(&d, sizeof(uint8_t), 1, f);
		if (d != depth) {
			fclose(f);
			return 0;
		}

		fread(&width, sizeof(uint16_t), 1, f);
		fread(&height, sizeof(uint16_t), 1, f);
		buf = new uint8_t[width * height * depth];
		fread(buf, sizeof(uint8_t), width * height * depth, f);

		fclose(f);
		return true;
	}
	~sprite() { finit(); }
};

typedef struct sprite<1> sptG;
typedef struct sprite<3> sptRGB;
typedef struct sprite<4> sptRGBA;