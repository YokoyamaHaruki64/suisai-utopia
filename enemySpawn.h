#pragma once
#include "define.h"

#define SPAWNER_MAX	(5)
#define SPAWNER_SIZEX	4
#define SPAWNER_SIZEY	2

struct SPAWNER
{
	VECTOR2 pos;
	VECTOR2 posOld;
	int nHP;
	int nMaxHP;
	bool bUse;

	int nSpawnCT;
	int nSpawnCTCnt;

	int nMutekiCnt;
};

void InitEnemySpawn();
void UnInitEnemySpawn();
void UpdateEnemySpawn();
void DrawEnemySpawn();

SPAWNER* GetSpawner();
void DestroySpawner(int nCnt);
void CreateSpawner(float fX, float fY, int nHP, int nSpawnCT);