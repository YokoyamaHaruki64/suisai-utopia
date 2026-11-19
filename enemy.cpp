#define CONIOEX
#include "conioex.h"
#include "player.h"
#include "enemy.h"
#include "game.h"
#include "collision.h"
#include "score.h"
#include "define.h"
#include "drawWindow.h"
#include "animation.h"
#include "UI.h"
#include <iostream>

#define DESTROY_PAINT_X		(EXPLOSION_X + 2)
#define DESTROY_PAINT_Y		(EXPLOSION_Y + 1)

#define	ENEMY_MUTEKIJIKAN	40

#define ENEMY1_SHOT_CT		120
#define ENEMY1_HP_MAX		1

#define ENEMY2_SPEED		(MOVE_SPEED_X * 1.0)	//スピード
#define ENEMY2_SPEED		(MOVE_SPEED_X * 1.0)	//スピード
#define ENEMY2_HP_MAX		2
#define ENEMY2_JUMPPOW		(0.7)	//ジャンプ力
#define ENEMY2_SEARCH_DISTANCE	(30)//敵がプレイヤーを追い始める距離
#define P_TO_E2_MAX			(30)	//プレイヤーと敵の距離の正規化に使う
#define P_TO_E2_MIN			(5)	//プレイヤーと敵の距離の正規化に使う
#define EXPL_SHOT_CT_MIN	(100)	//ボム投げクールタイム(最短)
#define EXPL_SHOT_CT_MAX	(200)	//ボム投げクールタイム(最長)

#define E_RANDOMSPEED_MIN		(1)		//ランダムスピードの幅
#define E_RANDOMSPEED_MAX		(100)	//ランダムスピードの幅



ENEMY g_tEnemy[ENEMY_MAX];
bool g_bUseBullets[ENEMY_MAX];
MAPTILE* g_pnEnemyMap;
bool once;

//敵キャラ初期配置構造体
struct EnemyData
{
	 float fX;
	 float fY;
	 float fMx;
	 float fMy;
	 ENEMYID nID;
	 ENEMYBULLET* ptE_Bullet;

};
//初期配置データテーブル
EnemyData g_tEnemyTable[INIT_ENEMY_MAX]
{
	{30,2,0.6,-0.1,MOVE_RANDOM},
	{80,20,-0.4,0.0,MOVE_RANDOM},
	{50,13,0.3,0.5,MOVE_CHASE}
};

void InitEnemy()
{
	memset(g_tEnemy,0,sizeof(ENEMY) * ENEMY_MAX);
	memset(g_bUseBullets, 0, sizeof(bool) * ENEMY_MAX);
	g_pnEnemyMap = GetMap();

	for (int nCnt = 0; nCnt < INIT_ENEMY_MAX; nCnt++)
	{
		CreateEnemy(g_tEnemyTable[nCnt].fX, g_tEnemyTable[nCnt].fY,
					g_tEnemyTable[nCnt].fMx, g_tEnemyTable[nCnt].fMy,
					g_tEnemyTable[nCnt].nID,nCnt);

		g_bUseBullets[nCnt] = true;
	}

	once = true;
}

void UnInitEnemy()
{
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		SetMapOffset(g_tEnemy[nCnt].fPosOldX, g_tEnemy[nCnt].fPosOldY, TILE_Blank);
		SetMapOffset(g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY, TILE_Blank);
	}
}

void UpdateEnemy()
{
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		if (g_tEnemy[nCnt].bUse)
		{
			switch (g_tEnemy[nCnt].nID)
			{
			case MOVE_RANDOM:
				Enemy1(nCnt);
				break;

			case MOVE_CHASE:
				Enemy2(nCnt);
				break;
			default:

				break;
			}
		}
	}

	if (GetGameTime() % 30 == 29 && once)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			//空いてる弾配列の探索
			int nBulletID = 0;
			bool bMax = true;

			for (nBulletID; nBulletID < ENEMY_MAX; nBulletID++)
			{
				if (!g_bUseBullets[nBulletID])
				{//空き配列を見つけたら終了、nBulletIDをそのまま引数に使う
					bMax = false;
					break;
				}
			}

			if (bMax)
			{//弾配列が埋まってたら湧き不可
				continue;
			}

			switch (nCnt)
			{
			case 0:
				CreateEnemy(10, 13, 0.3, 0.4, MOVE_RANDOM, nBulletID);
				break;

			case 1:
				CreateEnemy(80, 3, 0.0, 0.0, MOVE_CHASE, nBulletID);
				break;

			case 2:
				CreateEnemy(180, 16, -0.8, 0.1, MOVE_RANDOM, nBulletID);
				break;

			default:
				break;
			}
		}
		once = false;
	}
	else if (GetGameTime() % 20 == 11)
	{
		once = true;
	}
}

void DrawEnemy()
{
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		if ((g_tEnemy[nCnt].fPosX != g_tEnemy[nCnt].fPosOldX) ||
			(g_tEnemy[nCnt].fPosY != g_tEnemy[nCnt].fPosOldY))
		{
			SetMapOffset(g_tEnemy[nCnt].fPosOldX, g_tEnemy[nCnt].fPosOldY,TILE_Blank);
			SetMapOffset(g_tEnemy[nCnt].fPosOldX + 1, g_tEnemy[nCnt].fPosOldY,TILE_Blank);
		}

		if (g_tEnemy[nCnt].bUse)
		{
			if (g_tEnemy[nCnt].bVisible)
			{
				switch (g_tEnemy[nCnt].nID)
				{
				case MOVE_RANDOM:
					SetMapOffset(g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY, TILE_Enemy1);
					SetMapOffset(g_tEnemy[nCnt].fPosX + 1, g_tEnemy[nCnt].fPosY, TILE_Zenkaku);
					break;

				case MOVE_CHASE:
					SetMapOffset(g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY, TILE_Enemy2);
					SetMapOffset(g_tEnemy[nCnt].fPosX + 1, g_tEnemy[nCnt].fPosY, TILE_Zenkaku);
					break;

				default:
					break;
				}
			}
			else
			{
				SetMapOffset(g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY, TILE_Blank);
				SetMapOffset(g_tEnemy[nCnt].fPosX + 1, g_tEnemy[nCnt].fPosY, TILE_Blank);
			}

		}
	}
}

void CreateEnemy(float x, float y, float mx, float my,ENEMYID nID,int nBulletsArrayID)
{
	ENEMYBULLET* pEnemyBullet = GetEnemyBullet();
	pEnemyBullet += nBulletsArrayID;
	for (int nCnt = 0; nCnt < ENEMY_MAX; nCnt++)
	{
		if (!g_tEnemy[nCnt].bUse)
		{
			g_tEnemy[nCnt].bUse = true;
			g_tEnemy[nCnt].fPosOldX = g_tEnemy[nCnt].fPosX = x;
			g_tEnemy[nCnt].fPosOldY = g_tEnemy[nCnt].fPosY = y;
			g_tEnemy[nCnt].fSpeedX = mx;
			g_tEnemy[nCnt].fSpeedY = my;
			g_tEnemy[nCnt].nID = nID;
			g_tEnemy[nCnt].ptE_Bullet = pEnemyBullet;
			g_tEnemy[nCnt].nBulletArrayID = nBulletsArrayID;
			g_tEnemy[nCnt].nShotCT = RandomRange(100,200);
			g_tEnemy[nCnt].nMutekiCnt = 0;

			g_bUseBullets[nBulletsArrayID] = true;

			switch (nID)
			{
			case MOVE_RANDOM:
				g_tEnemy[nCnt].fSizeX = 1.0f;
				g_tEnemy[nCnt].fSizeY = 1.0f;
				g_tEnemy[nCnt].nHP = ENEMY1_HP_MAX;
				break;

			case MOVE_CHASE:
				g_tEnemy[nCnt].fSizeX = 1.0f;
				g_tEnemy[nCnt].fSizeY = 2.0f;
				g_tEnemy[nCnt].nDirCnt = 0;
				g_tEnemy[nCnt].nHP = ENEMY2_HP_MAX;
				break;

			default:
				break;
			}


			break;
		}
	}
}

ENEMY* GetEnemy(void)
{
	return g_tEnemy;
}

bool* GetUsedE_Bullet(void)
{
	return g_bUseBullets;
}

void ResetEnemyBullets(int nBulletID)
{
	ENEMYBULLET* ptEneBul = GetEnemyBullet();
	ptEneBul += nBulletID;
	memset(ptEneBul, 0, sizeof(ENEMYBULLET));
	g_bUseBullets[nBulletID] = false;
}

void Enemy1(int nCnt)
{
	//現在の座標を保存
	g_tEnemy[nCnt].fPosOldX = g_tEnemy[nCnt].fPosX;
	g_tEnemy[nCnt].fPosOldY = g_tEnemy[nCnt].fPosY;

	PLAYER* ptPlayer = GetPlayer();
	g_tEnemy[nCnt].EtoP.x = ptPlayer->fPosX - g_tEnemy[nCnt].fPosX;
	g_tEnemy[nCnt].EtoP.y = ptPlayer->fPosY - g_tEnemy[nCnt].fPosY;
	float fDistance = Vec2_Magnitude(g_tEnemy[nCnt].EtoP);

	//座標更新
	if (g_tEnemy[nCnt].nMutekiCnt > 0 && g_tEnemy[nCnt].nMutekiCnt <= 20)
	{//ヒットストップ
		g_tEnemy[nCnt].fPosX += g_tEnemy[nCnt].fSpeedX * 0.0f;
		g_tEnemy[nCnt].fPosY += g_tEnemy[nCnt].fSpeedY * 0.0f;
	}
	else
	{
		g_tEnemy[nCnt].fPosX += g_tEnemy[nCnt].fSpeedX;
		g_tEnemy[nCnt].fPosY += g_tEnemy[nCnt].fSpeedY;
	}

	//横で壁にぶつかったとき
	if (g_tEnemy[nCnt].fPosX < SCREEN_LEFT)
	{
		g_tEnemy[nCnt].fSpeedX *= -1.0;//移動量反転
		g_tEnemy[nCnt].fPosX = SCREEN_LEFT;
	}
	if (g_tEnemy[nCnt].fPosX > MAP_X - 2)
	{
		g_tEnemy[nCnt].fSpeedX *= -1.0;
		g_tEnemy[nCnt].fPosX = MAP_X - 2;
	}

	//横で壁にぶつかったとき
	if (g_tEnemy[nCnt].fPosY < SCREEN_TOP + 1)
	{
		g_tEnemy[nCnt].fSpeedY *= -1.0;
		g_tEnemy[nCnt].fPosY = SCREEN_TOP + 1;
	}
	if (g_tEnemy[nCnt].fPosY > SCREEN_BOTTOM - 2)
	{
		g_tEnemy[nCnt].fSpeedY *= -1.0;
		g_tEnemy[nCnt].fPosY = SCREEN_BOTTOM - 2;
	}

	//床
	if (EnemyCollisionField(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedY *= -1.0;//移動量反転
	}
	//天井
	if (EnemyCollisionCeiling(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedY *= -1.0;
	}
	//右
	if (EnemyCollisionRight(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedX *= -1.0;//移動量反転
	}
	//左
	if (EnemyCollisionLeft(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedX *= -1.0;//移動量反転
	}

	//拡散ショット
	g_tEnemy[nCnt].nShotCTCnt++;
	if (fDistance <= 80)
	{
		if (g_tEnemy[nCnt].nShotCTCnt >= g_tEnemy[nCnt].nShotCT)
		{
			for (int nAngle = 0; nAngle < 360; nAngle += 72)
			{
				VECTOR2 vec = AngleToVector2((float)nAngle);
				CreateEnemyBullet(g_tEnemy[nCnt].fPosX + vec.x, g_tEnemy[nCnt].fPosY + vec.y,
					vec.x, vec.y, NORMAL, g_tEnemy[nCnt].nBulletArrayID);
			}

			g_tEnemy[nCnt].nShotCTCnt = 0;
		}
	}

	if (g_tEnemy[nCnt].bExplHit)
	{
		g_tEnemy[nCnt].nHP -= 2;
		g_tEnemy[nCnt].bHit = true;
		g_tEnemy[nCnt].bExplHit = false;
		if (g_tEnemy[nCnt].nHP <= 0)
		{
			g_tEnemy[nCnt].nHP = 0;
		}
	}

	//弾との当たり判定
	if (!g_tEnemy[nCnt].bHit)
	{
		g_tEnemy[nCnt].bHit = EnemyCollisionBullet(&g_tEnemy[nCnt]);
		if (g_tEnemy[nCnt].bHit)
		{
			g_tEnemy[nCnt].nHP--;
		}
	}
	else
	{
		g_tEnemy[nCnt].nMutekiCnt++;
		if (g_tEnemy[nCnt].nMutekiCnt >= ENEMY_MUTEKIJIKAN)
		{
			g_tEnemy[nCnt].bHit = false;
			g_tEnemy[nCnt].nMutekiCnt = 0;
		}
	}

	g_tEnemy[nCnt].bVisible = Blinking(g_tEnemy[nCnt].nMutekiCnt);

	if (g_tEnemy[nCnt].nHP <= 0)
	{//死
		g_tEnemy[nCnt].bUse = false;
		g_bUseBullets[g_tEnemy[nCnt].nBulletArrayID] = false;
		ExplosionPaint(g_tEnemy[nCnt].fPosX,g_tEnemy[nCnt].fPosY, TILE_PlayerColor);
		CreateAnimation(EXPLOSION_FRAME,g_tEnemy[nCnt].fPosX,g_tEnemy[nCnt].fPosY,A_PcolorExpl);


		RemoveEnemyBullets(nCnt);//描画を消す
		ResetEnemyBullets(g_tEnemy[nCnt].nBulletArrayID);//弾の配列を解放する
	}
}

void Enemy2(int nCnt)
{
	PLAYER* ptPlayer = GetPlayer();
	g_tEnemy[nCnt].EtoP.x = ptPlayer->fPosX - g_tEnemy[nCnt].fPosX;
	g_tEnemy[nCnt].EtoP.y = ptPlayer->fPosY - g_tEnemy[nCnt].fPosY;
	float fDistance = Vec2_Magnitude(g_tEnemy[nCnt].EtoP);
	///////////////////////////////////////////////
	//ステート設定
	switch (g_tEnemy[nCnt].moveState)
	{
	case IDLE:
		//アイドルtoチェイス
		if (fDistance <= ENEMY2_SEARCH_DISTANCE)
		{
			g_tEnemy[nCnt].moveState = CHASE;
			g_tEnemy[nCnt].nDirCnt = 0;
			g_tEnemy[nCnt].nDirMax = RandomRange(30, 120);	//折り返し時間の再設定
			g_tEnemy[nCnt].nShotCT = RandomRange(EXPL_SHOT_CT_MIN, EXPL_SHOT_CT_MAX);
		}
		break;

	case CHASE:
		//チェイスtoアイドル
		if (fDistance >= ENEMY2_SEARCH_DISTANCE + 10)
		{
			g_tEnemy[nCnt].moveState = IDLE;
			g_tEnemy[nCnt].nDirCnt = 0;
			g_tEnemy[nCnt].nDirMax = RandomRange(180, 240);	//折り返し時間の再設定
			g_tEnemy[nCnt].nShotCT = RandomRange(EXPL_SHOT_CT_MIN * 5, EXPL_SHOT_CT_MAX * 5);
		}
		break;
	default:
		break;
	}


	//現在の座標位置を保存
	g_tEnemy[nCnt].fPosOldX = g_tEnemy[nCnt].fPosX;
	g_tEnemy[nCnt].fPosOldY = g_tEnemy[nCnt].fPosY;


	//移動方向の更新
	if (g_tEnemy[nCnt].nDirCnt == 0)
	{
		switch (g_tEnemy[nCnt].moveState)
		{
		case IDLE:
			//ランダム行動
			g_tEnemy[nCnt].fSpeedX = ENEMY2_SPEED * (Normarize0to1(E_RANDOMSPEED_MIN,E_RANDOMSPEED_MAX,RandomRange(E_RANDOMSPEED_MIN,E_RANDOMSPEED_MAX) + 0.1f));
			if (_1d2() == 2)
			{
				g_tEnemy[nCnt].fSpeedX *= -1;
			}
			g_tEnemy[nCnt].nDirMax = RandomRange(180,250);	//折り返し時間の再設定
			break;

		case CHASE:
			//プレイヤーを追う
			if (g_tEnemy[nCnt].EtoP.x > 0)
			{//10～30に値を丸め、その値で正規化、0のときのために0.1を足して、スピードをかける
				g_tEnemy[nCnt].fSpeedX = ENEMY2_SPEED * (Normarize0to1(P_TO_E2_MIN, P_TO_E2_MAX, ClampInt(P_TO_E2_MIN, P_TO_E2_MAX, abs(g_tEnemy[nCnt].EtoP.x))) + 0.1f);
			}
			else
			{
				g_tEnemy[nCnt].fSpeedX = -ENEMY2_SPEED * (Normarize0to1(P_TO_E2_MIN, P_TO_E2_MAX, ClampInt(P_TO_E2_MIN, P_TO_E2_MAX, abs(g_tEnemy[nCnt].EtoP.x))) + 0.1f);
			}
			g_tEnemy[nCnt].nDirMax = RandomRange(30,100);	//折り返し時間の再設定
			break;

		default:
			break;
		}
	}

	//折り返し時間のカウント
	g_tEnemy[nCnt].nDirCnt++;
	if (g_tEnemy[nCnt].nDirCnt >= g_tEnemy[nCnt].nDirMax)
	{//次のフレームで折り返しするように
		g_tEnemy[nCnt].nDirCnt = 0;
	}

	if (g_tEnemy[nCnt].nMutekiCnt > 0 && g_tEnemy[nCnt].nMutekiCnt <= 30)
	{//ヒットストップ
		g_tEnemy[nCnt].fSpeedX = 0.0f;
	}
	//else if (g_tEnemy[nCnt].nMutekiCnt > 0)
	//{//無敵中は速度を落とす
	//	g_tEnemy[nCnt].fSpeedX /= 3.0f;
	//}

	//座標更新
	//X座標制御
	g_tEnemy[nCnt].fPosX += g_tEnemy[nCnt].fSpeedX;

	//Y座標制御
	g_tEnemy[nCnt].fSpeedY += VALUE_GRAVITY;
	g_tEnemy[nCnt].fPosY += g_tEnemy[nCnt].fSpeedY;


	//着地
	if (g_tEnemy[nCnt].fPosY >= SCREEN_BOTTOM - 1.5f)
	{
		g_tEnemy[nCnt].fPosY = SCREEN_BOTTOM - 1.5f;
		g_tEnemy[nCnt].fSpeedY = 0.0f;
		g_tEnemy[nCnt].bJump = false;
		g_tEnemy[nCnt].bLand = true;
	}

	//ジャンプ制御
	if (g_tEnemy[nCnt].moveState == CHASE)
	{
		if (!g_tEnemy[nCnt].bLand && !g_tEnemy[nCnt].bJump && g_tEnemy[nCnt].EtoP.y <= -3)
		{//空中かつジャンプ中じゃないかつプレイヤーが自身より3マス以上上にいる場合ジャンプ
			EnemyJump(nCnt);
			g_tEnemy[nCnt].bLand = false;

		}
	}

	Enemy2CollisionEnemy2(&g_tEnemy[nCnt],nCnt);

	//当たり判定
	//床
	if (EnemyCollisionField(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedY = 0;
		g_tEnemy[nCnt].bJump = false;
		g_tEnemy[nCnt].bLand = true;
	}
	else
	{
		g_tEnemy[nCnt].bLand = false;
	}
	//天井
	if (EnemyCollisionCeiling(&g_tEnemy[nCnt]))
	{
		g_tEnemy[nCnt].fSpeedY = VALUE_GRAVITY;
	}
	//右
	if (EnemyCollisionRight(&g_tEnemy[nCnt]))
	{
		if (!g_tEnemy[nCnt].bJump)
		{
			EnemyJump(nCnt);
		}
	}
	//左
	if (EnemyCollisionLeft(&g_tEnemy[nCnt]))
	{
		if (!g_tEnemy[nCnt].bJump)
		{
			EnemyJump(nCnt);
		}
	}

	//座標制御
	//横で壁にぶつかったとき
	if (g_tEnemy[nCnt].fPosX < SCREEN_LEFT)
	{
		g_tEnemy[nCnt].fPosX = SCREEN_LEFT;
	}
	if (g_tEnemy[nCnt].fPosX > MAP_X - 2)
	{
		g_tEnemy[nCnt].fPosX = MAP_X - 2;
	}

	//横で壁にぶつかったとき
	if (g_tEnemy[nCnt].fPosY < SCREEN_TOP + 1)
	{
		g_tEnemy[nCnt].fPosY = SCREEN_TOP + 1;
		g_tEnemy[nCnt].fSpeedY = VALUE_GRAVITY;
	}

	//攻撃
	g_tEnemy[nCnt].nShotCTCnt++;
	if (g_tEnemy[nCnt].nShotCTCnt >= g_tEnemy[nCnt].nShotCT)
	{
		CreateEnemyBullet(g_tEnemy[nCnt].fPosX,g_tEnemy[nCnt].fPosY,
							Vec2_Normarize(g_tEnemy[nCnt].EtoP).x * 2.0f,-2.0f,EXPLOSION,g_tEnemy[nCnt].nBulletArrayID);


		//CTの再設定
		switch (g_tEnemy[nCnt].moveState)
		{
		case IDLE:
			g_tEnemy[nCnt].nShotCT = RandomRange(EXPL_SHOT_CT_MIN * 5, EXPL_SHOT_CT_MAX * 5);
			break;

		case CHASE:
			g_tEnemy[nCnt].nShotCT = RandomRange(EXPL_SHOT_CT_MIN, EXPL_SHOT_CT_MAX);
			break;

		default:
			break;
		}

		g_tEnemy[nCnt].nShotCTCnt = 0;

	}

	///////////////////////////////////////////////

	if (g_tEnemy[nCnt].bExplHit)
	{
		g_tEnemy[nCnt].nHP -= 2;
		g_tEnemy[nCnt].bHit = true;
		g_tEnemy[nCnt].bExplHit = false;

		if (g_tEnemy[nCnt].nHP <= 0)
		{
			g_tEnemy[nCnt].nHP = 0;
		}
	}

	//弾との当たり判定
	if (!g_tEnemy[nCnt].bHit)
	{
		g_tEnemy[nCnt].bHit = EnemyCollisionBullet(&g_tEnemy[nCnt]);
		if (g_tEnemy[nCnt].bHit)
		{
			g_tEnemy[nCnt].nHP--;
		}
	}
	else
	{
		g_tEnemy[nCnt].nMutekiCnt++;
		if (g_tEnemy[nCnt].nMutekiCnt >= ENEMY_MUTEKIJIKAN)
		{
			g_tEnemy[nCnt].bHit = false;
			g_tEnemy[nCnt].nMutekiCnt = 0;
		}
	}

	g_tEnemy[nCnt].bVisible = Blinking(g_tEnemy[nCnt].nMutekiCnt);

	if (g_tEnemy[nCnt].nHP <= 0)
	{//死
		g_tEnemy[nCnt].bUse = false;
		g_bUseBullets[g_tEnemy[nCnt].nBulletArrayID] = false;
		ExplosionPaint(g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY,TILE_PlayerColor);
		CreateAnimation(EXPLOSION_FRAME, g_tEnemy[nCnt].fPosX, g_tEnemy[nCnt].fPosY, A_PcolorExpl);
		RemoveEnemyBullets(nCnt);//弾の描画を消す
		ResetEnemyBullets(g_tEnemy[nCnt].nBulletArrayID);//弾の配列を解放する
	}

}

void EnemyJump(int nCnt)
{
	g_tEnemy[nCnt].fSpeedY = -ENEMY2_JUMPPOW;
	g_tEnemy[nCnt].bJump = true;
}

