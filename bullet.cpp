#define CONIOEX
#include "conioex.h"
#include "bullet.h"
#include "game.h"
#include "collision.h"
#include "drawWindow.h"
#include <iostream>

//グローバル変数
BULLET g_tBullets[BULLET_MAX];

void InitBullet(void)
{
	memset(g_tBullets, 0, sizeof(BULLET) * BULLET_MAX);
	//先頭アドレスの取得（posx,posyと合わせるためにxyをそれぞれ1ずらすように調整する）
}

void UnInitBullet(void)
{
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		//構造体が使用中なら更新
		if (g_tBullets[nCnt].bUse)
		{
			SetMapOffset(g_tBullets[nCnt].fPosOldX, g_tBullets[nCnt].fPosOldY,TILE_Blank);
			SetMapOffset(g_tBullets[nCnt].fPosX, g_tBullets[nCnt].fPosY,TILE_Blank);
		}
	}
}

void UpdateBullet(void)
{
	//弾の最大数分全部回す
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		//構造体が使用中なら更新
		if (g_tBullets[nCnt].bUse)
		{
			//現在の座標を保存
			g_tBullets[nCnt].fPosOldX = g_tBullets[nCnt].fPosX;
			g_tBullets[nCnt].fPosOldY = g_tBullets[nCnt].fPosY;

			//座標の更新
			g_tBullets[nCnt].fPosX += g_tBullets[nCnt].fSpeedX;
			g_tBullets[nCnt].fPosX += g_tBullets[nCnt].fSpeedX;

			//Y座標制御
			g_tBullets[nCnt].fSpeedY += VALUE_GRAVITY;
			g_tBullets[nCnt].fPosY += g_tBullets[nCnt].fSpeedY;

			//現在の座標が画面外なら使用中フラグをオフに
			if (g_tBullets[nCnt].fPosX < SCREEN_LEFT)
			{
				g_tBullets[nCnt].fPosX = SCREEN_LEFT;
				CollisionRoutine(nCnt);
			}
			if (g_tBullets[nCnt].fPosX > MAP_X - 2)
			{
				g_tBullets[nCnt].fPosX = MAP_X - 2;
				CollisionRoutine(nCnt);
			}
			if (g_tBullets[nCnt].fPosY < SCREEN_TOP + 1)
			{
				g_tBullets[nCnt].fPosY = SCREEN_TOP + 1;
				CollisionRoutine(nCnt);
			}
			if(g_tBullets[nCnt].fPosY > SCREEN_BOTTOM - 1)
			{
				g_tBullets[nCnt].fPosY = SCREEN_BOTTOM - 1;
				CollisionRoutine(nCnt);
			}

			if (BulletCollisionField(&g_tBullets[nCnt]))
			{
				CollisionRoutine(nCnt);
			}
			if (BulletCollisionCeiling(&g_tBullets[nCnt]))
			{
				CollisionRoutine(nCnt);
			}
			if (BulletCollisionRight(&g_tBullets[nCnt]))
			{
				CollisionRoutine(nCnt);
			}
			if (BulletCollisionLeft(&g_tBullets[nCnt]))
			{
				CollisionRoutine(nCnt);
			}
		}

		if (g_tBullets[nCnt].bLastHit)
		{
			g_tBullets[nCnt].nLastHitCnt++;

			if (g_tBullets[nCnt].nLastHitCnt > 3)
			{
				g_tBullets[nCnt].bLastHit = false;
				g_tBullets[nCnt].nLastHitCnt = 0;
			}
		}
	}
}

void DrawBullet(void)
{
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (g_tBullets[nCnt].bUse)
		{

			if ((g_tBullets[nCnt].fPosX != g_tBullets[nCnt].fPosOldX) ||
				(g_tBullets[nCnt].fPosY != g_tBullets[nCnt].fPosOldY))
			{
				//ポインタを二次元配列のように扱う
				SetMapOffset(g_tBullets[nCnt].fPosOldX, g_tBullets[nCnt].fPosOldY,TILE_Blank);
				SetMapOffset(g_tBullets[nCnt].fPosOldX + 1, g_tBullets[nCnt].fPosOldY,TILE_Blank);
			}

			SetMapOffset(g_tBullets[nCnt].fPosX, g_tBullets[nCnt].fPosY, TILE_PlayerBullet);
			SetMapOffset(g_tBullets[nCnt].fPosX + 1, g_tBullets[nCnt].fPosY, TILE_Zenkaku);
		}

		//ヒット地点に数f表示
		if (g_tBullets[nCnt].bLastHit)
		{
			
			if (g_tBullets[nCnt].nLastHitCnt == 1)
			{
				SetMapOffset(g_tBullets[nCnt].fPosOldX, g_tBullets[nCnt].fPosOldY, TILE_Blank);
				SetMapOffset(g_tBullets[nCnt].fPosOldX + 1, g_tBullets[nCnt].fPosOldY, TILE_Blank);
				SetMapOffset(g_tBullets[nCnt].fPosX, g_tBullets[nCnt].fPosY, TILE_PlayerBullet);
				SetMapOffset(g_tBullets[nCnt].fPosX + 1, g_tBullets[nCnt].fPosY, TILE_Zenkaku);
			}
			else
			{//消去
				SetMapOffset(g_tBullets[nCnt].fPosX, g_tBullets[nCnt].fPosY, TILE_Blank);
				SetMapOffset(g_tBullets[nCnt].fPosX + 1, g_tBullets[nCnt].fPosY, TILE_Blank);
			}
		}
	}

}

void CreateBullet(float fX, float fY, float fMx, float fMy)
{
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (!g_tBullets[nCnt].bUse && !g_tBullets[nCnt].bLastHit)
		{
			g_tBullets[nCnt].bUse = true;
			g_tBullets[nCnt].fPosOldX = g_tBullets[nCnt].fPosX = fX;
			g_tBullets[nCnt].fPosOldY = g_tBullets[nCnt].fPosY = fY;
			g_tBullets[nCnt].fSpeedX = fMx * BULLET_SPEED_X;
			g_tBullets[nCnt].fSpeedY = fMy * BULLET_SPEED_Y;

			g_tBullets[nCnt].bLastHit = false;
			g_tBullets[nCnt].nLastHitCnt = 0;
			break;
		}
	}

}

void CollisionRoutine(int nCnt)
{
	g_tBullets[nCnt].bUse = false;
	g_tBullets[nCnt].bLastHit = true;
}

BULLET* GetBullet()
{
	return g_tBullets;
}
