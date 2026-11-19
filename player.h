#pragma once
#include "define.h"

#define PLAYER_START_X		25
#define PLAYER_HP_MAX		(8)						//HP最大値

#define CHARGE_MAX			(100)

enum PLAYERSTATE
{
	PS_Idle,
	PS_Dash,
};

//構造体定義
struct PLAYER
{
	float fPosX;	 // 現在のX座標
	float fPosY;	 // 現在のY座標

	float fPosOldX;	 // 過去のX座標
	float fPosOldY;	 // 過去のY座標

	float fSpeedX;	 // X座標の移動量
	float fSpeedY;	 // Y座標の移動量

	int nHP;

	VECTOR2 angle;		//キャラの向いてる方向
	float fShotAngle;	//キャラの向いてる方向(上下)
	int nShotCnt;		//連射用のカウンター
	int nBombCnt;		//連射用のカウンター

	int nJumpCnt;	//長押し判定

	bool bJump;		//ジャンプ中かどうか
	bool bUp;		//ジャンプ後上昇中かどうか
	bool bDoubleJump;	//二回ジャンプ用
	bool bLand;		//着地しているか
	bool bJumpEnd;	//ジャンプの終了判定(挙動はonceに近い)
	bool bWall;	//ジャンプの終了判定(挙動はonceに近い)
	float fWallPos;
	bool bWallUp;

	float fCharge;	//エネルギー(水量)

	bool bHit;		//ヒットフラグ
	bool bExplHit;

	int nMutekiCnt;	//無敵時間カウント
	int nRespawn;
	bool bVisible;	//表示・非表示

	float fSizeX;
	float fSizeY;

	PLAYERSTATE nState;//状態
};

//プロトタイプ宣言
void InitPlayer(void);
void UnInitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER* GetPlayer(void);//プレイヤー構造体のゲッター

void Shot(int nBulletType);