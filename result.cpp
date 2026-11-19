#define CONIOEX
#include "conioex.h"
#include "scene.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "field.h"
#include "paintMap.h"
#include "collision.h"
#include "drawWindow.h"
#include "inputKey.h"
#include "color.h"
#include "sound.h"
#include "UI.h"

bool g_resultEnd;
int fCount;
#define RESULT_POSX		37
#define RESULT_POSY		12


void InitResult(void)
{
	InitField();
	InitDrawWindow();
	g_resultEnd = false;
	fCount = 0;
	SoundPlay(RESULTBGM,LOOP);
}

void UnInitResult(void)
{
	UnInitPlayer();
	UnInitBullet();
	UnInitEnemyBullet();
	UnInitEnemy();
	//UninitScore();
	UnInitField();
	SoundEnd(RESULTBGM);
}

void UpdateResult(void)
{
	UpdateUI();

	if ((GetKeyTrigger(KEY_ENTER) || GetKeyTrigger(KEY_SHOT)) && g_resultEnd)
	{
		SetScene(SCENE_TITLE);
	}

}


void DrawResult(void)
{//後の関数が優先的に表示される
	DrawField();
	UpdateDrawWindow();
	DrawDrawWindow();
	DrawUI();
	fCount++;

	if (fCount == 2)
	{
		msleep(1000);
	}

	if (g_resultEnd)
	{//最後の画面の表示
		int nPlayerColor = GetColorCount()[0];
		int nEnemyColor = GetColorCount()[1];
		std::string sOutput;

		sOutput += "\x1b[" + IntToString(RESULT_POSY) + ";" + IntToString(RESULT_POSX) + "H";
		sOutput += ConvertSkyColor(RESULT_POSY - 1);

		if (nPlayerColor >= nEnemyColor)
		{//勝利
			sOutput += ColorNameToString(C_Player,TEXTCOLOR);

			sOutput += "WIN!!";
		}
		else
		{//勝利
			sOutput += ColorNameToString(C_Enemy,TEXTCOLOR);

			sOutput += "LOSE...";
		}

		sOutput += "\x1b[14;32H";
		sOutput += "\x1b[38;2;0;0;0m";
		sOutput += ConvertSkyColor(RESULT_POSY + 1);
		sOutput += u8"クリックでもどる";

		std::cout << sOutput;
	}
}

void SetResultEnd(bool end)
{
	g_resultEnd = end;
}