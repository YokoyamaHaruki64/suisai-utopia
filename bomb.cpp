#define CONIOEX
#include "conioex.h"
#include "bomb.h"
#include "game.h"
#include "collision.h"
#include "drawWindow.h"
#include "animation.h"
#include <iostream>

//グローバル変数
BULLET g_tBomb[BOMB_MAX];

void InitBomb(void)
{
	memset(g_tBomb, 0, sizeof(BULLET) * BOMB_MAX);
	//先頭アドレスの取得（posx,posyと合わせるためにxyをそれぞれ1ずらすように調整する）
}

void UnInitBomb(void)
{
	for (int nCnt = 0; nCnt < BOMB_MAX; nCnt++)
	{
		//構造体が使用中なら更新
		if (g_tBomb[nCnt].bUse)
		{
			SetMapOffset(g_tBomb[nCnt].fPosOldX, g_tBomb[nCnt].fPosOldY, TILE_Blank);
			SetMapOffset(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY, TILE_Blank);
		}
	}
}

void UpdateBomb(void)
{
	//弾の最大数分全部回す
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		//構造体が使用中なら更新
		if (g_tBomb[nCnt].bUse)
		{
			//現在の座標を保存
			g_tBomb[nCnt].fPosOldX = g_tBomb[nCnt].fPosX;
			g_tBomb[nCnt].fPosOldY = g_tBomb[nCnt].fPosY;

			//座標の更新
			g_tBomb[nCnt].fPosX += g_tBomb[nCnt].fSpeedX;
			g_tBomb[nCnt].fPosX += g_tBomb[nCnt].fSpeedX;

			//Y座標制御
			g_tBomb[nCnt].fSpeedY += VALUE_GRAVITY;
			g_tBomb[nCnt].fPosY += g_tBomb[nCnt].fSpeedY;

			//現在の座標が画面外なら使用中フラグをオフに
			if (g_tBomb[nCnt].fPosX < SCREEN_LEFT)
			{
				g_tBomb[nCnt].fPosX = SCREEN_LEFT;
				BombCollisionRoutine(nCnt);
			}
			if (g_tBomb[nCnt].fPosX > MAP_X - 2)
			{
				g_tBomb[nCnt].fPosX = MAP_X - 2;
				BombCollisionRoutine(nCnt);
			}
			if (g_tBomb[nCnt].fPosY < SCREEN_TOP + 1)
			{
				g_tBomb[nCnt].fPosY = SCREEN_TOP + 1;
				BombCollisionRoutine(nCnt);
			}
			if (g_tBomb[nCnt].fPosY > SCREEN_BOTTOM - 1)
			{
				g_tBomb[nCnt].fPosY = SCREEN_BOTTOM - 1;
				BombCollisionRoutine(nCnt);
			}

			if (BulletCollisionField(&g_tBomb[nCnt]))
			{
				BombCollisionRoutine(nCnt);
			}
			if (BulletCollisionCeiling(&g_tBomb[nCnt]))
			{
				BombCollisionRoutine(nCnt);
			}
			if (BulletCollisionRight(&g_tBomb[nCnt]))
			{
				BombCollisionRoutine(nCnt);
			}
			if (BulletCollisionLeft(&g_tBomb[nCnt]))
			{
				BombCollisionRoutine(nCnt);
			}
		}
	}
}

void DrawBomb(void)
{
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (g_tBomb[nCnt].bUse)
		{

			if ((g_tBomb[nCnt].fPosX != g_tBomb[nCnt].fPosOldX) ||
				(g_tBomb[nCnt].fPosY != g_tBomb[nCnt].fPosOldY))
			{
				//ポインタを二次元配列のように扱う
				SetMapOffset(g_tBomb[nCnt].fPosOldX, g_tBomb[nCnt].fPosOldY, TILE_Blank);
				SetMapOffset(g_tBomb[nCnt].fPosOldX + 1, g_tBomb[nCnt].fPosOldY, TILE_Blank);
			}

			SetMapOffset(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY, TILE_PlayerBomb);
			SetMapOffset(g_tBomb[nCnt].fPosX + 1, g_tBomb[nCnt].fPosY, TILE_Zenkaku);
		}

		////ヒット地点に数f表示
		//if (g_tBomb[nCnt].bLastHit)
		//{

		//	if (g_tBomb[nCnt].nLastHitCnt == 1)
		//	{
		//		SetMapOffset(g_tBomb[nCnt].fPosOldX, g_tBomb[nCnt].fPosOldY, TILE_Blank);
		//		SetMapOffset(g_tBomb[nCnt].fPosOldX + 1, g_tBomb[nCnt].fPosOldY, TILE_Blank);
		//		SetMapOffset(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY, TILE_PlayerBomb);
		//		SetMapOffset(g_tBomb[nCnt].fPosX + 1, g_tBomb[nCnt].fPosY, TILE_Zenkaku);
		//	}
		//	else
		//	{//消去
		//		SetMapOffset(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY, TILE_Blank);
		//		SetMapOffset(g_tBomb[nCnt].fPosX + 1, g_tBomb[nCnt].fPosY, TILE_Blank);
		//	}
		//}
	}

}

void CreateBomb(float fX, float fY, float fMx, float fMy)
{
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		if (!g_tBomb[nCnt].bUse && !g_tBomb[nCnt].bLastHit)
		{
			g_tBomb[nCnt].bUse = true;
			g_tBomb[nCnt].fPosOldX = g_tBomb[nCnt].fPosX = fX;
			g_tBomb[nCnt].fPosOldY = g_tBomb[nCnt].fPosY = fY;
			g_tBomb[nCnt].fSpeedX = fMx * BULLET_SPEED_X;
			g_tBomb[nCnt].fSpeedY = fMy * BULLET_SPEED_Y;

			g_tBomb[nCnt].bLastHit = false;
			g_tBomb[nCnt].nLastHitCnt = 0;
			break;
		}
	}

}

void BombCollisionRoutine(int nCnt)
{
	g_tBomb[nCnt].bUse = false;
	CreateAnimation(EXPLOSION_FRAME,g_tBomb[nCnt].fPosX,g_tBomb[nCnt].fPosY,A_PcolorExpl);
	ExplosionPaint(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY,TILE_PlayerColor);
	ExplosionCollision(g_tBomb[nCnt].fPosX, g_tBomb[nCnt].fPosY,PLAYERSIDE);
}

BULLET* GetBomb()
{
	return g_tBomb;
}
