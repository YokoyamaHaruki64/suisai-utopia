#pragma once
#include "bullet.h"

#define BOMB_MAX (32)
#define BOMB_SPEED_X	(0.4) 
#define BOMB_SPEED_Y	(0.4) 

//プロトタイプ宣言
void InitBomb(void);
void UnInitBomb(void);
void UpdateBomb(void);
void DrawBomb(void);
void CreateBomb(float fX, float fY, float fMx, float fMy);
void BombCollisionRoutine(int nCnt);

BULLET* GetBomb();