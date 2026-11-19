#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "drawWindow.h"
#include "define.h"
#include "UI.h"
#include "scene.h"
#include "player.h"
#include "sound.h"
#include <iostream>

#define UI_BAR_COUNTMAX 1000



int g_nColorCount[2];
MAPTILE* g_pnPaintMap;

int nTime;
int nStartTime;
int nTimeLimit;
SCENE g_NowScene_UI;
extern COLORLIST g_tColorList[COLOR_MAX];

void InitUI()
{
	g_pnPaintMap = GetMap();
	nTime = 0;
	nStartTime = timeGetTime();
}

void UnInitUI()
{
}

void UpdateUI()
{
	g_NowScene_UI = GetScene();

	memset(g_nColorCount,0,sizeof(int) * 2);
	//色ブロックのカウント
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			//全探索
			if (g_pnPaintMap[y * MAP_X + x] != 0)
			{//色が登録されている部分をカウント
				switch (g_pnPaintMap[y * MAP_X + x])
				{
				case TILE_PlayerColor:
					g_nColorCount[0]++;
					break;

				case TILE_EnemyColor:
					g_nColorCount[1]++;
					break;

				default:
					break;
				}
			}
		}
	}

	//残り時間を算出
	nTime = timeGetTime();
	nTimeLimit = TIMELIMIT - (nTime - nStartTime) / 1000;

	if (nTimeLimit == -1 && g_NowScene_UI == SCENE_GAME)
	{
		SoundEnd(BGM);
		SoundPlay(SE_GAMEEND,NOTLOOP);

		msleep(2000);
		SetScene(SCENE_RESULT);
	}
}

void DrawUI()
{
	std::string sOutput;

	switch (g_NowScene_UI)
	{
	case SCENE_TITLE:
		break;

	case SCENE_GAME:
		sOutput += "" + StringVSBar() + StringTime() + StringCharge() + StringHPBar();
		break;

	case SCENE_RESULT:
		sOutput += "" + StringVSBar();
		break;

	case SCENE_PRACTICE:
		sOutput += "" + StringVSBar() + StringCharge() + StringHPBar();
		break;

	case SCENE_NUM:
		break;

	default:
		break;
	}

	std::cout << sOutput;
}

std::string StringVSBar()
{
	std::string sRet;
	float nWork;

	//0～上限値に値を丸める
	//その値が上限値に対しどの位置かを割合で出し、バーの最大長にかけてバーの長さを算出
	sRet += "\x1b[2;14H";			//UI開始位置
	sRet += ConvertSkyColor(1);		//背景色
	for (int nCnt = 0; nCnt < UI_BAR_LENGTH * 2 + 2; nCnt++)
	{//前フレームのリセット
		sRet += " ";
	}

	sRet += "\x1b[2;14H";

	//味方
	sRet += ColorNameToString(C_Player, TEXTCOLOR);	//文字色
	sRet += "\x1b[48;2;0;0;0m";	//文字色
	nWork = ClampInt(0, UI_BAR_COUNTMAX, g_nColorCount[0]);
	nWork /= UI_BAR_COUNTMAX;
	nWork = nWork * UI_BAR_LENGTH;
	//空白の表示
	for (int nCnt = 0; nCnt < UI_BAR_LENGTH - (int)nWork; nCnt++)
	{
		sRet += " ";
	}

	//バーの表示
	sRet += ColorNameToString(C_Player, BACKCOLOR);		//背景色
	for (int nCnt = 0; nCnt < (int)nWork; nCnt++)
	{
		sRet += " ";
	}

	//間
	sRet += "\x1b[48;2;0;0;0m";	//文字色
	sRet += "v";
	sRet += "\x1b[38;2;255;90;0m";	//文字色
	sRet += "s";
	sRet += ColorNameToString(C_Enemy, BACKCOLOR);		//背景色

	//敵
	nWork = ClampInt(0, UI_BAR_COUNTMAX, g_nColorCount[1]);
	nWork /= UI_BAR_COUNTMAX;
	nWork = nWork * UI_BAR_LENGTH;
	//空白の表示
	for (int nCnt = 0; nCnt < UI_BAR_LENGTH; nCnt++)
	{
		if (nCnt == (int)nWork)
		{
			sRet += "\x1b[48;2;0;0;0m";	//背景色
		}
		sRet += " ";
	}

	sRet += "\x1b[38;2;255;255;255m";	//文字色
	sRet += "\x1b[48;2;0;0;0m";	//文字色
	//sRet += ConvertSkyColor(1);		//背景色

	return sRet;
}

std::string StringTime()
{
	SCENE nowScene = GetScene();
	std::string sRet;

	sRet += "\x1b[1;36H";				//UI開始位置
	sRet += ConvertSkyColor(0);			//背景色
	sRet += "\x1b[38;2;255;0;255m";		//文字色

	if (nowScene == SCENE_GAME)
	{
		sRet += "<" + IntToString(nTimeLimit / 60) + ":";

		if (nTimeLimit % 60 < 10)
		{//一桁
			sRet += "0" + IntToString(nTimeLimit % 60) + ">";
		}
		else
		{
			sRet += IntToString(nTimeLimit % 60) + ">";
		}
	}
	else if (nowScene == SCENE_RESULT)
	{
		sRet += "<0:00>";
	}
	return sRet;
}

std::string StringCharge()
{
	std::string ret = "";
	ret +="\x1b[" + IntToString(UI_TANK_POSY - 1) + ";" + IntToString(UI_TANK_POSX) + "H";
	ret += ColorNameToString(C_Player,BACKCOLOR);;
	ret += "\x1b[38;2;255;255;255m";
	ret += "WATER";

	PLAYER* ptPlayer = GetPlayer();

	//チャージ残量の割合を計算(百分率)
	int fRatio = (float)(ptPlayer->fCharge / CHARGE_MAX) * 100;

	//割合をブロック数(25)に変換
	fRatio /= 4;
	int nCnt = 0;
	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			nCnt++;
			if (25 - fRatio >= nCnt)
			{
				ret += "\x1b[" + IntToString(UI_TANK_POSY + y) + ";" + IntToString(UI_TANK_POSX + x) + "H";
				ret += ColorNameToString(C_UI_Tank_BG, BACKCOLOR);
				ret += " ";
			}
			else
			{
				ret += "\x1b[" + IntToString(UI_TANK_POSY + y) + ";" + IntToString(UI_TANK_POSX + x) + "H";
				ret += ColorNameToString(C_UI_Tank_Water, BACKCOLOR);
				ret += " ";
			}
		}
	}

	return ret;

}

std::string StringHPBar()
{
	std::string ret = "";
	COLOR work = {255,255,255};
	int HP = GetPlayer()->nHP;

	ret += "\x1b[25;60H";

	ret += ColorNameToString(C_Player,BACKCOLOR);
	ret += ColorToString(work,TEXTCOLOR) + "HP";

	work = { 0,255,0 };
	ret += ColorToString(work,BACKCOLOR);
	for (int nCnt = 0; nCnt < HP; nCnt++)
	{
		ret += "  ";
	}

	work = {255,0,0};
	ret += ColorToString(work,BACKCOLOR);
	for (int nCnt = 0; nCnt < PLAYER_HP_MAX - HP; nCnt++)
	{
		ret += "  ";
	}

	return ret;
}

int GetTimeLimit()
{
	return nTimeLimit;
}

int GetGameTime()
{
	return (nTime - nStartTime) / 1000;
}

int* GetColorCount()
{
	return g_nColorCount;
}
