#include "enemy.h"
#include "drawWindow.h"
#include "enemySpawn.h"
#include "define.h"
#include "collision.h"
#include "animation.h"
#include "UI.h"

#define SPAWNER_MUTEKIJIKAN		(5)
#define ENEMYSPEED_MIN	(30)		//これを100で割った値を使う
#define ENEMYSPEED_MAX	(60)		//これを100で割った値を使う
SPAWNER g_tSpawner[SPAWNER_MAX];

void InitEnemySpawn()
{
	memset(g_tSpawner,0,sizeof(SPAWNER) * SPAWNER_MAX);
	CreateSpawner(93.0f,16.0f,15,240);
}

void UnInitEnemySpawn()
{
}

void UpdateEnemySpawn()
{
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++)
	{
		if (g_tSpawner[nCnt].bUse)
		{
			g_tSpawner[nCnt].nSpawnCTCnt++;
			if (g_tSpawner[nCnt].nMutekiCnt > 0)
			{
				g_tSpawner[nCnt].nMutekiCnt--;
			}
			if (g_tSpawner[nCnt].nSpawnCTCnt >= g_tSpawner[nCnt].nSpawnCT)
			{//敵を湧かせる
				//カウンターのリセット
				g_tSpawner[nCnt].nSpawnCTCnt = 0;

				bool bSpawn[ENEMY_KINDS];	//どの敵の種類が湧けるか判定用
				bool bEnd = true;			//一体も敵が湧けない場合

				int nEnemyAmount[ENEMY_KINDS] = {};	//どの敵が何体いるか
				int nEnemyMaxAmount[ENEMY_KINDS] = {ENEMY1_MAX,ENEMY2_MAX};//敵の最大数を登録
				memset(bSpawn,1,sizeof(bool));
				ENEMY* eneData = GetEnemy();

				for (int nEnemy = 0; nEnemy < ENEMY_MAX; nEnemy++, eneData++)
				{
					if (eneData->bUse)
					{//使ってる敵の種類のカウント
						nEnemyAmount[eneData->nID]++;
					}
					else
					{
						bEnd = false;//最低でも一体は湧く
					}
				}
				if (bEnd)
				{
					return;//このフレームはこれ以上敵が湧けないので関数を終了
				}

				for (int nKinds = 0; nKinds < ENEMY_KINDS; nKinds++)
				{//対応する種類が最大数以上なら湧けない判定をする
					if (nEnemyAmount[nKinds] >= nEnemyMaxAmount[nKinds])
					{
						bSpawn[nKinds] = false;
					}
				}

				bool bNotSpawn = true;
				ENEMYID eID;

				eID = (ENEMYID)RandomRange(0, ENEMY_KINDS - 1);
				if (bSpawn[eID])
				{//湧ける場合湧かせる
					bNotSpawn = false;
				}

				if (!bNotSpawn)
				{
					//空いてる弾配列の探索
					bool* bUseBullet = GetUsedE_Bullet();
					int nBulletID = 0;
					for (nBulletID; nBulletID < ENEMY_MAX; nBulletID++)
					{
						if (!bUseBullet[nBulletID])
						{//空き配列を見つけたら終了、nBulletIDをそのまま引数に使う
							break;
						}
					}
					VECTOR2 dir = RandomVector();
					float fSpeed = (float)RandomRange(ENEMYSPEED_MIN, ENEMYSPEED_MAX) / 100.0f;
					CreateEnemy(g_tSpawner[nCnt].pos.x, g_tSpawner[nCnt].pos.y,
						dir.x * fSpeed, dir.y * fSpeed,
						eID, nBulletID);
				}
			}

			//ここにコリジョンを書く
			if(SpawnerCollisionBullet(&g_tSpawner[nCnt]) && g_tSpawner[nCnt].nMutekiCnt <= 0)
			{
				g_tSpawner[nCnt].nHP--;
				g_tSpawner[nCnt].nMutekiCnt = SPAWNER_MUTEKIJIKAN;
			}

			if (g_tSpawner[nCnt].nHP <= 0)
			{
				DestroySpawner(nCnt);
			}

		}
	}


}

void DrawEnemySpawn()
{
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++)
	{
		if (g_tSpawner[nCnt].bUse)
		{
			for (int x = 0; x < SPAWNER_SIZEX; x++)
			{
				for (int y = 0; y < SPAWNER_SIZEY; y++)
				{
					switch (x % 2)
					{//全角対応のためのスイッチ
					case 0:
						SetMapOffset(g_tSpawner[nCnt].pos.x + x, g_tSpawner[nCnt].pos.y + y, TILE_Spawner);
						break;

					case 1:
						SetMapOffset(g_tSpawner[nCnt].pos.x + x, g_tSpawner[nCnt].pos.y + y, TILE_Zenkaku);
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

void CreateSpawner(float fX, float fY, int nHP,int nSpawnCT)
{
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++)
	{
		if (!g_tSpawner[nCnt].bUse)
		{
			g_tSpawner[nCnt].pos.x = g_tSpawner[nCnt].posOld.x = fX;
			g_tSpawner[nCnt].pos.y = g_tSpawner[nCnt].posOld.y = fY;
			g_tSpawner[nCnt].nMaxHP = g_tSpawner[nCnt].nHP = nHP;
			g_tSpawner[nCnt].nSpawnCT = nSpawnCT;
			g_tSpawner[nCnt].nSpawnCTCnt = 0;
			g_tSpawner[nCnt].nMutekiCnt = 0;
			g_tSpawner[nCnt].bUse = true;

			break;
		}
	}
}

void DestroySpawner(int nCnt)
{
	for (int x = 0; x < SPAWNER_SIZEX; x++)
	{
		for (int y = 0; y < SPAWNER_SIZEY; y++)
		{
			SetMapOffset(g_tSpawner[nCnt].pos.x + x, g_tSpawner[nCnt].pos.y + y, TILE_Blank);
		}
	}
	CreateAnimation(EXPLOSION_FRAME,g_tSpawner[nCnt].pos.x,g_tSpawner[nCnt].pos.y,A_PcolorExpl);
	ExplosionPaint(g_tSpawner[nCnt].pos.x, g_tSpawner[nCnt].pos.y,TILE_PlayerColor);
	g_tSpawner[nCnt].bUse = false;

}

SPAWNER* GetSpawner()
{
	return g_tSpawner;
}