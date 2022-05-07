#include <Windows.h>
#include "FundLibs/sh_rend_cpu/sh_win.h"

class MainRenderer : public sh_dwaw_win_cpu {
    bool sh_init() {
        AppName = L"CPURenderer";
        return 1;
    }
    bool sh_loop(double dt) {
        static float r = 0;
        r += dt;
        fill_rect(0, 0, get_dr_w(), get_dr_h(), 30, 57, 48);

        fill_circ(200+40.0f*sin(r), 200 + 40.0f * cos(r), 100 + 100.0f * sin(r), 200, 20, 240);

        fill_circ(50, 50, 30, 20, 200, 240);
        fill_circ(50, 100, 30, 20, 200, 20);
        fill_circ(100, 50, 30, 200, 200, 240);
        fill_circ(50, 50, 10, 20, 20, 240);

        return 1;
    }
    bool sh_finit() {
        return 1;
    }
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR sCmdLine, int iShow) {
	MainRenderer simulation;
	if (simulation.init(hInst, 256, 256, 512, 512))
		simulation.run();
	return 0;
}