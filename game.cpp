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

void InitGame(void)
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

	SoundPlay(BGM,LOOP);
}
void UnInitGame(void)
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

void UpdateGame(void)
{
	UpdateBullet();
	UpdateBomb();
	UpdatePlayer();
	UpdateEnemy();
	UpdateEnemyBullet();
	UpdateField();
	UpdateUI();
	UpdateEnemySpawn();
	UpdateAnimation();

	if (GetKeyTrigger(KEY_BACK))
	{
		SetScene(SCENE_TITLE);
	}
}


void DrawGame(void)
{//å„ÇÃä÷êîÇ™óDêÊìIÇ…ï\é¶Ç≥ÇÍÇÈ
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
}