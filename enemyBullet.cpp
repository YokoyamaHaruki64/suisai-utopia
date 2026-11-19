#define CONIOEX
#include "conioex.h"
#include "enemyBullet.h"
#include "collision.h"
#include "game.h"
#include "enemy.h"
#include "drawWindow.h"
#include "animation.h"
#include <iostream>


//グローバル変数
ENEMYBULLET g_tEnemyBullets[ENEMY_MAX][ENEMYBULLET_MAX];
bool* g_pbUseBullet;
MAPTILE* g_pnE_BulletMap;

void InitEnemyBullet(void)
{
	memset(&g_tEnemyBullets[0][0], 0, sizeof(ENEMYBULLET) * ENEMY_MAX * ENEMYBULLET_MAX);
	g_pbUseBullet = GetUsedE_Bullet();
	g_pnE_BulletMap = GetMap();
}

void UnInitEnemyBullet(void)
{
	//弾の最大数分全部回す
	for (int nEnemy = 0; nEnemy < ENEMY_MAX; nEnemy++)
	{
		//使ってる配列の情報取ってきてfor文の回数削減を目指す
		for (int nBullet = 0; nBullet < ENEMYBULLET_MAX; nBullet++)
		{
			//構造体が使用中なら更新
			if (g_tEnemyBullets[nEnemy][nBullet].bUse)
			{
				SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
				SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY,TILE_Blank);
			}
		}
	}

}

void UpdateEnemyBullet(void)
{
	//弾の最大数分全部回す
	for (int nEnemy = 0; nEnemy < ENEMY_MAX; nEnemy++)
	{
		if (g_pbUseBullet[nEnemy])
		{
			//使ってる配列の情報取ってきてfor文の回数削減を目指す
			for (int nBullet = 0; nBullet < ENEMYBULLET_MAX; nBullet++)
			{
				//構造体が使用中なら更新
				if (g_tEnemyBullets[nEnemy][nBullet].bUse)
				{
					switch (g_tEnemyBullets[nEnemy][nBullet].nType)
					{
					case NORMAL:
						NormalBulletUpdate(nEnemy, nBullet);
						break;

					case EXPLOSION:
						ExplosionBulletUpdate(nEnemy,nBullet);
						break;

					default:
						break;
					}
					
				}
			}
		}
	}
}

void DrawEnemyBullet(void)
{
	
	for (int nEnemy = 0; nEnemy < ENEMY_MAX; nEnemy++)
	{
		for (int nBullet = 0; nBullet < ENEMYBULLET_MAX; nBullet++)
		{
			if (g_tEnemyBullets[nEnemy][nBullet].bUse)
			{
				//前の弾を消す
				if ((g_tEnemyBullets[nEnemy][nBullet].fPosX != g_tEnemyBullets[nEnemy][nBullet].fPosOldX) ||
					(g_tEnemyBullets[nEnemy][nBullet].fPosY != g_tEnemyBullets[nEnemy][nBullet].fPosOldY))
				{
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
				}

				switch (g_tEnemyBullets[nEnemy][nBullet].nType)
				{
				case NORMAL:
					//SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyBullet);
					//SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Zenkaku);
					break;

				case EXPLOSION:
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyBomb);
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Zenkaku);
					break;

				default:
					break;
				}

			}

			//ヒット地点に数f表示
			if (g_tEnemyBullets[nEnemy][nBullet].bLastHit)
			{

				if (g_tEnemyBullets[nEnemy][nBullet].nLastHitCnt == 1)
				{
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);

					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyBullet);
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Zenkaku);
				}
				else
				{//消去
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Blank);
					SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Blank);
				}
			}

		}
	}
}

void CreateEnemyBullet(float fX, float fY, float fMx, float fMy,E_BULLETTYPE nType,int nArrayNum)
{
	for (int nCnt = 0; nCnt < ENEMYBULLET_MAX; nCnt++)
	{
		if (!g_tEnemyBullets[nArrayNum][nCnt].bUse && !g_tEnemyBullets[nArrayNum][nCnt].bLastHit)
		{
			g_tEnemyBullets[nArrayNum][nCnt].bUse = true;
			g_tEnemyBullets[nArrayNum][nCnt].fPosOldX = g_tEnemyBullets[nArrayNum][nCnt].fPosX = fX;
			g_tEnemyBullets[nArrayNum][nCnt].fPosOldY = g_tEnemyBullets[nArrayNum][nCnt].fPosY = fY;
			g_tEnemyBullets[nArrayNum][nCnt].fSpeedX = fMx * ENEMYBULLET_SPEED_X;
			g_tEnemyBullets[nArrayNum][nCnt].fSpeedY = fMy * ENEMYBULLET_SPEED_Y;
			g_tEnemyBullets[nArrayNum][nCnt].nType = nType;
			break;
		}
	}

}



ENEMYBULLET* GetEnemyBullet()
{
	return &g_tEnemyBullets[0][0];
}

void CollisionRoutine(int nEnemy,int nBullet)
{
	g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
	SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosOldX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosOldY, TILE_Blank);
	SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Blank);
	SetMapOffset(g_tEnemyBullets[nEnemy][nBullet].fPosX + 1, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_Blank);
}

void RemoveEnemyBullets(int nEnemy)
{
	for (int nCnt = 0; nCnt < ENEMYBULLET_MAX; nCnt++)
	{
		if (g_tEnemyBullets[nEnemy][nCnt].bUse)
		{
			CollisionRoutine(nEnemy,nCnt);
		}
	}
}

void NormalBulletUpdate(int nEnemy, int nBullet)
{
	//現在の座標を保存
	g_tEnemyBullets[nEnemy][nBullet].fPosOldX = g_tEnemyBullets[nEnemy][nBullet].fPosX;
	g_tEnemyBullets[nEnemy][nBullet].fPosOldY = g_tEnemyBullets[nEnemy][nBullet].fPosY;

	//座標の更新
	g_tEnemyBullets[nEnemy][nBullet].fPosX += g_tEnemyBullets[nEnemy][nBullet].fSpeedX;
	g_tEnemyBullets[nEnemy][nBullet].fPosX += g_tEnemyBullets[nEnemy][nBullet].fSpeedX;

	//Y座標制御
	g_tEnemyBullets[nEnemy][nBullet].fPosY += g_tEnemyBullets[nEnemy][nBullet].fSpeedY;

	//現在の座標が画面外なら使用中フラグをオフに
	if (g_tEnemyBullets[nEnemy][nBullet].fPosX < SCREEN_LEFT)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosX = SCREEN_LEFT;
		CollisionRoutine(nEnemy, nBullet);
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosX > MAP_X - 2)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosX = MAP_X - 2;
		CollisionRoutine(nEnemy, nBullet);
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosY < SCREEN_TOP + 1)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosY = SCREEN_TOP + 1;
		CollisionRoutine(nEnemy, nBullet);
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosY > SCREEN_BOTTOM - 1)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosY = SCREEN_BOTTOM - 1;
		CollisionRoutine(nEnemy, nBullet);
	}

	//地形との当たり判定
	if (EnemyBulletCollisionField(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		CollisionRoutine(nEnemy, nBullet);
	}
	if (EnemyBulletCollisionCeiling(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		CollisionRoutine(nEnemy, nBullet);
	}
	if (EnemyBulletCollisionLeft(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		CollisionRoutine(nEnemy, nBullet);
	}
	if (EnemyBulletCollisionRight(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		CollisionRoutine(nEnemy, nBullet);
	}

	//if (g_tEnemyBullets[nEnemy][nBullet].bLastHit)
	//{
	//	g_tEnemyBullets[nEnemy][nBullet].nLastHitCnt++;

	//	if (g_tEnemyBullets[nEnemy][nBullet].nLastHitCnt > 3)
	//	{
	//		g_tEnemyBullets[nEnemy][nBullet].bLastHit = false;
	//		g_tEnemyBullets[nEnemy][nBullet].nLastHitCnt = 0;
	//	}
	//}
}

void ExplosionBulletUpdate(int nEnemy,int nBullet)
{
	//現在の座標を保存
	g_tEnemyBullets[nEnemy][nBullet].fPosOldX = g_tEnemyBullets[nEnemy][nBullet].fPosX;
	g_tEnemyBullets[nEnemy][nBullet].fPosOldY = g_tEnemyBullets[nEnemy][nBullet].fPosY;

	//座標の更新
	g_tEnemyBullets[nEnemy][nBullet].fPosX += g_tEnemyBullets[nEnemy][nBullet].fSpeedX;
	g_tEnemyBullets[nEnemy][nBullet].fPosX += g_tEnemyBullets[nEnemy][nBullet].fSpeedX;

	//Y座標制御
	g_tEnemyBullets[nEnemy][nBullet].fSpeedY += VALUE_GRAVITY;
	g_tEnemyBullets[nEnemy][nBullet].fPosY += g_tEnemyBullets[nEnemy][nBullet].fSpeedY;

	//現在の座標が画面外なら使用中フラグをオフに
	if (g_tEnemyBullets[nEnemy][nBullet].fPosX < SCREEN_LEFT)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosX = SCREEN_LEFT;
		ExplosionPaint(g_tEnemyBullets[nEnemy][nBullet].fPosX,g_tEnemyBullets[nEnemy][nBullet].fPosY,TILE_EnemyColor);
		CreateAnimation(EXPLOSION_FRAME, g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, A_EcolorExpl);
		ExplosionCollision(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, ENEMYSIDE);
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosX > MAP_X - 2)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosX = MAP_X - 2;
		ExplosionPaint(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyColor);
		CreateAnimation(EXPLOSION_FRAME, g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, A_EcolorExpl);
		ExplosionCollision(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, ENEMYSIDE);
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosY < SCREEN_TOP + 1)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosY = SCREEN_TOP + 1;
		ExplosionPaint(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyColor);
		CreateAnimation(EXPLOSION_FRAME, g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, A_EcolorExpl);
		ExplosionCollision(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, ENEMYSIDE);
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (g_tEnemyBullets[nEnemy][nBullet].fPosY > SCREEN_BOTTOM - 1)
	{
		g_tEnemyBullets[nEnemy][nBullet].fPosY = SCREEN_BOTTOM - 1;
		ExplosionPaint(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, TILE_EnemyColor);
		CreateAnimation(EXPLOSION_FRAME, g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY,A_EcolorExpl);
		ExplosionCollision(g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY, ENEMYSIDE);
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}

	//地形との当たり判定
	if (EnemyBulletCollisionField(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (EnemyBulletCollisionCeiling(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (EnemyBulletCollisionLeft(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}
	if (EnemyBulletCollisionRight(&g_tEnemyBullets[nEnemy][nBullet]))
	{
		g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	}

}

void EB_Explosion(int nEnemy,int nBullet)
{
	g_tEnemyBullets[nEnemy][nBullet].bUse = false;
	CreateAnimation(EXPLOSION_FRAME, g_tEnemyBullets[nEnemy][nBullet].fPosX, g_tEnemyBullets[nEnemy][nBullet].fPosY,A_EcolorExpl);
}