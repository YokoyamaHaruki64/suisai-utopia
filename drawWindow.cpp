#include "player.h"
#include "game.h"
#include <iostream>
#include "drawWindow.h"
#include "UI.h"
#include "define.h"
#include "result.h"
#include "scene.h"

#include "enemySpawn.h"

#define CAMERA_MINUS		39
#define CAMERA_PLUS			40

#define GRADETION_DIVISION	25		//グラデの分割数
#define GRADETION_FREQ		10		//グラデの更新フレーム数

#define RESULT_MOVECAMERA_X	0.5
#define DOUBLEBUFFER

struct WINDOW
{
	std::string textColor;
	std::string backColor;
	std::string chara;
};

COLORLIST g_tColorList[COLOR_MAX]
{
	{C_Player,			{0,	  100, 255}},
	{C_DiePlayer,		{0,	  0  , 0  }},
	{C_P_GradeStart,	{0  , 127, 255}},
	{C_P_GradeEnd,		{110, 180, 255}},
	{C_Enemy,			{255, 90 , 0  }},
	{C_SpawnerDie,		{180,	  0 ,  0  }},
	{C_E_GradeStart,	{255, 106, 0  }},
	{C_E_GradeEnd,		{255, 164, 100}},
	{C_Ground,			{140, 80 , 40 }},
	{C_Grass,			{60 , 190, 70 }},
	{C_Field,			{255, 255, 255}},
	{C_SkyStart,		{120, 190, 255}},
	{C_SkyEnd,			{190, 240, 255}},
	{C_Cloud,			{255, 255, 255}},
	{C_CloudShadow,		{217, 217, 217}},
	{C_ForwardMt,		{126, 235, 111}},
	{C_BackMt,			{183, 243, 149}},
	{C_Bar,				{244, 220, 116}},
	{C_BarAcce,			{237, 197, 27 }},
	{C_BarGrade,		{240, 210, 77 }},
	{C_FarMt,			{100, 180, 180}},
	{C_P_Expl,			{255, 90 , 0  }},
	{C_P_Expl_Smoke,	{255, 179, 138}},
	{C_P_Expl_Center,	{160, 60 , 5  }},
	{C_E_Expl,			{0  , 212, 255}},
	{C_E_Expl_Smoke,	{192, 233, 255}},
	{C_E_Expl_Center,	{7  , 152, 173}},
	{C_Title_FireRed,	{255, 0  , 0  }},
	{C_Title_FireOrange,{255, 90 , 0  }},
	{C_Title_FireYellow,{255, 255, 0  }},
	{C_Title_Rain1,		{85 , 240, 255}},
	{C_Title_Rain2,		{128, 234, 255}},
	{C_Title_Rain3,		{150, 240, 255}},
	{C_Title_Logo,		{30,  100, 255}},
	{C_RainbowRed,		{249, 215, 215}},
	{C_RainbowOre,		{255, 233, 220}},
	{C_RainbowYel,		{255, 254, 209}},
	{C_RainbowGre,		{225, 255, 222}},
	{C_RainbowCyan,		{218, 254, 255}},
	{C_RainbowBlue,		{212, 218, 255}},
	{C_RainbowPur,		{232, 207, 255}},
	{C_UI_Tank_Water,	{134, 236, 249}},
	{C_UI_Tank_BG,		{180, 180, 180}},
};

MAPTILE g_nWindowData[WINDOW_Y][WINDOW_X];
MAPTILE g_nMapData[WINDOW_Y][MAP_X];
MAPTILE g_nOldWindowData[WINDOW_Y][WINDOW_X];
MAPTILE g_nColorMap[WINDOW_Y][MAP_X];	//構造体に変えて浸透度
MAPTILE g_nUIMap[WINDOW_Y][WINDOW_X];
MAPTILE g_nWindowBG[WINDOW_Y][WINDOW_X];
int g_nBGData[WINDOW_Y][MAP_X];

WINDOW g_tWindow[WINDOW_Y][WINDOW_X];

COLOR g_baseSkyColor = {70,140,255};
COLOR g_baseSkyColor2 = {180,230,255};

float g_CameraPosX;
float g_CameraOldPosX;
int g_nGradeCnt;
int g_nFlameCnt;
bool g_bGradeCntUp;
SCENE g_NowScene;
bool Zenkaku;

void InitDrawWindow(void)
{
	g_nFlameCnt = 0;
	g_nGradeCnt = 0;
	g_bGradeCntUp = true;
	g_NowScene = GetScene();

	switch (g_NowScene)
	{
	case SCENE_TITLE:
		memset(&g_nMapData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		memset(&g_nBGData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		break;

	case SCENE_GAME:
		memset(&g_nMapData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		memset(&g_nBGData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		for (int y = 0; y < WINDOW_Y; y++)
		{
			for (int x = 0; x < WINDOW_X; x++)
			{
				g_nWindowData[y][x] = TILE_Blank;
				g_nOldWindowData[y][x] = TILE_Blank;
			}
		}

		for (int y = 0; y < WINDOW_Y; y++)
		{
			for (int x = 0; x < MAP_X; x++)
			{
				g_nMapData[y][x] = TILE_Blank;
			}
		}
		memset(&g_nColorMap[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		memset(&g_nUIMap[0][0], 0, sizeof(int) * WINDOW_Y * WINDOW_X);

		ReadCSV(MAP_X, WINDOW_Y, &g_nBGData[0][0], "BackGround.csv");

		//for (int y = 0; y < WINDOW_Y; y++)
		//{
		//	for (int x = 0; x < MAP_X; x++)
		//	{
		//		if (RandomRange(0, 80) == 0)
		//		{
		//			g_nColorMap[y - 1][x - 2] = TILE_EnemyColor;
		//			g_nColorMap[y - 1][x - 1] = TILE_EnemyColor;
		//			g_nColorMap[y - 1][  x  ] = TILE_EnemyColor;
		//			g_nColorMap[y - 1][x + 1] = TILE_EnemyColor;
		//			g_nColorMap[y - 1][x + 2] = TILE_EnemyColor;
		//			g_nColorMap[y - 1][x + 3] = TILE_EnemyColor;

		//			g_nColorMap[y][x - 2] = TILE_EnemyColor;
		//			g_nColorMap[y][x - 1] = TILE_EnemyColor;
		//			g_nColorMap[y][  x  ] = TILE_EnemyColor;
		//			g_nColorMap[y][x + 1] = TILE_EnemyColor;
		//			g_nColorMap[y][x + 2] = TILE_EnemyColor;
		//			g_nColorMap[y][x + 3] = TILE_EnemyColor;

		//			g_nColorMap[y + 1][x - 2] = TILE_EnemyColor;
		//			g_nColorMap[y + 1][x - 1] = TILE_EnemyColor;
		//			g_nColorMap[y + 1][  x  ] = TILE_EnemyColor;
		//			g_nColorMap[y + 1][x + 1] = TILE_EnemyColor;
		//			g_nColorMap[y + 1][x + 2] = TILE_EnemyColor;
		//			g_nColorMap[y + 1][x + 3] = TILE_EnemyColor;
		//		}
		//	}
		//}

		break;

	case SCENE_PRACTICE:
		memset(&g_nMapData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		memset(&g_nBGData[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		for (int y = 0; y < WINDOW_Y; y++)
		{
			for (int x = 0; x < WINDOW_X; x++)
			{
				g_nWindowData[y][x] = TILE_Blank;
				g_nOldWindowData[y][x] = TILE_Blank;
			}
		}

		for (int y = 0; y < WINDOW_Y; y++)
		{
			for (int x = 0; x < MAP_X; x++)
			{
				g_nMapData[y][x] = TILE_Blank;
			}
		}
		memset(&g_nColorMap[0][0], 0, sizeof(int) * WINDOW_Y * MAP_X);
		memset(&g_nUIMap[0][0], 0, sizeof(int) * WINDOW_Y * WINDOW_X);

		ReadCSV(MAP_X, WINDOW_Y, &g_nBGData[0][0], "BackGround.csv");
		break;

	case SCENE_RESULT:
		break;
	case SCENE_NUM:
		break;
	default:
		break;
	}

	g_CameraPosX = g_CameraOldPosX = PLAYER_START_X;
	for (int y = 0; y < WINDOW_Y; y++)
	{
		int x_StartZero = 0;
		for (int x = g_CameraPosX - CAMERA_MINUS; x < g_CameraPosX + CAMERA_PLUS; x++, x_StartZero++)
		{
			g_nWindowData[y][x_StartZero] = g_nMapData[y][x];
			g_nWindowBG[y][x_StartZero] = (MAPTILE)g_nBGData[y][x];
		}
	}

	//最初の描画
	std::string sOutput;
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = 0; x < WINDOW_X; x++)
		{
			//oldへの保存
			g_nOldWindowData[y][x] = g_nWindowData[y][x];

			ConvertWindowData(y, x, g_nWindowData[y][x]);
			//カーソル移動
			sOutput += "\x1b[" + IntToString(y + 1) + ";" + IntToString(x + 1) + "H";
			//文字色と背景色を設定
			sOutput += g_tWindow[y][x].textColor;
			sOutput += g_tWindow[y][x].backColor;
			//文字
			sOutput += g_tWindow[y][x].chara;
		}
	}
	//最後にカーソルを左上に移動させる
	sOutput += "\x1b[1;1H";
	std::cout << sOutput;
}

void UnInitDrawWindow(void)
{
}

void UpdateDrawWindow(void)
{
	g_NowScene = GetScene();

	for (int x = 0; x < 100; x++)
	{
		if (g_nMapData[0][x] == ANIM_Title_Logo)
		{
			std::cout << "";
		}
	}


	//色マップとの合成
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = 0; x < MAP_X; x++)
		{
			//全探索
			if (g_nColorMap[y][x] != 0 && (g_nMapData[y][x] == TILE_NormalField ||
				g_nMapData[y][x] == TILE_PlayerColor ||
				g_nMapData[y][x] == TILE_EnemyColor))
			{//色が登録されていてかつ、タイルが特定の部分のみ置き換え
				g_nMapData[y][x] = g_nColorMap[y][x];
			}
		}
	}


	if (g_NowScene == SCENE_GAME || g_NowScene == SCENE_PRACTICE)
	{
		g_CameraPosX = GetPlayer()->fPosX - 1;	//配列の番号に対応した数値にしながら取得
	}
	else if(g_NowScene == SCENE_RESULT)
	{
		//仮想プレイヤーとして動かす
		g_CameraPosX += RESULT_MOVECAMERA_X;
		if (g_CameraPosX > MAP_X - 2)
		{
			g_CameraPosX = MAP_X - 2;
			SetResultEnd(true);
		}
	}
	else
	{
		g_CameraPosX = 1;
	}

	bool isScroll = true;

	if (g_CameraPosX < CAMERA_MINUS || g_CameraPosX > MAP_X - CAMERA_PLUS)
	{
		isScroll = false;
	}


	if (isScroll)
	{
		g_CameraOldPosX = g_CameraPosX;
		for (int y = 0; y < WINDOW_Y; y++)
		{
			int x_StartZero = 0;
			for (int x = g_CameraPosX - CAMERA_MINUS; x < g_CameraPosX + CAMERA_PLUS + 1; x++, x_StartZero++)
			{	
				g_nWindowData[y][x_StartZero] = g_nMapData[y][x];
				g_nWindowBG[y][x_StartZero] = (MAPTILE)g_nBGData[y][x];//リリースにするとバグる
			}
		}
	}
	else if (g_CameraPosX < CAMERA_MINUS)
	{//左端の描画
		g_CameraOldPosX = g_CameraPosX;
		for (int y = 0; y < WINDOW_Y; y++)
		{
			for (int x = 0; x < WINDOW_X; x++)
			{
				g_nWindowData[y][x] = g_nMapData[y][x];
				g_nWindowBG[y][x] = (MAPTILE)g_nBGData[y][x];
			}
		}
	}
	else
	{//右端の描画
		g_CameraOldPosX = g_CameraPosX;
		for (int y = 0; y < WINDOW_Y; y++)
		{
			int x_StartZero = 0;
			for (int x = MAP_X - WINDOW_X; x < MAP_X; x++, x_StartZero++)
			{
				g_nWindowData[y][x_StartZero] = g_nMapData[y][x];
				g_nWindowBG[y][x_StartZero] = (MAPTILE)g_nBGData[y][x];
			}
		}
	}

	//右端描画すると左端に残るやつの対策
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = WINDOW_X - 2; x < WINDOW_X; x++)
		{
			switch (g_nWindowData[y][x])
			{
			case TILE_Enemy1:
				g_nWindowData[y][x] = TILE_Blank;

			case TILE_Enemy2:
				g_nWindowData[y][x] = TILE_Blank;
				break;
			}
		}
	}


	//背景との合成と各キャラクターの配置をここで行う
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = 0; x < WINDOW_X; x++)
		{
			switch (g_nWindowData[y][x])
			{//2マス以上のキャラ配置
			case TILE_Player:
				g_nWindowData[y + 1][x] = TILE_PlayerUnder;
				g_nWindowData[y + 1][x + 1] = TILE_Zenkaku;
				break;

			case TILE_Enemy2:
				if (g_nWindowData[y + 1][x] != TILE_P_Dash &&
					g_nWindowData[y + 1][x + 1] != TILE_P_Dash)
				{
					g_nWindowData[y + 1][x] = TILE_Enemy2Under;
					g_nWindowData[y + 1][x + 1] = TILE_Zenkaku;
				}
				break;

			default:
				break;
			}

			if (g_nWindowData[y][x] == TILE_Blank)
			{
				g_nWindowData[y][x] = g_nWindowBG[y][x];
			}

			if (g_NowScene == SCENE_GAME || g_NowScene == SCENE_PRACTICE)
			{	//UIの部分を描画から外す
				if (y >= UI_TANK_POSY -2 && x >= UI_TANK_POSX -1)
				{
					if (y < UI_TANK_POSY - 2 + 6 && x < UI_TANK_POSX - 1 + 5)
					{
						g_nWindowData[y][x] = UI;
					}
				}

				if (x == WINDOW_X - 1)
				{//端っこの描画をしない
					if (g_nWindowData[y][x] == TILE_Spawner)
					{
						g_nWindowData[y][x] = TILE_Blank;
					}
				}
			}

			if (g_nFlameCnt % 60 == 0)
			{//カス残った時のリセット
				if (g_nWindowData[y][x] == TILE_EnemyBullet)
				{
					g_nWindowData[y][x] = TILE_Blank;
				}
			}
		}
	}
	if (g_NowScene == SCENE_GAME ||
		g_NowScene == SCENE_RESULT ||
		g_NowScene == SCENE_PRACTICE)
	{//UIの部分を描画から外す(タンク以外)
		for (int x = 0; x < UI_BAR_LENGTH * 2 + 2; x++)
		{
			g_nWindowData[UI_BAR_POSY][UI_BAR_POSX + x] = UI;
		}
	}

	//グラデーションのカウント
	g_nFlameCnt++;
	if (g_nFlameCnt % GRADETION_FREQ == 0)
	{
		if (g_bGradeCntUp)
		{
			g_nGradeCnt++;
		}
		else
		{
			g_nGradeCnt--;
		}

		if (g_bGradeCntUp && g_nGradeCnt == GRADETION_DIVISION)
		{
			g_bGradeCntUp = false;
		}
		else if (!g_bGradeCntUp && g_nGradeCnt == 0)
		{
			g_bGradeCntUp = true;
		}
	}

}

void DrawDrawWindow(void)
{
#ifdef DOUBLEBUFFER
	std::string sOutput;
	bool draw = false;
	for (int y = 0; y < WINDOW_Y; y++)
	{
		for (int x = 0; x < WINDOW_X; x++)
		{
			//描画更新判定
			if (g_nWindowData[y][x] != g_nOldWindowData[y][x])
			{//描画更新

				if (g_nWindowData[y][x] != TILE_PlayerColor ||	//ダブルバッファとは別のタイミングで更新するタイルをここに登録する
					g_nWindowData[y][x] != TILE_EnemyColor ||
					g_nWindowData[y][x] != TILE_Player ||
					g_nWindowData[y][x] != TILE_PlayerUnder ||
					g_nWindowData[y][x] != TILE_Spawner ||
					g_nWindowData[y][x] != UI)
				{
					draw = true;
					//oldへの保存
					g_nOldWindowData[y][x] = g_nWindowData[y][x];

					ConvertWindowData(y, x, g_nWindowData[y][x]);
					//カーソル移動
					sOutput += "\x1b[" + IntToString(y + 1) + ";" + IntToString(x + 1) + "H";
					//文字色と背景色を設定
					sOutput += g_tWindow[y][x].textColor;
					sOutput += g_tWindow[y][x].backColor;
					//文字
					sOutput += g_tWindow[y][x].chara;
				}
			}

			//一部は毎回更新
			if (g_nWindowData[y][x] == TILE_Player ||
				g_nWindowData[y][x] == TILE_PlayerUnder ||
				g_nWindowData[y][x] == TILE_P_Dash ||
				g_nWindowData[y][x] == TILE_Spawner)
			{
				draw = true;
				ConvertWindowData(y, x, g_nWindowData[y][x]);
				//カーソル移動
				sOutput += "\x1b[" + IntToString(y + 1) + ";" + IntToString(x + 1) + "H";
				//文字色と背景色を設定
				sOutput += g_tWindow[y][x].textColor;
				sOutput += g_tWindow[y][x].backColor;
				//文字
				sOutput += g_tWindow[y][x].chara;
			}

			if (g_nFlameCnt % GRADETION_FREQ == 0)
			{
				if (g_nWindowData[y][x] == TILE_PlayerColor || g_nWindowData[y][x] == TILE_EnemyColor)
				{//カラーマップはグラデ用に全部更新
					draw = true;
					ConvertWindowData(y, x, g_nWindowData[y][x]);
					//カーソル移動
					sOutput += "\x1b[" + IntToString(y + 1) + ";" + IntToString(x + 1) + "H";
					//文字色と背景色を設定
					sOutput += g_tWindow[y][x].textColor;
					sOutput += g_tWindow[y][x].backColor;
					//文字
					sOutput += g_tWindow[y][x].chara;
				}
			}
		}
	}
	if (draw)
	{
		//最後にカーソルを左上に移動させて空タイルを描画する(バグ対策)
		sOutput += "\x1b[1;1H";
		sOutput += ConvertSkyColor(0);
		sOutput += " ";

		std::cout << sOutput;
	}

#else	//ダブルバッファオフ

	std::cout << "\x1b[1;1H";

	for (int y = 0; y < WINDOW_Y; y++)
	{
		std::string sOutput;
		for (int x = 0; x < WINDOW_X; x++)
		{
			Zenkaku = false;
			//oldへの保存
			//g_nOldWindowData[y][x] = g_nWindowData[y][x];

			ConvertWindowData(y, x, g_nWindowData[y][x]);
			//カーソル移動
			//sOutput += "\x1b[" + IntToString(y + 1) + ";" + IntToString(x + 1) + "H";
			//文字色と背景色を設定
			sOutput += g_tWindow[y][x].textColor;
			sOutput += g_tWindow[y][x].backColor;
			//文字
			sOutput += g_tWindow[y][x].chara;

			//全角は二文字分
			if (Zenkaku)
			{
				x++;
			}
		}
		if (y < WINDOW_Y - 1)
		{
			sOutput += "\n";
		}
		std::cout << sOutput;

	}

	////最後にカーソルを左上に移動させる
	//sOutput += "\x1b[1;1H";
	//std::cout << sOutput;

#endif // DOUBLUBUFFER
}

void ConvertWindowData(int y, int x, MAPTILE data)
{
	SPAWNER* ptSpawner = GetSpawner();//スポナー用に取得


	//マップタイルに応じた色と文字を設定
	switch (data)
	{
	case TILE_Blank:
		ConvertBackGround(y,x);
		g_tWindow[y][x].chara = " ";							//表示する文字
		break;

	case TILE_Blankhalf:
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = " ";							//表示する文字
		break;

	case TILE_Player:
		if (GetPlayer()->nRespawn == 0)
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_Player, TEXTCOLOR);		//文字色のコード
		}
		else
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_DiePlayer, TEXTCOLOR);		//文字色のコード
		}
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25CF";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_P_Dash:
		if (GetPlayer()->nRespawn == 0)
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_Player, TEXTCOLOR);		//文字色のコード
		}
		else
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_DiePlayer, TEXTCOLOR);		//文字色のコード
		}

		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25CF";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_PlayerUnder:
		if (GetPlayer()->nRespawn == 0)
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_Player, TEXTCOLOR);		//文字色のコード
		}
		else
		{
			g_tWindow[y][x].textColor = ColorNameToString(C_DiePlayer, TEXTCOLOR);		//文字色のコード
		}
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25A0";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_PlayerBullet:
		//g_tWindow[y][x].textColor = ColorNameToString(C_Player,TEXTCOLOR);		//文字色のコード
		g_tWindow[y][x].textColor = ColorNameToString(C_Player(),TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25C9";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_PlayerBomb:
		//g_tWindow[y][x].textColor = ColorNameToString(C_Player,TEXTCOLOR);		//文字色のコード
		g_tWindow[y][x].textColor = ColorNameToString(C_Player(),TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25CF";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_NormalField:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";			//文字色のコード
		g_tWindow[y][x].backColor = "\x1b[48;2;255;255;255m";	//背景色のコード
		g_tWindow[y][x].chara = " ";							//表示する文字
		break;

	case TILE_Ground:
		g_tWindow[y][x].textColor = ColorNameToString(C_Ground, TEXTCOLOR);			//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Grass,BACKCOLOR);	//背景色のコード
		g_tWindow[y][x].chara = u8"\u2586";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_Enemy1:
		g_tWindow[y][x].textColor = ColorNameToString(C_Enemy, TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25C6";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_Enemy2:
		g_tWindow[y][x].textColor = ColorNameToString(C_Enemy, TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25C6";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_Enemy2Under:
		g_tWindow[y][x].textColor = ColorNameToString(C_Enemy, TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25C6";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_EnemyBullet:
		g_tWindow[y][x].textColor = ColorNameToString(C_Enemy, TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u2B22";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_Spawner:

		for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++,ptSpawner++)
		{
			if (ptSpawner->bUse)
			{
				break;
			}
		}

		g_tWindow[y][x].textColor = ColorToString(Gradetion(C_Enemy,C_SpawnerDie,ptSpawner->nMaxHP,ptSpawner->nMaxHP + 1 - ptSpawner->nHP), TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u25A0";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_EnemyBomb:
		g_tWindow[y][x].textColor = ColorNameToString(C_Enemy, TEXTCOLOR);		//文字色のコード
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u2B24";						//表示する文字
		Zenkaku = true;
		break;

	case TILE_PlayerColor:
		g_tWindow[y][x].backColor = ColorToString(Gradetion(C_P_GradeStart,C_P_GradeEnd,GRADETION_DIVISION,g_nGradeCnt),BACKCOLOR);		//背景色のコード
		g_tWindow[y][x].chara = " ";							//表示する文字
		break;

	case TILE_EnemyColor:
		g_tWindow[y][x].backColor = ColorToString(Gradetion(C_E_GradeStart, C_E_GradeEnd, GRADETION_DIVISION, g_nGradeCnt), BACKCOLOR);		//背景色のコード
		g_tWindow[y][x].chara = " ";							//表示する文字
		break;

	case ANIM_P_Expl:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_P_Expl,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_P_Expl_Smoke:
		g_tWindow[y][x].textColor = ColorNameToString(C_P_Expl_Smoke, TEXTCOLOR);
		ConvertBackGround(y, x);
		g_tWindow[y][x].chara = u8"\u2592";					//表示する文字
		break;

	case ANIM_P_Expl_Center:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_P_Expl_Center,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_E_Expl:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_E_Expl,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_E_Expl_Smoke:
		g_tWindow[y][x].textColor = ColorNameToString(C_E_Expl_Smoke, TEXTCOLOR);
		ConvertBackGround(y,x);
		g_tWindow[y][x].chara = u8"\u2592";					//表示する文字
		break;

	case ANIM_E_Expl_Center:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_E_Expl_Center,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_FireR:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_FireRed,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_FireO:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_FireOrange,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_FireY:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_FireYellow,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_Rain1:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_Rain1,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_Rain2:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_Rain2,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_Rain3:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_Rain3,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_Title_Logo:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_Title_Logo,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowRed:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowRed,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowOre:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowOre,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowYel:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowYel,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowGre:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowGre,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowCyan:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowCyan,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowBlue:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowBlue,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case ANIM_RainbowPur:
		//g_tWindow[y][x].textColor = "\x1b[38;2;0;0;0m";	//文字色のコード
		g_tWindow[y][x].backColor = ColorNameToString(C_RainbowPur,BACKCOLOR);
		g_tWindow[y][x].chara = " ";					//表示する文字
		break;

	case TILE_Zenkaku:
		g_tWindow[y][x].chara = "";
		break;

	default:
		ConvertBackGround(y,x);
		g_tWindow[y][x].chara = " ";//BG系のタイル全般
	}

}

void ConvertBackGround(int y,int x)
{
	switch (g_nWindowBG[y][x])
	{
	case BG_Sky:
		g_tWindow[y][x].backColor = ConvertSkyColor(y);	//背景色のコード
		break;

	case BG_Cloud:
		g_tWindow[y][x].backColor = ColorNameToString(C_Cloud,BACKCOLOR);	//背景色のコード
		break;

	case BG_CloudShadow:
		g_tWindow[y][x].backColor = ColorNameToString(C_CloudShadow,BACKCOLOR);	//背景色のコード
		break;

	case BG_ForwardMountain:
		g_tWindow[y][x].backColor = ColorNameToString(C_ForwardMt, BACKCOLOR);	//背景色のコード
		break;

	case BG_BackMountain:
		g_tWindow[y][x].backColor = ColorNameToString(C_BackMt, BACKCOLOR);	//背景色のコード
		break;

	case BG_Bar:
		g_tWindow[y][x].backColor = ColorNameToString(C_Bar, BACKCOLOR);	//背景色のコード
		break;

	case BG_BarAccent:
		g_tWindow[y][x].backColor = ColorNameToString(C_BarAcce, BACKCOLOR);	//背景色のコード
		break;

	case BG_BarGradation:
		g_tWindow[y][x].backColor = ColorNameToString(C_BarGrade, BACKCOLOR);	//背景色のコード
		break;

	case BG_FarMt:
		g_tWindow[y][x].backColor = ColorNameToString(C_FarMt, BACKCOLOR);	//背景色のコード
		break;


	default:
		break;
	}
}

std::string ConvertSkyColor(int y)
{
	COLOR gradeStart = GetColor(C_SkyStart);
	COLOR gradeEnd = GetColor(C_SkyEnd);
	COLOR skyColor = gradeStart;
	skyColor.nRed += (int)((gradeEnd.nRed - gradeStart.nRed) / WINDOW_Y) * (y - 1);
	skyColor.nGreen += (int)((gradeEnd.nGreen - gradeStart.nGreen) / WINDOW_Y) * (y - 1);

	return ColorToString(skyColor,BACKCOLOR);
}

COLOR Gradetion(COLORNAME start,COLORNAME end,int divAmount,int ratio)
{
	COLOR tStartColor = GetColor(start);
	COLOR tEndColor = GetColor(end);
	COLOR ret = tStartColor;
	ret.nRed += (int)((tEndColor.nRed - tStartColor.nRed) / divAmount) * ratio;
	ret.nGreen += (int)((tEndColor.nGreen - tStartColor.nGreen) / divAmount) * ratio;
	ret.nBlue += (int)((tEndColor.nBlue - tStartColor.nBlue) / divAmount) * ratio;

	return ret;
}

std::string ColorNameToString(COLORNAME nName,std::string sMode)
{
	COLOR tColor = GetColor(nName);
	return	"\x1b[" + sMode + ";2;" + IntToString(tColor.nRed) + ";" + IntToString(tColor.nGreen) + ";" + IntToString(tColor.nBlue) + "m";
}

std::string ColorToString(COLOR tColor,std::string sMode)
{
	return	"\x1b[" + sMode + ";2;" + IntToString(tColor.nRed) + ";" + IntToString(tColor.nGreen) + ";" + IntToString(tColor.nBlue) + "m";
}

MAPTILE* GetDrawWindow(void)
{
	return &g_nWindowData[0][0];
}

MAPTILE* GetMap(void)
{
	return &g_nMapData[0][0];
}

MAPTILE GetMapPoint(int x,int y)
{
	return g_nMapData[y][x];
}

MAPTILE GetMapPointOffset(int x,int y)
{
	return g_nMapData[y-1][x-1];
}

MAPTILE* GetColorMap(void)
{
	return &g_nColorMap[0][0];
}

MAPTILE* GetUIMap(void)
{
	return &g_nUIMap[0][0];
}

COLOR GetColor(COLORNAME nName)
{
	return g_tColorList[nName].tColor;
}

void SetBackGroundMap(int x,int y,int nData)
{
	g_nBGData[y][x] = nData;
}

std::string IntToString(int num)
{
	std::string ret;
	int nDigit = log10(num) + 1;//whileでワンチャン軽量化
	int nDigitNum[256] = {};
	int nWork = 1;

	if (num <= 0)
	{
		nDigit = 1;
	}

	//作業用変数の作成
	for (int nCnt = 0; nCnt < nDigit - 1; nCnt++)
	{
		nWork *= 10;
	}

	for (int nCnt = 0; nCnt < nDigit; nCnt++)
	{
		nDigitNum[nCnt] = num / nWork % 10;
		ret += nDigitNum[nCnt] + 48;
		nWork /= 10;
	}

	return ret;
}

void SetMap(int x,int y,MAPTILE nData)
{
	g_nMapData[y][x] = nData;
}

void SetMapOffset(int x,int y,MAPTILE nData)
{
	g_nMapData[y - 1][x - 1] = nData;
}

void SetColorMapOffset(int x,int y,MAPTILE nData)
{
	g_nColorMap[y - 1][x - 1] = nData;
}

int OffsetMapPointer(int x, int y)
{
	//ポインタを二次元配列のように扱うための計算(-1はカーソル位置と配列を合わせるため)
	return (y - 1) * MAP_X + (x - 1);

}

int OffsetMapPointerF(float x, float y)
{
	//ポインタを二次元配列のように扱うための計算(-1はカーソル位置と配列を合わせるため)
	return ((int)y - 1) * MAP_X + ((int)x - 1);

}