#pragma once

#define ENEMYBULLET_MAX (8)
#define ENEMYBULLET_SPEED_X	(0.3) 
#define ENEMYBULLET_SPEED_Y	(0.3) 

enum E_BULLETTYPE
{
	NORMAL,
	EXPLOSION
};

//構造体宣言
struct ENEMYBULLET
{
	bool bUse;		 // 使用中フラグ
	E_BULLETTYPE nType;		 // 弾の種類
	float fPosX;	 // X座標
	float fPosY;	 // Y座標
	float fPosOldX;	 // 過去のX座標
	float fPosOldY;	 // 過去のY座標
	float fSpeedX;	 // X座標の移動量
	float fSpeedY;	 // Y座標の移動量

	bool bLastHit;	 //
	int nLastHitCnt; // 当たった地点に表示する制御用(カウンター)
};

//プロトタイプ宣言
void InitEnemyBullet(void);
void UnInitEnemyBullet(void);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void CreateEnemyBullet(float fX, float fY, float fMx, float fMy,E_BULLETTYPE nType,int nArrayNum);
ENEMYBULLET* GetEnemyBullet();

void CollisionRoutine(int nEnemy, int nBullet);
void RemoveEnemyBullets(int nEnemy);

void NormalBulletUpdate(int nEnemy, int nBullet);
void ExplosionBulletUpdate(int nEnemy, int nBullet);
void ResetEnemyBullets(int nBulletID);