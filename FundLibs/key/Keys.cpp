#include "Keys.h"

int16_t m_keyOldState[256] = { 0 };
int16_t m_keyNewState[256] = { 0 };
k_stat m_keys[256], m_mouse[5];
POINT pt = { 0, 0 };
POINT ptOld = { 0, 0 };
POINT START = { 0, 0 };

bool m_mouseOldState[5] = { 0 };
bool m_mouseNewState[5] = { 0 };

void get_cur_pos(POINT& pptt) {
	GetCursorPos(&pptt);
	pptt.x = pptt.x - START.x;
	pptt.y = -pptt.y - START.y;
}

void key_loop(HWND hwnd){
	{
		RECT wRect, cRect;
		GetWindowRect(hwnd, &wRect);
		GetClientRect(hwnd, &cRect);
		START.x = 0.5f * (wRect.right + wRect.left - cRect.right);
		START.y = -wRect.bottom + 0.15f * (wRect.bottom - wRect.top - cRect.bottom);
		for (int i = 0; i < 256; i++) {
			m_keyNewState[i] = GetAsyncKeyState(i);

			m_keys[i].pressed = false;
			m_keys[i].released = false;

			if (m_keyNewState[i] != m_keyOldState[i])
				if (m_keyNewState[i] & 0x8000) {
					m_keys[i].pressed = !m_keys[i].held;
					m_keys[i].held = true;
				}
				else {
					m_keys[i].released = true;
					m_keys[i].held = false;
				}

			m_keyOldState[i] = m_keyNewState[i];
		}
		for (int m = 0; m < 5; m++) {
			m_mouse[m].pressed = false;
			m_mouse[m].released = false;

			if (m_mouseNewState[m] != m_mouseOldState[m])
				if (m_mouseNewState[m]) {
					m_mouse[m].pressed = true;
					m_mouse[m].held = true;
				}
				else {
					m_mouse[m].released = true;
					m_mouse[m].held = false;
				}
			m_mouseOldState[m] = m_mouseNewState[m];
		}
		ptOld = pt;
		get_cur_pos(pt);
	}
}
k_stat get_key(uint32_t id) { return m_keys[id]; }
k_stat get_mouse(uint32_t id) { return m_mouse[id]; }
void set_cursor(uint32_t x, uint32_t y){
	SetCursorPos(x + START.x, -(int32_t)(y + START.y));
	get_cur_pos(pt);
}
int32_t get_x() { return pt.x; }
int32_t get_y() { return pt.y; }
int32_t getdx() { return pt.x - ptOld.x; }
int32_t getdy() { return pt.y - ptOld.y; }