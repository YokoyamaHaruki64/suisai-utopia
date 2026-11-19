#pragma once
#include "color.h"

struct VECTOR2
{
	float x;
	float y;
};

VECTOR2 AngleToVector2(float fAngle);
VECTOR2 Vec2_Normarize(VECTOR2 vec);
VECTOR2 RandomVector();
float Vec2_Magnitude(VECTOR2 vec);

int ClampInt(int nMin, int nMax, int nNum);
int StringToInt(char* str);
void ReadCSV(int nSize_x, int nSize_y, int* pnArray, const char* sFileName);
void ReadCSV_Anim(int nSize_x, int nSize_y, int nFrame, int* pnArray, const char* sFileName);
int RandomRange(int min, int MAX);
int _1d2();//1‚©2‚ª•Ô‚Á‚Ä‚­‚é
float Normarize0to1(float min, float max, float num);

bool Blinking(int nCount);