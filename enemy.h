#pragma once

#include "enemyBullet.h"
#include "define.h"
#define ENEMY1_MAX	(3)	//同時出現数
#define ENEMY2_MAX	(1)	//同時出現数
#define ENEMY_MAX	(ENEMY1_MAX + ENEMY2_MAX)	//同時出現数
#define INIT_ENEMY_MAX	(3)	//初期出現数

enum ENEMYID
{
	MOVE_RANDOM = 0,
	MOVE_CHASE,

	ENEMY_KINDS
};


enum ENEMY_STATE
{
	IDLE,
	CHASE
};


struct ENEMY
{
	ENEMY_STATE moveState;
	float fPosX;		//X座標
	float fPosY;		//Y座標
	float fPosOldX;		//過去のX座標
	float fPosOldY;		//過去のY座標
	float fSpeedX;		//X座標の移動量
	float fSpeedY;		//Y座標の移動量
	float fSizeX;
	float fSizeY;
	int nHP;
	int nMutekiCnt;

	bool bUse;			//使用中フラグ
	bool bHit;			//ヒットフラグ
	bool bExplHit;
	bool bVisible;		//点滅

	ENEMYID nID;		//種類
	int nBulletArrayID;	//使っている弾配列の要素番号
	int nShotCTCnt;		//発射間隔カウンター

	ENEMYBULLET* ptE_Bullet;	//弾配列の情報


	int nDirCnt;		//方向決める際のカウンター(固定時間で方向決め直し)
	int nDirMax;
	VECTOR2 EtoP;
	int nShotCT;
	bool bJump;
	bool bLand;
};


void InitEnemy(void);
void UnInitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void CreateEnemy(float x, float y, float mx, float my, ENEMYID nID,int nBulletsArrayID);
ENEMY* GetEnemy(void);
bool* GetUsedE_Bullet(void);

void Enemy1(int nCnt);
void Enemy2(int nCnt);
void EnemyJump(int nCnt);
//void ResetEnemyBullets(int OffsetE_Bullet);
