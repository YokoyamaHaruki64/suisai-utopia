#pragma once

#define FIELD_MAX 40

struct FIELD
{
	float fPosX;		// X座標
	float fPosY;		// Y座標
	float fWidth;		// 幅
	float fHight;		// 高さ
	bool bUse;			// 
	bool bFill;			//塗りつぶされたか

	float fPosOldX;		// 過去のX座標
	float fPosOldY;		// 過去のY座標
	float fSpeedX;		// X方向の移動量
	float fSpeedY;		// Y方向の移動量
	int nCount;			// カウンター
	bool bMove;			// 地面が移動しているかどうか
};

void InitField();
void UnInitField();
void UpdateField();
void DrawField();
FIELD* GetField();

void UpdateDrawField();
void CreateField(float fX, float fY,
	float fWidth, float fHight,
	float fMx, float fMy);
void FieldUpdate();

int CheckColorField(FIELD tField);
void FillColor(FIELD tField, int nMode);