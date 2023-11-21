#include <stdio.h>
#include "screen.h"
#include "donut.h"

int main() 
{
	screen_t* screen = CreateScreen(18);

	for (;;)
	{
		Clear(screen);
		DrawDonut(screen);
		Render(screen);
	}
	return 0;
}
