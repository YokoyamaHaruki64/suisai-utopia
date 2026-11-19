#define CONIOEX
#include "conioex.h"
#include "field.h"
#include "game.h"
#include <iostream>
#include "drawWindow.h"
#include "color.h"
#include "sound.h"
#include "player.h"

FIELD g_tField[FIELD_MAX];
bool g_tUpdateFlag = true;
MAPTILE* g_pnFieldMap;
int g_SE;

#define P_COLOR_FILL	1
#define E_COLOR_FILL	2

void InitField()
{
	//先頭アドレスの取得（posx,posyと合わせるためにxyをそれぞれ1ずらすように調整する）
	g_pnFieldMap = GetMap();
	//g_pnFieldMap += WINDOW_X + 1;

	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
	{
		g_tField[nCnt].fPosX = 0.0f;
		g_tField[nCnt].fPosY = 0.0f;
		g_tField[nCnt].fWidth = 0.0f;
		g_tField[nCnt].fHight = 0.0f;
		g_tField[nCnt].bUse = false;
		g_tField[nCnt].bFill = false;

		g_tField[nCnt].fPosOldX = 0.0f;
		g_tField[nCnt].fPosOldY = 0.0f;
		g_tField[nCnt].fSpeedX = 0.0f;
		g_tField[nCnt].fSpeedY = 0.0f;
		g_tField[nCnt].bMove = false;
		g_tField[nCnt].nCount = 0;
	}

	CreateField(134.0,  22.0,  8.0, 3.0, 0.0, 0.0);
	CreateField(146.0,  18.0, 16.0, 4.0, 0.0, 0.0);
	CreateField(160.0,  15.0,  8.0, 5.0, 0.0, 0.0);
	CreateField(134.0,  10.0, 10.0, 4.0, 0.0, 0.0);
	CreateField(92.0,  21.0,  6.0, 4.0, 0.0, 0.0);
	CreateField(174.0,  19.0,  8.0, 6.0, 0.0, 0.0);
	CreateField(170.0,  8.0,  30.0, 3.0, 0.0, 0.0);
	CreateField(82.0,  18.0, 26.0, 3.0, 0.0, 0.0);
	CreateField(96.0,  8.0,	 12.0, 2.0, 0.0, 0.0);
	CreateField(114.0, 22.0, 10.0, 3.0, 0.0, 0.0);
	CreateField(22.0, 15.0, 22.0, 4.0, 0.0, 0.0);
	CreateField(32.0, 11.0, 12.0, 7.0, 0.0, 0.0);
	CreateField(22.0, 24.0, 22.0, 1.0, 0.0, 0.0);
	CreateField(50.0, 8.0, 30.0, 2.0, 0.0, 0.0);
	CreateField(4.0, 20.0, 10.0, 5.0, 0.0, 0.0);
	CreateField(4.0, 8.0, 10.0, 5.0, 0.0, 0.0);
	CreateField(44.0, 15.0, 30.0, 3.0, 0.0, 0.0);
	CreateField(44.0, 23.0, 30.0, 2.0, 0.0, 0.0);
	CreateField(118.0, 12.0, 6.0, 5.0, 0.0, 0.0);

	g_tUpdateFlag = true;
}


void UnInitField()
{
	g_tUpdateFlag = true;
}

void UpdateField()
{

	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
	{
		if (g_tField[nCnt].bUse)
		{//使っている場合
			if (g_tField[nCnt].bMove)
			{//移動処理
				g_tField[nCnt].fPosOldX = g_tField[nCnt].fPosX;
				g_tField[nCnt].fPosOldY = g_tField[nCnt].fPosY;
				g_tField[nCnt].fPosX += g_tField[nCnt].fSpeedX;
				g_tField[nCnt].fPosY += g_tField[nCnt].fSpeedY;

				//移動方向
				g_tField[nCnt].nCount++;
				if (g_tField[nCnt].nCount > 80)
				{
					g_tField[nCnt].fSpeedX *= -1.0f;
					g_tField[nCnt].fSpeedY *= -1.0f;
					g_tField[nCnt].nCount = 0;
				}
			}

			switch (CheckColorField(g_tField[nCnt]))
			{
			case P_COLOR_FILL:
				//塗られてないときだけ全塗り（こうしないと一回塗りきった後塗れない）
				if (!g_tField[nCnt].bFill)
				{
					FillColor(g_tField[nCnt], P_COLOR_FILL);
					g_tField[nCnt].bFill = true;
				}
				break;

			case E_COLOR_FILL:
				//塗られてないときだけ全塗り（こうしないと一回塗りきった後塗れない）
				if (!g_tField[nCnt].bFill)
				{
					FillColor(g_tField[nCnt], E_COLOR_FILL);
					g_tField[nCnt].bFill = true;
				}
				break;

			default:
				//塗りきってない判定
				g_tField[nCnt].bFill = false;
				break;
			}
		}
	}
}

void DrawField()
{
	//背景再描画フラグ
	if (g_tUpdateFlag)
	{//Field再描画
		UpdateDrawField();
		g_tUpdateFlag = false;
	}
	else
	{
		for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
		{
			if (g_tField[nCnt].bUse)
			{//使っている場合
				if (g_tField[nCnt].bMove)
				{//移動処理
					if (g_tField[nCnt].fPosOldX != g_tField[nCnt].fPosX ||
						g_tField[nCnt].fPosOldY != g_tField[nCnt].fPosY)
					{
						for (int nHight = 0; nHight < g_tField[nCnt].fHight; nHight++)
						{
							for (int nWidth = 0; nWidth < g_tField[nCnt].fWidth; nWidth++)
							{
								//ポインタを二次元配列のように扱う
								//g_pnFieldMap[OffsetMapPointerF(g_tField[nCnt].fPosOldX + nWidth, g_tField[nCnt].fPosOldY + nHight)] = TILE_Blank;
								SetMapOffset(g_tField[nCnt].fPosOldX + nWidth, g_tField[nCnt].fPosOldY + nHight,TILE_Blank);
							}
						}
					}
				}	
				//地面の表示
				for (int nHight = 0; nHight < g_tField[nCnt].fHight; nHight++)
				{
					for (int nWidth = 0; nWidth < g_tField[nCnt].fWidth; nWidth++)
					{
						//ポインタを二次元配列のように扱う
						//g_pnFieldMap[OffsetMapPointerF(g_tField[nCnt].fPosX + (nWidth),g_tField[nCnt].fPosY + nHight) ] = TILE_NormalField;
						SetMapOffset(g_tField[nCnt].fPosX + nWidth, g_tField[nCnt].fPosY + nHight,TILE_NormalField);
					}
				}
			}
		}
	}
}

FIELD* GetField()
{
	return g_tField;
}

void UpdateDrawField()
{
	//外枠の表示
	int nCharSize = 2;
	//一番上の行
	//for (int x = 0; x < WINDOW_X - 1; x++)
	//{
	//	g_pnFieldMap[x] = TILE_Wall;
	//}
	//横の壁
	//for (int y = 1; y < WINDOW_Y; y++)
	//{
	//	//ポインタを二次元配列のように扱う
	//	g_pnFieldMap[y * MAP_X] = TILE_Wall;//左端
	//	g_pnFieldMap[y * MAP_X + 1] = TILE_Zenkaku;//左端
	//	g_pnFieldMap[y * MAP_X - nCharSize] = TILE_Wall;//右端
	//	g_pnFieldMap[y * MAP_X - nCharSize +1] = TILE_Zenkaku;//右端

	//}
	//一番下の行
	for (int x = 0; x < MAP_X; x++)
	{
		//g_pnFieldMap[((WINDOW_Y - 1) * MAP_X + 1) + x] = TILE_Ground;
		//g_pnFieldMap[((WINDOW_Y - 1) * MAP_X + 1) + x + 1] = TILE_Zenkaku;
		SetMap(x, WINDOW_Y-1, TILE_Ground);
		//SetMap(x*2+1, WINDOW_Y-1, TILE_Zenkaku);
	}

	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
	{
		if (g_tField[nCnt].bUse)
		{//使っている場合
			if (g_tField[nCnt].bMove)
			{//移動処理
				if (g_tField[nCnt].fPosOldX != g_tField[nCnt].fPosX ||
					g_tField[nCnt].fPosOldY != g_tField[nCnt].fPosY)
				{
					for (int nHight = 0; nHight < g_tField[nCnt].fHight; nHight++)
					{
						for (int nWidth = 0; nWidth < g_tField[nCnt].fWidth; nWidth++)
						{
							//ポインタを二次元配列のように扱う
							//g_pnFieldMap[OffsetMapPointerF(g_tField[nCnt].fPosOldX + nWidth, g_tField[nCnt].fPosOldY + nHight)] = TILE_Blank;
							SetMapOffset(g_tField[nCnt].fPosOldX + nWidth, g_tField[nCnt].fPosOldY + nHight,TILE_Blank);
						}
					}
				}

			}
			//地面の表示
			for (int nHight = 0; nHight < g_tField[nCnt].fHight; nHight++)
			{
				for (int nWidth = 0; nWidth < g_tField[nCnt].fWidth; nWidth++)
				{
					//ポインタを二次元配列のように扱う
					//g_pnFieldMap[OffsetMapPointerF(g_tField[nCnt].fPosX + nWidth, g_tField[nCnt].fPosY + nHight)] = TILE_NormalField;
					SetMapOffset(g_tField[nCnt].fPosX + nWidth, g_tField[nCnt].fPosY + nHight, TILE_Blank);
				}
			}

		}
	}
}

void CreateField(float fX, float fY,
	float fWidth, float fHight,
	float fMx, float fMy)
{
	for (int nCnt = 0; nCnt < FIELD_MAX; nCnt++)
	{
		if (!g_tField[nCnt].bUse)
		{
			g_tField[nCnt].fPosX = fX;
			g_tField[nCnt].fPosY = fY;
			g_tField[nCnt].fWidth = fWidth;
			g_tField[nCnt].fHight = fHight;
			g_tField[nCnt].bUse = true;

			g_tField[nCnt].fPosOldX = fX;
			g_tField[nCnt].fPosOldY = fY;
			g_tField[nCnt].fSpeedX = fMx;
			g_tField[nCnt].fSpeedY = fMy;
			if (g_tField[nCnt].fSpeedX == 0.0f && g_tField[nCnt].fSpeedY == 0.0f)
			{
				g_tField[nCnt].bMove = false;
			}
			else
			{
				g_tField[nCnt].bMove = true;
			}
			break;
		}
	}
}

void FieldUpdate()
{
	g_tUpdateFlag = true;
}

int CheckColorField(FIELD tField)
{
	//表面積をある程度塗ったら全部塗るようにする

	int targetCnt = ((tField.fHight * 2 + tField.fWidth * 2) - 4) / 5 * 4;
	int playerFieldCnt = 0;
	int enemyFieldCnt = 0;

	//上
	for (int x = tField.fPosX; x < tField.fPosX + tField.fWidth; x++)
	{
		switch (GetMapPointOffset(x,tField.fPosY))
		{
		case TILE_PlayerColor:
			playerFieldCnt++;
			break;

		case TILE_EnemyColor:
			enemyFieldCnt++;
			break;

		default:
			break;
		}
	}
	//左
	for (int y = tField.fPosY; y < tField.fPosY + tField.fHight; y++)
	{
		switch (GetMapPointOffset(tField.fPosX, y))
		{
		case TILE_PlayerColor:
			playerFieldCnt++;
			break;

		case TILE_EnemyColor:
			enemyFieldCnt++;
			break;

		default:
			break;
		}
	}
	//右
	for (int y = tField.fPosY; y < tField.fPosY + tField.fHight; y++)
	{
		switch (GetMapPointOffset(tField.fPosX + tField.fWidth - 1,y))
		{
		case TILE_PlayerColor:
			playerFieldCnt++;
			break;

		case TILE_EnemyColor:
			enemyFieldCnt++;
			break;

		default:
			break;
		}

	}
	//下
	for (int x = tField.fPosX; x < tField.fPosX + tField.fWidth; x++)
	{
		switch (GetMapPointOffset(x, tField.fPosY + tField.fHight - 1))
		{
		case TILE_PlayerColor:
			playerFieldCnt++;
			break;

		case TILE_EnemyColor:
			enemyFieldCnt++;
			break;

		default:
			break;
		}
	}

	//for (int y = tField.fPosY; y < tField.fPosY + tField.fHight; y++)
	//{
	//	for (int x = tField.fPosX; x < tField.fPosX + tField.fWidth; x++)
	//	{
	//		switch (GetMapPointOffset(x, y))
	//		{
	//		case TILE_PlayerColor:
	//			playerFieldCnt++;
	//			break;

	//		case TILE_EnemyColor:
	//			enemyFieldCnt++;
	//			break;

	//		default:
	//			break;
	//		}
	//	}
	//}

	//カウントがターゲット以上ならfillする
	//以下ならbFillをオフにする
	if (playerFieldCnt >= targetCnt)
	{
		return P_COLOR_FILL;
	}
	if (enemyFieldCnt >= targetCnt)
	{
		return E_COLOR_FILL;
	}

	return 0;
}

void FillColor(FIELD tField,int nMode)
{
	switch (nMode)
	{
	case P_COLOR_FILL:
		for (int y = tField.fPosY; y < tField.fPosY + tField.fHight; y++)
		{
			for (int x = tField.fPosX; x < tField.fPosX + tField.fWidth; x++)
			{
				SetColorMapOffset(x,y,TILE_PlayerColor);
			}
		}
		break;

	case E_COLOR_FILL:
		for (int y = tField.fPosY; y < tField.fPosY + tField.fHight; y++)
		{
			for (int x = tField.fPosX; x < tField.fPosX + tField.fWidth; x++)
			{
				SetColorMapOffset(x, y, TILE_EnemyColor);
			}
		}
		break;

	default:
		break;
	}

	//フィールドの端(x座標で見た場合)の近い側とプレイヤーの距離が一定距離以内なら音を鳴らす
	float fDistance;
	PLAYER* ptPlayer = GetPlayer();
	if (abs(tField.fPosX - ptPlayer->fPosX) > abs((tField.fPosX + tField.fWidth)- ptPlayer->fPosX))
	{//小さい方をDistanceに格納
		fDistance = abs((tField.fPosX + tField.fWidth) - ptPlayer->fPosX);
	}
	else
	{
		fDistance = abs(tField.fPosX - ptPlayer->fPosX);
	}

	if (fDistance <= SOUND_RANGE)
	{
		SoundPlay(SE_MAPFILL, NOTLOOP);
	}
}