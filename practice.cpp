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
#include "UI.h"
#include "game.h"
#include "animation.h"
#include "enemySpawn.h"
#include "sound.h"
#include "inputkey.h"
#include "scene.h"
#include "bomb.h"

bool g_IsSpawner;

void InitPractice(void)
{
	InitPlayer();
	InitBullet();
	InitEnemy();
	InitEnemyBullet();
	InitField();
	InitCollision();
	InitUI();
	InitAnimation();
	InitDrawWindow();
	InitEnemySpawn();
	InitBomb();

	SoundPlay(BGM, LOOP);
	g_IsSpawner = true;
}

void UnInitPractice(void)
{
	UnInitPlayer();
	UnInitBullet();
	UnInitEnemyBullet();
	UnInitEnemy();
	UnInitAnimation();
	UnInitField();
	UnInitCollision();
	UnInitBomb();

	SoundEnd(BGM);
}

void UpdatePractice(void)
{
	UpdateBullet();
	UpdatePlayer();
	UpdateEnemy();
	UpdateEnemyBullet();
	UpdateField();
	UpdateUI();
	UpdateEnemySpawn();
	UpdateAnimation();
	UpdateBomb();

	if (GetKeyTrigger(KEY_BACK))
	{
		SetScene(SCENE_TITLE);
	}

	SPAWNER* spawner = GetSpawner();
	g_IsSpawner = false;
	for (int nCnt = 0; nCnt < SPAWNER_MAX; nCnt++,spawner++)
	{
		if (spawner->bUse) 
		{
			g_IsSpawner = true;
		}
	}


	if (GetKeyTrigger(KEY_SPAWN) && !g_IsSpawner)
	{
		CreateSpawner(93.0f, 16.0f, 30, 120);
	}

}


void DrawPractice(void)
{//後の関数が優先的に表示される
	DrawAnimation();
	DrawEnemyBullet();
	DrawBullet();
	DrawBomb();
	DrawEnemy();
	DrawPlayer();
	DrawField();
	DrawEnemySpawn();

	UpdateDrawWindow();
	DrawDrawWindow();
	DrawUI();

	gotoxy(1,5);
	std::cout << ConvertSkyColor(5);
	std::cout << "\x1b[38;2;0;0;0m";
	std::cout << u8"Bキー/optionボタン:タイトルへ";
	gotoxy(1, 6);
	std::cout << ConvertSkyColor(3);
	std::cout << u8"Tキー/左スティック押し込み:ホットスポット復活";
}