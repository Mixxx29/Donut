#pragma once

#include <Windows.h>

typedef struct screen 
{
	HANDLE hConsole;
	int nWidth;
	int nHeight;
	CHAR_INFO* pcData;
	float *zBuffer;
} screen_t;

screen_t* CreateScreen(int fontSize);
void SetChar(screen_t* screen, int x, int y, WCHAR c);
void Render(screen_t* screen);
void Clear(screen_t* screen);
COORD GetScreenSize(screen_t* screen);