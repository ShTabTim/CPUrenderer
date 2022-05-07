#include "sh_win.h"
#include <chrono>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	if (uiMsg == WM_CLOSE)
		PostQuitMessage(0);
	else
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	return 0;
}

bool sh_dwaw_win_cpu::init(HINSTANCE hInst, uint16_t dr_width, uint16_t dr_height, uint32_t scr_width, uint32_t scr_height){
	this->dr_width   = dr_width  ;
	this->dr_height  = dr_height ;
	this->scr_width  = scr_width ;
	this->scr_height = scr_height;
    WNDCLASSEX wcex;

    memset(&wcex, 0, sizeof(WNDCLASSEX));

	buf = new col[dr_width * dr_height];
	memset(buf, 0, sizeof(col) * dr_height * dr_height);

	memset(&buf_info, 0, sizeof(BITMAPINFOHEADER));

	buf_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	buf_info.bmiHeader.biPlanes = 1;
	buf_info.bmiHeader.biBitCount = 24;
	buf_info.bmiHeader.biCompression = BI_RGB;
	buf_info.bmiHeader.biWidth = dr_width;
	buf_info.bmiHeader.biHeight = dr_height;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = (LPCWSTR)(rand() + 1);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (RegisterClassEx(&wcex) == 0)
        return MessageBox(NULL, L"RegisterClassEx failed!", L"hhhh", MB_ERR_INVALID_CHARS), false;

    if ((hwnd = CreateWindow(
		wcex.lpszClassName,
		AppName.c_str(), 
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		scr_width, 
		scr_height, 
		NULL, 
		NULL, 
		hInst, 
		NULL)) == NULL)
        return MessageBox(NULL, L"CreateWindowEx failed!", L"hhhh", MB_ERR_INVALID_CHARS), false;
    return 1;
}

void sh_dwaw_win_cpu::run() {
	show(false);

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	MSG msg;

	RECT rect;

	GetClientRect(hwnd, &rect);

	HDC dc = GetDC(hwnd);

	sh_init();

	while (1) {
		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {

			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<double> dTime = tp2 - tp1;
			tp1 = tp2;
			double dt = dTime.count();

			if (!sh_loop(dt))
				break;

			wchar_t s[64];
			swprintf_s(s, 64, L"%s : %5.2f fps", AppName.c_str(), 1.0f / dt);

			SetWindowText(hwnd, s);

			RECT cRect;
			GetClientRect(hwnd, &cRect);

			scr_width = cRect.right;
			scr_height = cRect.bottom;

			if (buf) {
				if (scr_width == scr_height)
					StretchDIBits(dc, 0, 0, scr_width, scr_height, 0, 0, dr_width, dr_height, buf, &buf_info, DIB_RGB_COLORS, SRCCOPY);
				else if (scr_width > scr_height)
					StretchDIBits(dc, (scr_width - scr_height) * 0.5f, 0, scr_height, scr_height, 0, 0, dr_width, dr_height, buf, &buf_info, DIB_RGB_COLORS, SRCCOPY);
				else
					StretchDIBits(dc, 0, (scr_height - scr_width) * 0.5f, scr_width, scr_width, 0, 0, dr_width, dr_height, buf, &buf_info, DIB_RGB_COLORS, SRCCOPY);
			}
		}
	}

	sh_finit();

	if (buf) delete[] buf;
	buf = NULL;

	PostMessage(hwnd, WM_DESTROY, 0, 0);
}

HWND sh_dwaw_win_cpu::get_hwnd() const {
	return hwnd;
}

uint16_t sh_dwaw_win_cpu::get_dr_w() const { return dr_width; }
uint16_t sh_dwaw_win_cpu::get_dr_h() const { return dr_height; }
uint32_t sh_dwaw_win_cpu::get_scr_w() const { return scr_width; }
uint32_t sh_dwaw_win_cpu::get_scr_h() const { return scr_height; }

uint8_t* sh_dwaw_win_cpu::get_buf() const { return (uint8_t*)buf; }

void sh_dwaw_win_cpu::show(bool Maximized) {
    RECT dRect, wRect, cRect;

    GetWindowRect(GetDesktopWindow(), &dRect);
    GetWindowRect(hwnd, &wRect);
    GetClientRect(hwnd, &cRect);

    wRect.right += scr_width - cRect.right;
    wRect.bottom+= scr_height - cRect.bottom;

    wRect.right -= wRect.left;
    wRect.bottom -= wRect.top;

    wRect.left = (dRect.right  - wRect.right )>>1;
    wRect.top  = (dRect.bottom - wRect.bottom)>>1;

    MoveWindow(hwnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);

    ShowWindow(hwnd, Maximized ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
}