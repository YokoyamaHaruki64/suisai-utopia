#pragma once

#define EXPLOSION_FRAMERATE		5
#define EXPLOSION_FRAME			7
#define EXPLOSION_X				14
#define EXPLOSION_Y				7

#define TITLE_FRAMERATE		4
#define TITLE_FRAME			18
#define TITLE_X				80
#define TITLE_Y				25

enum ANIMETORNAME
{
	A_EcolorExpl,
	A_PcolorExpl,
	A_Title
};

struct ANIMATER
{
	int nFrameCnt;
	int nSpriteCnt;
	int nMaxFrame;
	int nFrameRate;
	int nAnimSizeX;
	int nAnimSizeY;
	float fPosX;
	float fPosY;
	bool bUse;
};

void InitAnimation();
void ReadAnim();
void UnInitAnimation();
void UpdateAnimation();
void DrawAnimation();
void CreateAnimation(int nMaxFrame, float fX, float fY, ANIMETORNAME nAnimater);
