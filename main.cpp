#include <Windows.h>
#include "FundLibs/ShRendCPU/ShWin.h"

#define ScrWW 512
#define ScrHH 512

class MainRenderer : public ShWinRendCPU {
    bool initSim() {
        AppName = L"CPU3dRenderer";
        return true;
    }
    bool loopSim(float dt) {
        static float r = 0;
        r += dt;
        db.Fill(0, 0, db.GetW(), db.GetH(), { 30, 57, 48 });

        db.FillCircle(200+40.0f*sin(r), 200 + 40.0f * cos(r), 200, { 200, 20, 240 });

        db.FillCircle(50, 50, 30, { 20, 200, 240 });
        db.FillCircle(50, 100, 30, { 20, 200, 20 });
        db.FillCircle(100, 50, 30, { 200, 200, 240 });
        db.FillCircle(50, 50, 10, { 20, 20, 240 });

        return true;
    }
    bool finitSim() {
        return true;
    }
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
    if(uiMsg == WM_CLOSE)
        PostQuitMessage(0);
    else
        return DefWindowProc(hWnd, uiMsg, wParam, lParam);
    return 0;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR sCmdLine, int iShow) {
	MainRenderer simulation;
	if (simulation.init(hInst, ScrWW, ScrHH, WndProc))
		simulation.Start();
	return 0;
}