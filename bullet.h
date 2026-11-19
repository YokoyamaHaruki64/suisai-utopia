#pragma once

#define BULLET_MAX (32)
#define BULLET_SPEED_X	(0.8) 
#define BULLET_SPEED_Y	(0.8) 

struct BULLET
{
	bool bUse;		 // 使用中フラグ
	float fPosX;	 // X座標
	float fPosY;	 // Y座標
	float fPosOldX;	 // 過去のX座標
	float fPosOldY;	 // 過去のY座標
	float fSpeedX;	 // X座標の移動量
	float fSpeedY;	 // Y座標の移動量

	bool bLastHit;	 // 当たった地点に表示する制御用
	int nLastHitCnt; // 当たった地点に表示する制御用(カウンター)
};

//プロトタイプ宣言
void InitBullet(void);
void UnInitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void CreateBullet(float fX,float fY,float fMx, float fMy);
void CollisionRoutine(int nCnt);

BULLET* GetBullet();