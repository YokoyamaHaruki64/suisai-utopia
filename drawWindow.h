#pragma once

#define CONIOEX
#include "conioex.h"
#include "color.h"
#include <iostream>


#define WINDOW_X	(80)
#define WINDOW_Y	(25)

#define TEXTCOLOR	"38"
#define BACKCOLOR	"48"

enum MAPTILE
{
	BG_Sky = 0,
	BG_Cloud,
	BG_CloudShadow,
	BG_ForwardMountain,
	BG_BackMountain,
	BG_Bar,
	BG_BarAccent,
	BG_BarGradation,
	BG_FarMt,

	UI,

	TILE_Blank,
	TILE_Blankhalf,
	TILE_Player,
	TILE_PlayerUnder,
	TILE_P_Dash,
	TILE_Zenkaku,
	TILE_PlayerBullet,
	TILE_PlayerBomb,
	TILE_NormalField,
	TILE_Ground,
	TILE_Enemy1,
	TILE_Enemy2,
	TILE_Enemy2Under,
	TILE_EnemyBullet,
	TILE_EnemyBomb,
	TILE_EnemyBulletVoid,
	TILE_Spawner,
	TILE_PlayerColor,
	TILE_EnemyColor,

	ANIM_P_Expl,
	ANIM_P_Expl_Smoke,
	ANIM_P_Expl_Center,
	ANIM_E_Expl,
	ANIM_E_Expl_Smoke,
	ANIM_E_Expl_Center,

	ANIM_Title_FireR,
	ANIM_Title_FireO,
	ANIM_Title_FireY,
	ANIM_Title_Rain1,
	ANIM_Title_Rain2,
	ANIM_Title_Rain3,
	ANIM_Title_Logo,
	ANIM_RainbowRed,
	ANIM_RainbowOre,
	ANIM_RainbowYel,
	ANIM_RainbowGre,
	ANIM_RainbowCyan,
	ANIM_RainbowBlue,
	ANIM_RainbowPur,
};


//プロトタイプ宣言
void InitDrawWindow(void);
void UnInitDrawWindow(void);
void UpdateDrawWindow(void);
void DrawDrawWindow(void);
void ConvertWindowData(int y, int x, MAPTILE data);
void ConvertBackGround(int y, int x);
std::string ConvertSkyColor(int y);
void DrawUI();

std::string IntToString(int num);
std::string ColorNameToString(COLORNAME nName, std::string sMode);
std::string ColorToString(COLOR tColor, std::string sMode);
COLOR Gradetion(COLORNAME start, COLORNAME end, int divAmount, int ratio);

MAPTILE* GetDrawWindow(void);
MAPTILE* GetColorMap(void);
MAPTILE* GetUIMap(void);
MAPTILE* GetMap(void);
MAPTILE GetMapPoint(int x, int y);
MAPTILE GetMapPointOffset(int x, int y);

COLOR GetColor(COLORNAME nName);
void SetBackGroundMap(int x, int y, int nData);
void SetMap(int x, int y, MAPTILE nData);
void SetMapOffset(int x, int y, MAPTILE nData);
void SetColorMapOffset(int x, int y, MAPTILE nData);

int OffsetMapPointer(int x, int y);
int OffsetMapPointerF(float x, float y);
