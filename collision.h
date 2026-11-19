#pragma once
#include "bullet.h"
#include "enemy.h"
#include "player.h"
#include "field.h"
#include "enemyBullet.h"
#include "drawWindow.h"
#include "enemySpawn.h"

//キャラクターのサイズ
//キャラクターごとにサイズが異なるのが普通
//初期化で構造体へ格納しようね
#define CHARACTER_COLLISION_SIZEX	(0.5)
#define CHARACTER_COLLISION_SIZEY	(0.5)

#define PLAYERSIDE		0	//爆発当たり判定用
#define ENEMYSIDE		1	//爆発当たり判定用

void InitCollision();

//敵キャラクターとプレイヤー弾の当たり判定
//敵から呼び出す関数
bool EnemyCollisionBullet(ENEMY* ptEnemy);

bool SpawnerCollisionBullet(SPAWNER* ptSpawner);
void UnInitCollision();


//プレイヤーから呼び出す関数
//プレイヤーと敵キャラクターの当たり判定
bool PlayerCollisionEnemy(PLAYER* ptPlayer);
bool PlayerCollisionField(PLAYER* ptPlayer);
bool PlayerCollisionRight(PLAYER* ptPlayer);
bool PlayerCollisionLeft(PLAYER* ptPlayer);
bool PlayerCollisionCeiling(PLAYER* ptPlayer);

bool PlayerCollisionSpawnerF(PLAYER* ptPlayer);
bool PlayerCollisionSpawnerR(PLAYER* ptPlayer);
bool PlayerCollisionSpawnerL(PLAYER* ptPlayer);
bool PlayerCollisionSpawnerC(PLAYER* ptPlayer);

bool EnemyCollisionField(ENEMY* ptEnemy);
bool EnemyCollisionRight(ENEMY* ptEnemy);
bool EnemyCollisionLeft(ENEMY* ptEnemy);
bool EnemyCollisionCeiling(ENEMY* ptEnemy);

bool Enemy2CollisionEnemy2(ENEMY* ptEnemy, int nEnemyNum);

bool BulletCollisionField(BULLET* ptBullet);
bool BulletCollisionRight(BULLET* ptBullet);
bool BulletCollisionLeft(BULLET* ptBullet);
bool BulletCollisionCeiling(BULLET* ptBullet);

bool EnemyBulletCollisionField(ENEMYBULLET* ptEnemyBullet);
bool EnemyBulletCollisionRight(ENEMYBULLET* ptEnemyBullet);
bool EnemyBulletCollisionLeft(ENEMYBULLET* ptEnemyBullet);
bool EnemyBulletCollisionCeiling(ENEMYBULLET* ptEnemyBullet);

void ExplosionCollision(float fX,float fY, int nSide);

void HitPosPaintHorizon(float x,float y,MAPTILE nColor);
void HitPosPaintVertical(float x,float y,MAPTILE nColor,int nSide);

void ExplosionPaint(float fX, float fY,MAPTILE color);
