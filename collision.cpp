#define CONIOEX
#include "conioex.h"
#include "collision.h"
#include "paintMap.h"
#include "game.h"
#include "animation.h"
#include "enemySpawn.h"
#include <iostream>

#define RIGHT	1
#define LEFT	0

MAPTILE* g_pnColMap;
void InitCollision()
{
	g_pnColMap = GetColorMap();
}

void UnInitCollision()
{
}

//敵キャラとプレイヤーの当たり判定
//敵から呼び出す関数
//戻り値　true hit
bool EnemyCollisionBullet(ENEMY* ptEnemy)
{
	//弾の配列のポインタ―取得
	BULLET* tBullet = GetBullet();

	//敵の判定用の四角形を作成
	float fEnemyLeft = ptEnemy->fPosX - CHARACTER_COLLISION_SIZEX;
	float fEnemyRight = ptEnemy->fPosX + ptEnemy->fSizeX - CHARACTER_COLLISION_SIZEX;
	float fEnemyUp = ptEnemy->fPosY - CHARACTER_COLLISION_SIZEY;
	float fEnemyDown = ptEnemy->fPosY + ptEnemy->fSizeY - CHARACTER_COLLISION_SIZEY;

	//全部の弾とのチェック
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		//構造体が使用中か？
		if (tBullet[nCnt].bUse == true)
		{//弾の判定用の四角形を作成
			float fBulletLeft = tBullet[nCnt].fPosX - CHARACTER_COLLISION_SIZEX;
			float fBulletRight = tBullet[nCnt].fPosX + CHARACTER_COLLISION_SIZEX;
			float fBulletUp = tBullet[nCnt].fPosY - CHARACTER_COLLISION_SIZEY;
			float fBulletDown = tBullet[nCnt].fPosY + CHARACTER_COLLISION_SIZEY;

			//当たり判定
			if (fEnemyLeft > fBulletRight) 
				continue;
			if (fEnemyRight < fBulletLeft) 
				continue;
			if (fEnemyUp > fBulletDown) 
				continue;
			if (fEnemyDown < fBulletUp) 
				continue;

			//敵と弾が当たっている

			return true;
		}
	}
	//どの弾とも当たっていない
	return false;
}

bool PlayerCollisionEnemy(PLAYER* ptPlayer)
{
	//敵の配列のポインタ―取得
	ENEMY* tEnemy = GetEnemy();

	//キャラクターの判定用の四角形を作成
	float fPlayerLeft = ptPlayer->fPosX - CHARACTER_COLLISION_SIZEX;
	float fPlayerRight = ptPlayer->fPosX + ptPlayer->fSizeX - CHARACTER_COLLISION_SIZEX;
	float fPlayerUp = ptPlayer->fPosY - CHARACTER_COLLISION_SIZEY;
	float fPlayerDown = ptPlayer->fPosY + ptPlayer->fSizeY  - CHARACTER_COLLISION_SIZEY ;

	//全部の敵とのチェック
	for (int nCnt = 0; nCnt < ENEMY1_MAX; nCnt++)
	{
		//構造体が使用中か？
		if (tEnemy[nCnt].bUse)
		{//敵の判定用の四角形を作成
			float fEnemyLeft = tEnemy[nCnt].fPosX - CHARACTER_COLLISION_SIZEX;
			float fEnemyRight = tEnemy[nCnt].fPosX + tEnemy[nCnt].fSizeX - CHARACTER_COLLISION_SIZEX;
			float fEnemyUp = tEnemy[nCnt].fPosY - CHARACTER_COLLISION_SIZEY;
			float fEnemyDown = tEnemy[nCnt].fPosY + tEnemy[nCnt].fSizeY - CHARACTER_COLLISION_SIZEY;

			//当たり判定
			if (fPlayerLeft > fEnemyRight) 
				continue;
			if (fPlayerRight < fEnemyLeft) 
				continue;
			if (fPlayerUp > fEnemyDown) 
				continue;
			if (fPlayerDown < fEnemyUp) 
				continue;

			//プレイヤーと敵が当たっている
			return true;
		}
	}
	//どの敵とも当たっていない
	return false;
}

bool SpawnerCollisionBullet(SPAWNER* ptSpawner)
{
	BULLET* tBullet = GetBullet();

	//矩形作成
	float fSpawnerLeft = ptSpawner->pos.x - CHARACTER_COLLISION_SIZEX;
	float fSpawnerRight = ptSpawner->pos.x + SPAWNER_SIZEX - CHARACTER_COLLISION_SIZEX;
	float fSpawnerUp = ptSpawner->pos.y - CHARACTER_COLLISION_SIZEY;
	float fSpawnerDown = ptSpawner->pos.y + SPAWNER_SIZEY - CHARACTER_COLLISION_SIZEY;

	//
	for (int nCnt = 0; nCnt < BULLET_MAX; nCnt++)
	{
		//構造体が使用中か？
		if (tBullet[nCnt].bUse == true)
		{//弾の判定用の四角形を作成
			float fBulletLeft = tBullet[nCnt].fPosX - CHARACTER_COLLISION_SIZEX;
			float fBulletRight = tBullet[nCnt].fPosX + CHARACTER_COLLISION_SIZEX;
			float fBulletUp = tBullet[nCnt].fPosY - CHARACTER_COLLISION_SIZEY;
			float fBulletDown = tBullet[nCnt].fPosY + CHARACTER_COLLISION_SIZEY;

			//当たり判定
			if (fSpawnerLeft > fBulletRight)
				continue;
			if (fSpawnerRight < fBulletLeft)
				continue;
			if (fSpawnerUp > fBulletDown)
				continue;
			if (fSpawnerDown < fBulletUp)
				continue;

			//敵と弾が当たっている
			tBullet[nCnt].bUse = false;
			tBullet[nCnt].bLastHit = true;
			return true;
		}
	}
	//どの弾とも当たっていない
	return false;


}

bool PlayerCollisionField(PLAYER* ptPlayer)
{
	//if文は矩形作成に近い,2マス以上で対象と平行な線を引ける場合、始点*2と終点*2で計４つの分岐が必要になる
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++,ptField++)
	{
		if (ptPlayer->fPosX > (ptField->fPosX) &&
			ptPlayer->fPosX < (ptField->fPosX + ptField->fWidth))
		{
			if (ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) > (ptField->fPosY - 1.0f) &&
				ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) < (ptField->fPosY + 0.5f))
			{
				ptPlayer->fPosY = ptField->fPosY - ptPlayer->fSizeY;
				return true;
			}
		}
	}
	return false;
}

bool PlayerCollisionRight(PLAYER* ptPlayer)
{
	//if文は矩形作成に近い,2マス以上で対象と平行な線を引ける場合、始点*2と終点*2で計４つの分岐が必要になる
	FIELD* ptField = GetField();
	int nCharBite = 2;
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++,ptField++)
	{
		if (ptPlayer->fPosX > (ptField->fPosX - nCharBite) &&
			ptPlayer->fPosX < (ptField->fPosX + ptField->fWidth / 2))
		{
			if ((ptPlayer->fPosY > (ptField->fPosY + 0.1f) &&
				ptPlayer->fPosY < (ptField->fPosY + ptField->fHight)) ||
				(ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) > (ptField->fPosY + 0.1f) &&
				ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) < (ptField->fPosY + ptField->fHight)))
			{
				ptPlayer->fPosX = ptField->fPosX - nCharBite;
				return true;
			}
		}
	}
	return false;
}

bool PlayerCollisionLeft(PLAYER* ptPlayer)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++,ptField++)
	{
		if (ptPlayer->fPosX > (ptField->fPosX + ptField->fWidth / 2) &&
			ptPlayer->fPosX < (ptField->fPosX + ptField->fWidth + 0.1f))
		{
			if ((ptPlayer->fPosY > (ptField->fPosY + 0.1f) &&
				ptPlayer->fPosY < (ptField->fPosY + ptField->fHight)) ||
			   (ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) > (ptField->fPosY + 0.1f) &&
				ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) < (ptField->fPosY + ptField->fHight)))
			{
				ptPlayer->fPosX = ptField->fPosX + ptField->fWidth + 0.99f;
				return true;
			}
		}
	}
	return false;
}

bool PlayerCollisionCeiling(PLAYER* ptPlayer)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++,ptField++)
	{
		if (ptPlayer->fPosX > (ptField->fPosX) &&
			ptPlayer->fPosX < (ptField->fPosX + ptField->fWidth) - 0.92f)
		{
			if (ptPlayer->fPosY > (ptField->fPosY + 1.0) &&
				ptPlayer->fPosY < (ptField->fPosY + ptField->fHight) + 0.5f)
			{
				//ptPlayer->fPosY = ptField->fPosY + ptField->fHight + 0.5f;
				return true;
			}
		}
	}
	return false;
}

bool PlayerCollisionSpawnerF(PLAYER* ptPlayer)
{
	//if文は矩形作成に近い,2マス以上で対象と平行な線を引ける場合、始点*2と終点*2で計４つの分岐が必要になる
	SPAWNER* ptSpawner = GetSpawner();
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++,ptSpawner++)
	{
		if (ptSpawner->bUse)
		{
			if (ptPlayer->fPosX > (ptSpawner->pos.x) &&
				ptPlayer->fPosX < (ptSpawner->pos.x + SPAWNER_SIZEX))
			{
				if (ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) > (ptSpawner->pos.y - 1.0f) &&
					ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) < (ptSpawner->pos.y + 0.5f))
				{
					ptPlayer->fPosY = ptSpawner->pos.y - ptPlayer->fSizeY;
					return true;
				}
			}
		}
	}
	return false;
}

bool PlayerCollisionSpawnerR(PLAYER* ptPlayer)
{
	//if文は矩形作成に近い,2マス以上で対象と平行な線を引ける場合、始点*2と終点*2で計４つの分岐が必要になる
	SPAWNER* ptSpawner = GetSpawner();
	int nCharBite = 2;
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++, ptSpawner++)
	{
		if (ptSpawner->bUse)
		{
			if (ptPlayer->fPosX > (ptSpawner->pos.x - nCharBite) &&
				ptPlayer->fPosX < (ptSpawner->pos.x + SPAWNER_SIZEX / 2))
			{
				if ((ptPlayer->fPosY > (ptSpawner->pos.y + 0.1f) &&
					ptPlayer->fPosY < (ptSpawner->pos.y + SPAWNER_SIZEY)) ||
					(ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) > (ptSpawner->pos.y + 0.1f) &&
						ptPlayer->fPosY + (ptPlayer->fSizeY - 0.9f) < (ptSpawner->pos.y + SPAWNER_SIZEY)))
				{
					ptPlayer->fPosX = ptSpawner->pos.x - nCharBite;
					return true;
				}
			}
		}
	}
	return false;
}

bool PlayerCollisionSpawnerL(PLAYER* ptPlayer)
{
	SPAWNER* ptSpawner = GetSpawner();
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++, ptSpawner++)
	{
		if (ptSpawner->bUse)
		{
			if (ptPlayer->fPosX > (ptSpawner->pos.x + SPAWNER_SIZEX / 2) &&
				ptPlayer->fPosX < (ptSpawner->pos.x + SPAWNER_SIZEX + 0.1f))
			{
				if ((ptPlayer->fPosY > (ptSpawner->pos.y + 0.1f) &&
					ptPlayer->fPosY < (ptSpawner->pos.y + SPAWNER_SIZEY)) ||
					(ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) > (ptSpawner->pos.y + 0.1f) &&
						ptPlayer->fPosY + (ptPlayer->fSizeY - 1.0f) < (ptSpawner->pos.y + SPAWNER_SIZEY)))
				{
					ptPlayer->fPosX = ptSpawner->pos.x + SPAWNER_SIZEX + 0.99f;
					return true;
				}
			}
		}
	}
	return false;
}

bool PlayerCollisionSpawnerC(PLAYER* ptPlayer)
{
	SPAWNER* ptSpawner = GetSpawner();
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++, ptSpawner++)
	{
		if (ptSpawner->bUse)
		{
			if (ptPlayer->fPosX > (ptSpawner->pos.x) &&
				ptPlayer->fPosX < (ptSpawner->pos.x + SPAWNER_SIZEX) - 0.92f)
			{
				if (ptPlayer->fPosY > (ptSpawner->pos.y + 1.0) &&
					ptPlayer->fPosY < (ptSpawner->pos.y + SPAWNER_SIZEY) + 0.5f)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool Enemy2CollisionEnemy2(ENEMY* ptEnemy,int nEnemyNum)
{
	//敵の配列のポインタ―取得
	ENEMY* tEnemy = GetEnemy();

	//敵の判定用の四角形を作成
	float fEnemyLeft = ptEnemy->fPosX - CHARACTER_COLLISION_SIZEX;
	float fEnemyRight = ptEnemy->fPosX + ptEnemy->fSizeX - CHARACTER_COLLISION_SIZEX;
	float fEnemyUp = ptEnemy->fPosY - CHARACTER_COLLISION_SIZEY;
	float fEnemyDown = ptEnemy->fPosY + ptEnemy->fSizeY - CHARACTER_COLLISION_SIZEY;

	//全部の敵とのチェック
	for (int nCnt = 0; nCnt < ENEMY1_MAX; nCnt++)
	{
		//自分自身とのヒットを防ぐ
		if (nCnt != nEnemyNum)
		{
			//構造体が使用中か？
			if (tEnemy[nCnt].bUse)
			{//敵の判定用の四角形を作成
				float fEnemyColLeft = tEnemy[nCnt].fPosX - CHARACTER_COLLISION_SIZEX;
				float fEnemyColRight = tEnemy[nCnt].fPosX + tEnemy[nCnt].fSizeX - CHARACTER_COLLISION_SIZEX;
				float fEnemyColUp = tEnemy[nCnt].fPosY - CHARACTER_COLLISION_SIZEY;
				float fEnemyColDown = tEnemy[nCnt].fPosY + tEnemy[nCnt].fSizeY - CHARACTER_COLLISION_SIZEY;

				//当たり判定
				if (fEnemyLeft > fEnemyColRight)
					continue;
				if (fEnemyRight < fEnemyColLeft)
					continue;
				if (fEnemyUp > fEnemyColDown)
					continue;
				if (fEnemyDown < fEnemyColUp)
					continue;

				//敵と敵が当たっている

				if ((ptEnemy->fSpeedX > 0 && tEnemy[nCnt].fSpeedX > 0) || (ptEnemy->fSpeedX < 0 && tEnemy[nCnt].fSpeedX < 0))
				{//進む方向が同じ場合片方反転
					ptEnemy->fSpeedX *= -1.0f;
				}
				else
				{//違う場合両方反転
					ptEnemy->fSpeedX *= -1.0f;
					tEnemy[nCnt].fSpeedX *= -1.0f;
				}

				return true;
			}
		}
	}
	//どの敵とも当たっていない
	return false;

}

bool EnemyCollisionField(ENEMY* ptEnemy)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemy->fPosX > (ptField->fPosX) &&
			ptEnemy->fPosX < (ptField->fPosX + ptField->fWidth))
		{
			if (ptEnemy->fPosY + 1 > (ptField->fPosY - 1.0f) &&
				ptEnemy->fPosY + 1 < (ptField->fPosY + 0.5f))
			{
				HitPosPaintHorizon(ptEnemy->fPosX + 0.5, ptEnemy->fPosY + 1,TILE_EnemyColor);
				ptEnemy->fPosY = ptField->fPosY - 2.0f;
				return true;
			}
		}
	}
	return false;
}

bool EnemyCollisionRight(ENEMY* ptEnemy)
{
	FIELD* ptField = GetField();
	int nCharBite = 2;
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemy->fPosX > (ptField->fPosX - nCharBite) &&
			ptEnemy->fPosX < (ptField->fPosX + ptField->fWidth / 2))
		{
			if ((ptEnemy->fPosY > (ptField->fPosY + 0.1f) && ptEnemy->fPosY < (ptField->fPosY + ptField->fHight - 0.1f)) ||
			   (ptEnemy->fPosY + 1 > (ptField->fPosY + 0.1f) && ptEnemy->fPosY + 1 < (ptField->fPosY + ptField->fHight - 0.1f)))
			{
				HitPosPaintVertical(ptEnemy->fPosX + nCharBite + 0.3, ptEnemy->fPosY, TILE_EnemyColor,RIGHT);
				ptEnemy->fPosX = ptField->fPosX - nCharBite;
				return true;
			}
		}
	}
	return false;
}

bool EnemyCollisionLeft(ENEMY* ptEnemy)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemy->fPosX > (ptField->fPosX + ptField->fWidth / 2) &&
			ptEnemy->fPosX < (ptField->fPosX + ptField->fWidth))
		{
			if ((ptEnemy->fPosY > (ptField->fPosY + 0.1f) && ptEnemy->fPosY < (ptField->fPosY + ptField->fHight - 0.1f)) ||
			   (ptEnemy->fPosY + 1 > (ptField->fPosY + 0.1f) && ptEnemy->fPosY + 1 < (ptField->fPosY + ptField->fHight - 0.1f)))
			{
				HitPosPaintVertical(ptEnemy->fPosX - 0.5, ptEnemy->fPosY, TILE_EnemyColor,LEFT);
				ptEnemy->fPosX = ptField->fPosX + ptField->fWidth;
				return true;
			}
		}
	}
	return false;
}

bool EnemyCollisionCeiling(ENEMY* ptEnemy)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemy->fPosX > (ptField->fPosX) &&
			ptEnemy->fPosX < (ptField->fPosX + ptField->fWidth) - 0.9f)
		{
			if (ptEnemy->fPosY > (ptField->fPosY + 1.0) &&
				ptEnemy->fPosY < (ptField->fPosY + ptField->fHight) + 0.5f)
			{
				HitPosPaintHorizon(ptEnemy->fPosX, ptEnemy->fPosY - 1, TILE_EnemyColor);
				return true;
			}
		}
	}
	return false;
}

bool BulletCollisionField(BULLET* ptBullet)
{
	FIELD* ptField = GetField();

	for (int nField = 0; nField < FIELD_MAX; nField++, ptField++)
	{
		if (ptBullet->fPosX > (ptField->fPosX - 0.5f) &&
			ptBullet->fPosX < (ptField->fPosX + ptField->fWidth - 0.9f))
		{
			if (ptBullet->fPosY > (ptField->fPosY - 1.0f) &&
				ptBullet->fPosY < (ptField->fPosY + 2.0f))
			{
				HitPosPaintHorizon(ptBullet->fPosX, ptBullet->fPosY + 1, TILE_PlayerColor);
				ptBullet->fPosY = ptField->fPosY - 1;

				return true;
			}
		}
	}
	return false;
}

bool BulletCollisionRight(BULLET* ptBullet)
{
	FIELD* ptField = GetField();
	int nCharBite = 2;
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptBullet->fPosX > (ptField->fPosX - nCharBite) &&
			ptBullet->fPosX < (ptField->fPosX + ptField->fWidth / 2))
		{
			if (ptBullet->fPosY > (ptField->fPosY + 0.1f) &&
				ptBullet->fPosY < (ptField->fPosY + ptField->fHight - 0.2f))
			{
				HitPosPaintVertical(ptBullet->fPosX + nCharBite + 0.3, ptBullet->fPosY, TILE_PlayerColor,RIGHT);
				ptBullet->fPosX = ptField->fPosX - nCharBite;
				return true;
			}
		}
	}
	return false;
}

bool BulletCollisionLeft(BULLET* ptBullet)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptBullet->fPosX > (ptField->fPosX + ptField->fWidth / 2) &&
			ptBullet->fPosX < (ptField->fPosX + ptField->fWidth + 0.1f))
		{
			if (ptBullet->fPosY > (ptField->fPosY + 0.1f) &&
				ptBullet->fPosY < (ptField->fPosY + ptField->fHight - 0.2f))
			{
				HitPosPaintVertical(ptBullet->fPosX, ptBullet->fPosY, TILE_PlayerColor,LEFT);
				ptBullet->fPosX = ptField->fPosX + ptField->fWidth + 0.1f;
				return true;
			}
		}
	}
	return false;
}

bool BulletCollisionCeiling(BULLET* ptBullet)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptBullet->fPosX > (ptField->fPosX) &&
			ptBullet->fPosX < (ptField->fPosX + ptField->fWidth) - 0.1f)
		{
			if (ptBullet->fPosY > (ptField->fPosY + ptField->fHight - 0.5f) &&
				ptBullet->fPosY < (ptField->fPosY + ptField->fHight) + 0.5f)
			{
				HitPosPaintHorizon(ptBullet->fPosX, ptBullet->fPosY - 1, TILE_PlayerColor);
				ptBullet->fPosY = ptField->fPosY + ptField->fHight + 1.0f;
				return true;
			}
		}
	}
	return false;
}

bool EnemyBulletCollisionField(ENEMYBULLET* ptEnemyBullet)
{
	FIELD* ptField = GetField();

	for (int nField = 0; nField < FIELD_MAX; nField++, ptField++)
	{
		if (ptEnemyBullet->fPosX > (ptField->fPosX - 0.5f) &&
			ptEnemyBullet->fPosX < (ptField->fPosX + ptField->fWidth - 0.9f))
		{
			if (ptEnemyBullet->fPosY > (ptField->fPosY - 1.0f) &&
				ptEnemyBullet->fPosY < (ptField->fPosY + 2.0f))
			{
				switch (ptEnemyBullet->nType)
				{
				case NORMAL:
					HitPosPaintHorizon(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY + 1, TILE_EnemyColor);
					break;

				case EXPLOSION:
					ExplosionPaint(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, TILE_EnemyColor);
					CreateAnimation(EXPLOSION_FRAME, ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, A_EcolorExpl);
					ExplosionCollision(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, ENEMYSIDE);

					break;
				default:
					break;
				}
				ptEnemyBullet->fPosY = ptField->fPosY - 1;

				return true;
			}
		}
	}
	return false;
}

bool EnemyBulletCollisionRight(ENEMYBULLET* ptEnemyBullet)
{
	FIELD* ptField = GetField();
	int nCharBite = 2;
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemyBullet->fPosX > (ptField->fPosX - nCharBite) &&
			ptEnemyBullet->fPosX < (ptField->fPosX + ptField->fWidth / 2))
		{
			if (ptEnemyBullet->fPosY > (ptField->fPosY + 0.1f) &&
				ptEnemyBullet->fPosY < (ptField->fPosY + ptField->fHight - 0.2f))
			{
				switch (ptEnemyBullet->nType)
				{
				case NORMAL:
					HitPosPaintVertical(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY - 1, TILE_EnemyColor,RIGHT);
					break;

				case EXPLOSION:
					ExplosionPaint(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, TILE_EnemyColor);
					CreateAnimation(EXPLOSION_FRAME, ptEnemyBullet->fPosX, ptEnemyBullet->fPosY,A_EcolorExpl);
					ExplosionCollision(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, ENEMYSIDE);
					break;
				default:
					break;
				}
				ptEnemyBullet->fPosX = ptField->fPosX - nCharBite;
				return true;
			}
		}
	}
	return false;
}

bool EnemyBulletCollisionLeft(ENEMYBULLET* ptEnemyBullet)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemyBullet->fPosX > (ptField->fPosX + ptField->fWidth / 2) &&
			ptEnemyBullet->fPosX < (ptField->fPosX + ptField->fWidth + 0.1f))
		{
			if (ptEnemyBullet->fPosY > (ptField->fPosY + 0.1f) &&
				ptEnemyBullet->fPosY < (ptField->fPosY + ptField->fHight - 0.2f))
			{
				switch (ptEnemyBullet->nType)
				{
				case NORMAL:
					HitPosPaintVertical(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY - 1, TILE_EnemyColor,LEFT);
					break;

				case EXPLOSION:
					ExplosionPaint(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, TILE_EnemyColor);
					CreateAnimation(EXPLOSION_FRAME, ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, A_EcolorExpl);
					ExplosionCollision(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, ENEMYSIDE);
					break;
				default:
					break;
				}
				ptEnemyBullet->fPosX = ptField->fPosX + ptField->fWidth + 0.1f;
				return true;
			}
		}
	}
	return false;
}

bool EnemyBulletCollisionCeiling(ENEMYBULLET* ptEnemyBullet)
{
	FIELD* ptField = GetField();
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++, ptField++)
	{
		if (ptEnemyBullet->fPosX > (ptField->fPosX) &&
			ptEnemyBullet->fPosX < (ptField->fPosX + ptField->fWidth) - 0.1f)
		{
			if (ptEnemyBullet->fPosY > (ptField->fPosY + ptField->fHight - 0.5f) &&
				ptEnemyBullet->fPosY < (ptField->fPosY + ptField->fHight) + 0.5f)
			{
				switch (ptEnemyBullet->nType)
				{
				case NORMAL:
					HitPosPaintHorizon(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY - 1, TILE_EnemyColor);
					break;

				case EXPLOSION:
					ExplosionPaint(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY,TILE_EnemyColor);
					CreateAnimation(EXPLOSION_FRAME, ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, A_EcolorExpl);
					ExplosionCollision(ptEnemyBullet->fPosX, ptEnemyBullet->fPosY, ENEMYSIDE);
					break;
				default:
					break;
				}
				ptEnemyBullet->fPosY = ptField->fPosY + ptField->fHight + 1.0f;
				return true;
			}
		}
	}
	return false;
}

void ExplosionCollision(float fX, float fY,int nSide)
{
	float posX = fX - (EXPLOSION_X / 2);
	float posY = fY - (EXPLOSION_Y / 2);

	float fExplosionL = posX;
	float fExplosionR = posX + EXPLOSION_X;
	float fExplosionU = posY;;
	float fExplosionD = posY + EXPLOSION_Y;

	PLAYER* tPlayer = GetPlayer();
	ENEMY* tEnemy = GetEnemy();

	//当たり判定
	switch (nSide)
	{
	case PLAYERSIDE://プレイヤー側からの呼び出し
		for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
		{
			if (tEnemy[nCnt].bUse && !tEnemy[nCnt].bHit) 
			{//使用中かつ無敵時間外
				//矩形作成
				float fEnemyLeft = tEnemy[nCnt].fPosX - CHARACTER_COLLISION_SIZEX;
				float fEnemyRight = tEnemy[nCnt].fPosX + tEnemy[nCnt].fSizeX - CHARACTER_COLLISION_SIZEX;
				float fEnemyUp = tEnemy[nCnt].fPosY - CHARACTER_COLLISION_SIZEY;
				float fEnemyDown = tEnemy[nCnt].fPosY + tEnemy[nCnt].fSizeY - CHARACTER_COLLISION_SIZEY;

				//当たり判定
				if (fEnemyLeft > fExplosionR)
					continue;
				if (fEnemyRight < fExplosionL)
					continue;
				if (fEnemyUp > fExplosionD)
					continue;
				if (fEnemyDown < fExplosionU)
					continue;

				tEnemy[nCnt].bExplHit = true;

			}
		}

		break;

	case ENEMYSIDE:	//エネミー側からの呼び出し
		if (!tPlayer->bHit)
		{
			//矩形作成
			float fPlayerLeft = tPlayer->fPosX - CHARACTER_COLLISION_SIZEX;
			float fPlayerRight = tPlayer->fPosX + tPlayer->fSizeX - CHARACTER_COLLISION_SIZEX;
			float fPlayerUp = tPlayer->fPosY - CHARACTER_COLLISION_SIZEY;
			float fPlayerDown = tPlayer->fPosY + tPlayer->fSizeY - CHARACTER_COLLISION_SIZEY;

			//当たり判定
			if (fPlayerLeft > fExplosionR)
				break;
			if (fPlayerRight < fExplosionL)
				break;
			if (fPlayerUp > fExplosionD)
				break;
			if (fPlayerDown < fExplosionU)
				break;

			tPlayer->bExplHit = true;
		}
		break;

	default:
		break;
	}

}

void HitPosPaintHorizon(float x, float y,MAPTILE nColor)
{
	g_pnColMap[OffsetMapPointerF(x, y)] = nColor;
	g_pnColMap[OffsetMapPointerF(x-1, y)] = nColor;
	g_pnColMap[OffsetMapPointerF(x+1, y)] = nColor;
}

void HitPosPaintVertical(float x, float y, MAPTILE nColor, int nSide)
{
	if (nSide == RIGHT)
	{
		//xを偶数に補正
		if ((int)x % 2 == 1)
		{
			x--;
		}
	}

	g_pnColMap[OffsetMapPointerF(x, y)] = nColor;
	g_pnColMap[OffsetMapPointerF(x, y-1)] = nColor;
	g_pnColMap[OffsetMapPointerF(x, y+1)] = nColor;
}


void ExplosionPaint(float fX, float fY, MAPTILE color)
{
	//位置を中心とした(n*2)*nの色を塗る、posX,Yは四角形の左上
	int posX = (int)fX - EXPLOSION_X / 2;
	int posY = (int)fY - EXPLOSION_Y / 2;

	for (int y = 0; y < EXPLOSION_Y + 1; y++)
	{
		for (int x = 0; x < EXPLOSION_X + 1; x++)
		{
			SetColorMapOffset(posX + x, posY + y, color);
		}
	}
}
