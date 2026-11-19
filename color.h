#pragma once

struct COLOR
{
	int nRed;
	int nGreen;
	int nBlue;
};

enum COLORNAME
{
	C_Player,
	C_DiePlayer,
	C_P_GradeStart,
	C_P_GradeEnd,
	C_Enemy,
	C_SpawnerDie,
	C_E_GradeStart,
	C_E_GradeEnd,
	C_Ground,
	C_Grass,
	C_Field,
	C_SkyStart,
	C_SkyEnd,
	C_Cloud,
	C_CloudShadow,
	C_ForwardMt,
	C_BackMt,
	C_Bar,
	C_BarAcce,
	C_BarGrade,
	C_FarMt,
	C_P_Expl,
	C_P_Expl_Smoke,
	C_P_Expl_Center,
	C_E_Expl,
	C_E_Expl_Smoke,
	C_E_Expl_Center,
	C_Title_FireRed,
	C_Title_FireOrange,
	C_Title_FireYellow,
	C_Title_Rain1,
	C_Title_Rain2,
	C_Title_Rain3,
	C_Title_Logo,
	C_RainbowRed,
	C_RainbowOre,
	C_RainbowYel,
	C_RainbowGre,
	C_RainbowCyan,
	C_RainbowBlue,
	C_RainbowPur,
	C_UI_Tank_Water,
	C_UI_Tank_BG,

	COLOR_MAX
};

struct COLORLIST
{
	COLORNAME nName;
	COLOR tColor;
};

COLOR RandomColor();
COLOR RandomColorBullet();