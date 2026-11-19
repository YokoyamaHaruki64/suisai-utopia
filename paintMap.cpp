#define CONIOEX
#include "conioex.h"
#include <iostream>
#include "game.h"


int g_nPaintMap[SCREEN_BOTTOM][SCREEN_RIGHT];
int g_nOldPaintMap[SCREEN_BOTTOM][SCREEN_RIGHT];

void InitPaintMap()
{
	memset(&g_nPaintMap,0,sizeof(int) * SCREEN_BOTTOM * SCREEN_RIGHT);
	memset(&g_nOldPaintMap,0,sizeof(int) * SCREEN_BOTTOM * SCREEN_RIGHT);
}

void UnInitPaintMap()
{
}

void UpdatePaintMap()
{
	memcpy(&g_nOldPaintMap, &g_nPaintMap,sizeof(int) * SCREEN_BOTTOM * SCREEN_RIGHT);
}

void DrawPaintMap()
{
	for (int y = 0; y < SCREEN_BOTTOM; y++)
	{
		for (int x = 0; x < SCREEN_RIGHT; x++)
		{
			if (g_nPaintMap[y][x] != g_nOldPaintMap[y][x])
			{
				gotoxy(x,y);
				
				switch (g_nPaintMap[y][x])
				{
				case 0:
					break;

				case 1:
					textcolor(RED);
					break;

				case 2:
					textcolor(LIGHTCYAN);
					break;

				default:
					std::cout << "era-";
					break;
				}

				std::cout << "¡";

				textcolor(WHITE);
			}
		}
	}

}

int* GetPaintMap()
{
	return &g_nPaintMap[0][0];
}