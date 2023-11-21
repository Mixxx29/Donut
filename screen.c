
#include "screen.h"

screen_t* CreateScreen(int fontSize)
{
	screen_t* screen = (screen_t*)malloc(sizeof(screen_t*));

	// Initialize the console
	screen->hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set fullscreen
	SetConsoleDisplayMode(screen->hConsole, CONSOLE_FULLSCREEN_MODE, 0);

	// Set console font size
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.dwFontSize.X = 0;
	fontInfo.dwFontSize.Y = fontSize;
	SetCurrentConsoleFontEx(screen->hConsole, FALSE, &fontInfo);

	COORD maxSize = GetLargestConsoleWindowSize(screen->hConsole);

	// Set the console screen buffer size to the maximum possible
	SetConsoleScreenBufferSize(screen->hConsole, maxSize);

	// Set screen dimensions
	screen->nWidth = maxSize.X; // Set your desired width
	screen->nHeight = maxSize.Y; // Set your desired height

	// Allocate memory for screen data
	screen->pcData = (CHAR_INFO*)malloc(sizeof(CHAR_INFO) * screen->nWidth * screen->nHeight);
	for (int i = 0; i < screen->nWidth * screen->nHeight; i++)
	{
		screen->pcData[i].Char.UnicodeChar = L' ';
		screen->pcData[i].Attributes = FOREGROUND_INTENSITY |
			FOREGROUND_RED | FOREGROUND_GREEN; // White foreground
	}

	// Allocate memory for zBuffer
	screen->zBuffer = (float*)malloc(sizeof(float) * screen->nWidth * screen->nHeight);
	for (int i = 0; i < screen->nWidth * screen->nHeight; i++)
	{
		screen->zBuffer[i] = 0.0f;
	}

	// Enable unicode
	SetConsoleOutputCP(CP_UTF8);

	return screen;
}

void SetChar(screen_t* screen, int x, int y, WCHAR c)
{
	if (x < 0 || x >= screen->nWidth)
		return;

	if (y < 0 || y >= screen->nHeight)
		return;

	screen->pcData[y * screen->nWidth + x].Char.UnicodeChar = c;
}

void Render(screen_t* screen)
{
	COORD position = { 0, 0 };

	COORD size =
	{
		screen->nWidth,
		screen->nHeight
	};

	SMALL_RECT writeRegion =
	{
		0,
		0,
		screen->nWidth - 1,
		screen->nHeight - 1
	};

	WriteConsoleOutputW(
		screen->hConsole,
		screen->pcData,
		size, 
		position, 
		&writeRegion
	);
}

void Clear(screen_t* screen)
{
	for (int i = 0; i < screen->nWidth * screen->nHeight; i++)
	{
		screen->pcData[i].Char.UnicodeChar = L' ';
		screen->zBuffer[i] = 0;
	}
}

COORD GetScreenSize(screen_t* screen)
{
	return (COORD) {screen->nWidth, screen->nHeight};
}
