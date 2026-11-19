#include "drawWindow.h"
#include "UI.h"
#include "define.h"
#include "animation.h"
#include "game.h"
#include "sound.h"
#include "player.h"

#define EXPL_PCOLOR_ANIMATER_MAX		5
#define EXPL_ECOLOR_ANIMATER_MAX		10

#define LOGO_X	62
#define LOGO_Y	20
#define LOGO_START_X	8
#define LOGO_START_Y	1
#define LOGO_DELAY		25
#define TITLE_ANIM_CT			(LOGO_DELAY + 120)

ANIMATER g_PcolorAnimater[EXPL_PCOLOR_ANIMATER_MAX];
ANIMATER g_EcolorAnimater[EXPL_ECOLOR_ANIMATER_MAX];
ANIMATER g_TitleAnimater;

MAPTILE g_nEnemyExplosion[EXPLOSION_FRAME][EXPLOSION_Y][EXPLOSION_X];
MAPTILE g_nPlayerExplosion[EXPLOSION_FRAME][EXPLOSION_Y][EXPLOSION_X];
MAPTILE g_nTitle[TITLE_FRAME][TITLE_Y][TITLE_X];

int g_nTitleAnimCT;
int g_bTitleLogo;
MAPTILE g_nLogo[LOGO_Y][LOGO_X];

void InitAnimation()
{	
	memset(g_PcolorAnimater,0,sizeof(ANIMATER) * EXPL_PCOLOR_ANIMATER_MAX);
	memset(g_EcolorAnimater,0,sizeof(ANIMATER) * EXPL_ECOLOR_ANIMATER_MAX);
	memset(&g_TitleAnimater,0,sizeof(ANIMATER));

	for (int nCnt = 0; nCnt < EXPL_PCOLOR_ANIMATER_MAX; nCnt++)
	{
		g_PcolorAnimater[nCnt].nAnimSizeX = EXPLOSION_X;
		g_PcolorAnimater[nCnt].nAnimSizeY = EXPLOSION_Y;
	}
	for (int nCnt = 0; nCnt < EXPL_ECOLOR_ANIMATER_MAX; nCnt++)
	{
		g_EcolorAnimater[nCnt].nAnimSizeX = EXPLOSION_X;
		g_EcolorAnimater[nCnt].nAnimSizeY = EXPLOSION_Y;
	}


	g_TitleAnimater.nAnimSizeX = TITLE_X;
	g_TitleAnimater.nAnimSizeY = TITLE_Y;

	g_nTitleAnimCT = 0;
	g_bTitleLogo = false;
}

void ReadAnim()
{
	int nTmpE_Expl[EXPLOSION_FRAME][EXPLOSION_Y][EXPLOSION_X];
	int nTmpP_Expl[EXPLOSION_FRAME][EXPLOSION_Y][EXPLOSION_X];
	int nTmp_Title[TITLE_FRAME][TITLE_Y][TITLE_X];
	int nTmp_Logo[LOGO_Y][LOGO_X];

	ReadCSV_Anim(EXPLOSION_X, EXPLOSION_Y, EXPLOSION_FRAME, &nTmpE_Expl[0][0][0], "Explosion.csv");
	ReadCSV_Anim(EXPLOSION_X, EXPLOSION_Y, EXPLOSION_FRAME, &nTmpP_Expl[0][0][0], "Explosion.csv");
	ReadCSV_Anim(TITLE_X, TITLE_Y, TITLE_FRAME, &nTmp_Title[0][0][0], "Title.csv");
	ReadCSV(LOGO_X, LOGO_Y, &nTmp_Logo[0][0], "Logo.csv");

	//スプライトデータをTILEに変換
	for (int z = 0; z < EXPLOSION_FRAME; z++)
	{
		for (int y = 0; y < EXPLOSION_Y; y++)
		{
			for (int x = 0; x < EXPLOSION_X; x++)
			{
				switch (nTmpP_Expl[z][y][x])
				{
				case 0:
					g_nPlayerExplosion[z][y][x] = TILE_Blank;
					break;

				case 1:
					g_nPlayerExplosion[z][y][x] = ANIM_P_Expl;
					break;

				case 2:
					g_nPlayerExplosion[z][y][x] = ANIM_P_Expl_Smoke;
					break;

				case 3:
					g_nPlayerExplosion[z][y][x] = ANIM_P_Expl_Center;
					break;
				}

				switch (nTmpE_Expl[z][y][x])
				{
				case 0:
					g_nEnemyExplosion[z][y][x] = TILE_Blank;
					break;

				case 1:
					g_nEnemyExplosion[z][y][x] = ANIM_E_Expl;
					break;

				case 2:
					g_nEnemyExplosion[z][y][x] = ANIM_E_Expl_Smoke;
					break;

				case 3:
					g_nEnemyExplosion[z][y][x] = ANIM_E_Expl_Center;
					break;
				}
			}
		}
	}

	for (int z = 0; z < TITLE_FRAME; z++)
	{
		for (int y = 0; y < TITLE_Y; y++)
		{
			for (int x = 0; x < TITLE_X; x++)
			{
				switch (nTmp_Title[z][y][x])
				{
				case 1:
					g_nTitle[z][y][x] = ANIM_Title_FireR;
					break;

				case 2:
					g_nTitle[z][y][x] = ANIM_Title_FireO;
					break;

				case 3:
					g_nTitle[z][y][x] = ANIM_Title_FireY;
					break;

				case 4:
					g_nTitle[z][y][x] = ANIM_Title_Rain1;
					break;

				case 5:
					g_nTitle[z][y][x] = ANIM_Title_Rain2;
					break;

				case 6:
					g_nTitle[z][y][x] = ANIM_Title_Rain3;
					break;

				default:
					break;
				}
			}
		}
	}

	for (int y = 0; y < LOGO_Y; y++)
	{
		for (int x = 0; x < LOGO_X; x++)
		{
			switch (nTmp_Logo[y][x])
			{
			case 1:
				g_nLogo[y][x] = ANIM_Title_Logo;
				break;

			case 2:
				g_nLogo[y][x] = ANIM_RainbowRed;
				break;

			case 3:
				g_nLogo[y][x] = ANIM_RainbowOre;
				break;

			case 4:
				g_nLogo[y][x] = ANIM_RainbowYel;
				break;

			case 5:
				g_nLogo[y][x] = ANIM_RainbowGre;
				break;

			case 6:
				g_nLogo[y][x] = ANIM_RainbowCyan;
				break;

			case 7:
				g_nLogo[y][x] = ANIM_RainbowBlue;
				break;

			case 8:
				g_nLogo[y][x] = ANIM_RainbowPur;
				break;

			}
		}
	}
}

void UnInitAnimation()
{
	for (int nCnt = 0; nCnt < EXPL_PCOLOR_ANIMATER_MAX; nCnt++)
	{
		if (g_PcolorAnimater[nCnt].bUse)
		{
			for (int y = 0; y < g_PcolorAnimater[nCnt].nAnimSizeY; y++)
			{
				for (int x = 0; x < g_PcolorAnimater[nCnt].nAnimSizeX; x++)
				{
					int posX = g_PcolorAnimater[nCnt].fPosX + x;
					int posY = g_PcolorAnimater[nCnt].fPosY + y;

					//マップ外の書込み防止
					if (posX < SCREEN_LEFT - 1)
					{
						continue;
					}
					else if (posX > MAP_X - 1)
					{
						continue;
					}
					else if (posY < SCREEN_TOP - 1)
					{
						continue;
					}
					else if (posY > SCREEN_BOTTOM - 2)
					{
						continue;
					}

					SetMap(posX, posY, TILE_Blank);
				}
			}
		}
	}
	for (int nCnt = 0; nCnt < EXPL_ECOLOR_ANIMATER_MAX; nCnt++)
	{
		if (g_EcolorAnimater[nCnt].bUse)
		{
			for (int y = 0; y < g_EcolorAnimater[nCnt].nAnimSizeY; y++)
			{
				for (int x = 0; x < g_EcolorAnimater[nCnt].nAnimSizeX; x++)
				{
					int posX = g_EcolorAnimater[nCnt].fPosX + x;
					int posY = g_EcolorAnimater[nCnt].fPosY + y;

					//マップ外の書込み防止
					if (posX < SCREEN_LEFT - 1)
					{
						continue;
					}
					else if (posX > MAP_X - 1)
					{
						continue;
					}
					else if (posY < SCREEN_TOP - 1)
					{
						continue;
					}
					else if (posY > SCREEN_BOTTOM - 2)
					{
						continue;
					}

					SetMap(posX, posY, TILE_Blank);
				}
			}
		}
	}

	g_TitleAnimater.bUse = false;
	g_bTitleLogo = false;
}

void UpdateAnimation()
{
	for (int nCnt = 0; nCnt < EXPL_PCOLOR_ANIMATER_MAX; nCnt++)
	{
		if (g_PcolorAnimater[nCnt].bUse)
		{
			g_PcolorAnimater[nCnt].nFrameCnt++;

			if (g_PcolorAnimater[nCnt].nSpriteCnt >= g_PcolorAnimater[nCnt].nMaxFrame)
			{
				g_PcolorAnimater[nCnt].bUse = false;
			}
		}
	}

	for (int nCnt = 0; nCnt < EXPL_ECOLOR_ANIMATER_MAX;nCnt++)
	{
		if (g_EcolorAnimater[nCnt].bUse)
		{
			g_EcolorAnimater[nCnt].nFrameCnt++;
			if (g_EcolorAnimater[nCnt].nSpriteCnt >= g_EcolorAnimater[nCnt].nMaxFrame)
			{
				g_EcolorAnimater[nCnt].bUse = false;
			}
		}
	}

	if (g_TitleAnimater.bUse)
	{
		g_TitleAnimater.nFrameCnt++;
		if (g_TitleAnimater.nSpriteCnt >= g_TitleAnimater.nMaxFrame - 1)
		{
			g_nTitleAnimCT++;
		}

		if (g_nTitleAnimCT >= LOGO_DELAY)
		{
			g_bTitleLogo = true;
		}

		if (g_nTitleAnimCT >= TITLE_ANIM_CT)
		{
			g_TitleAnimater.nSpriteCnt = 0;
			g_nTitleAnimCT = 0;
			g_TitleAnimater.nFrameCnt = 0;
			g_bTitleLogo = false;
		}
	}
}

void DrawAnimation()
{
	//特定の数値を特定のタイルに変換する
	for (int nCnt = 0; nCnt < EXPL_PCOLOR_ANIMATER_MAX; nCnt++)
	{
		if (g_PcolorAnimater[nCnt].bUse)
		{
			//スプライトデータを書き込む
			for (int y = 0; y < EXPLOSION_Y; y++)
			{
				for (int x = 0; x < EXPLOSION_X; x++)
				{
					int posX = g_PcolorAnimater[nCnt].fPosX + x;
					int posY = g_PcolorAnimater[nCnt].fPosY + y;

					//マップ外の書込み防止
					if (posX < SCREEN_LEFT - 1)
					{
						continue;
					}
					else if (posX > MAP_X - 1)
					{
						continue;
					}
					else if (posY < SCREEN_TOP - 1)
					{
						continue;
					}
					else if (posY > SCREEN_BOTTOM - 2)
					{
						continue;
					}

					SetMap(posX,posY,g_nEnemyExplosion[g_PcolorAnimater[nCnt].nSpriteCnt][y][x]);
				}
			}
			//フレームレートに合わせて更新する
			if (g_PcolorAnimater[nCnt].nFrameCnt % g_PcolorAnimater[nCnt].nFrameRate == 0)
			{
				//次のスプライトデータにする
				g_PcolorAnimater[nCnt].nSpriteCnt++;
			}

		}
	}

	for (int nCnt = 0; nCnt < EXPL_ECOLOR_ANIMATER_MAX; nCnt++)
	{
		if (g_EcolorAnimater[nCnt].bUse)
		{
			//スプライトデータを書き込む
			for (int y = 0; y < EXPLOSION_Y; y++)
			{
				for (int x = 0; x < EXPLOSION_X; x++)
				{
					int posX = g_EcolorAnimater[nCnt].fPosX + x;
					int posY = g_EcolorAnimater[nCnt].fPosY + y;
					//マップ外の書込み防止
					if (posX < SCREEN_LEFT - 1)
					{
						continue;
					}
					else if (posX > MAP_X - 1)
					{
						continue;
					}
					else if (posY < SCREEN_TOP - 1)
					{
						continue;
					}
					else if (posY > SCREEN_BOTTOM - 2)
					{
						continue;
					}
					SetMap(posX, posY, g_nPlayerExplosion[g_EcolorAnimater[nCnt].nSpriteCnt][y][x]);
				}
			}
			//フレームレートに合わせて更新する
			if (g_EcolorAnimater[nCnt].nFrameCnt % g_EcolorAnimater[nCnt].nFrameRate == 0)
			{
				//次のスプライトデータにする
				g_EcolorAnimater[nCnt].nSpriteCnt++;
			}
		}
	}
	if (g_TitleAnimater.bUse && !g_bTitleLogo)
	{
		if (g_TitleAnimater.nFrameCnt % g_TitleAnimater.nFrameRate == 0)
		{
			//スプライトデータを書き込む
			for (int y = 0; y < TITLE_Y; y++)
			{
				for (int x = 0; x < TITLE_X; x++)
				{
					int posX = g_TitleAnimater.fPosX + x;
					int posY = g_TitleAnimater.fPosY + y;

					SetMap(posX,posY,g_nTitle[g_TitleAnimater.nSpriteCnt][y][x]);

					if (g_nTitle[g_TitleAnimater.nSpriteCnt][y][x] == ANIM_Title_Logo)
					{
						std::cout << "";
					}
				}
			}
			//次のスプライトデータにする
			if (g_TitleAnimater.nSpriteCnt < g_TitleAnimater.nMaxFrame - 1)
			{
				g_TitleAnimater.nSpriteCnt++;
				if (g_TitleAnimater.nSpriteCnt == 4)
				{
					SoundPlay(SE_TITLE_FIRE,NOTLOOP);
				}
			}
		}

	}
	if (g_bTitleLogo)
	{
		//スプライトデータを書き込む
		for (int y = 0; y < LOGO_Y; y++)
		{
			for (int x = 0; x < LOGO_X; x++)
			{
				int posX = LOGO_START_X + x;
				int posY = LOGO_START_Y + y;
				SetMap(posX, posY, g_nLogo[y][x]);
			}
		}
	}
}

void CreateAnimation(int nMaxFrame,float fX,float fY,ANIMETORNAME nAnimater)
{
	switch (nAnimater)
	{
	case A_EcolorExpl:
		for (int nCnt = 0; nCnt < EXPL_ECOLOR_ANIMATER_MAX; nCnt++)
		{
			if (!g_EcolorAnimater[nCnt].bUse)
			{
				g_EcolorAnimater[nCnt].nFrameCnt = 0;
				g_EcolorAnimater[nCnt].nSpriteCnt = 0;
				g_EcolorAnimater[nCnt].nMaxFrame = nMaxFrame;
				g_EcolorAnimater[nCnt].nFrameRate = EXPLOSION_FRAMERATE;
				//左上に補正
				g_EcolorAnimater[nCnt].fPosX = fX - g_EcolorAnimater[nCnt].nAnimSizeX / 2;
				g_EcolorAnimater[nCnt].fPosY = fY - g_EcolorAnimater[nCnt].nAnimSizeY / 2;

				//中央との距離によって音を鳴らす
				if (abs(fX - GetPlayer()->fPosX) <= SOUND_RANGE)
				{
					SoundPlay(SE_EXPLOSION,NOTLOOP);
				}

				g_EcolorAnimater[nCnt].bUse = true;
				break;
			}
		}
		break;

	case A_PcolorExpl:
		for (int nCnt = 0; nCnt < EXPL_PCOLOR_ANIMATER_MAX; nCnt++)
		{
			if (!g_PcolorAnimater[nCnt].bUse)
			{
				g_PcolorAnimater[nCnt].nFrameCnt = 0;
				g_PcolorAnimater[nCnt].nSpriteCnt = 0;
				g_PcolorAnimater[nCnt].nMaxFrame = nMaxFrame;
				g_PcolorAnimater[nCnt].nFrameRate = EXPLOSION_FRAMERATE;
				//左上に補正
				g_PcolorAnimater[nCnt].fPosX = fX - g_PcolorAnimater[nCnt].nAnimSizeX / 2;
				g_PcolorAnimater[nCnt].fPosY = fY - g_PcolorAnimater[nCnt].nAnimSizeY / 2;

				//中央との距離によって音を鳴らす
				if (abs(fX - GetPlayer()->fPosX) <= SOUND_RANGE)
				{
					SoundPlay(SE_EXPLOSION, NOTLOOP);
				}

				g_PcolorAnimater[nCnt].bUse = true;

				break;
			}

		}
		break;

	case A_Title:
		if (!g_TitleAnimater.bUse)
		{
			g_TitleAnimater.nFrameCnt = 0;
			g_TitleAnimater.nSpriteCnt = 0;
			g_TitleAnimater.nMaxFrame = nMaxFrame;
			g_TitleAnimater.nFrameRate = TITLE_FRAMERATE;

			g_TitleAnimater.bUse = true;
		}
		break;



	default:
		break;
	}
}

