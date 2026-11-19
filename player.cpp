#define CONIOEX
#include "conioex.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "collision.h"
#include "inputKey.h"
#include "score.h"
#include "color.h"
#include "drawWindow.h"
#include "define.h"
#include "animation.h"
#include "bomb.h"
#include "sound.h"
#include <iostream>

//グローバル変数
PLAYER g_tPlayer;
MAPTILE* g_pnPlayerMap;
float g_fOldJumpY;
bool isMouse;

//マクロ定義
#define PLAYER_MOVE_SPEED_X			(MOVE_SPEED_X * 0.7)		//X座標の移動量
#define PLAYER_MOVE_SPEED_Y			(MOVE_SPEED_Y * 0.7)		//Y座標の移動量
#define PLAYER_DASH_MULTIPLIER		(1.8)						//ダッシュ時の倍率
#define PLAYER_MUTEKIJIKAN			(120)						//無敵時間
#define PLAYER_RESPAWN_TIME			(300)						//HP最大値

#define VALUE_JUMP			 (0.55)	  //ジャンプ力
#define VALUE_JUMP_GRAVITY	 (44)	  //ジャンプ長押し時の重力みたいなやつ
#define VALUE_JUMP_MAX		 (8)	  //ジャンプの最大高さ(数値通りではない)

#define VALUE_SHOT_ANGLE_ADD	(1.0)	//-y方向を0度とした発射方向（上下）の変化量
#define VALUE_SHOT_ANGLE_MIN	(10)	//発射方向(上下)の下限
#define VALUE_SHOT_ANGLE_MAX	(135)	//発射方向(上下)の上限
#define SHOT_COOLTIME			(5)		//発射クールタイム(フレーム単位)
#define BOMB_COOLTIME			(60)	//発射クールタイム(フレーム単位)
#define SHOT_COST				(1.0)
#define BOMB_COST				(33.0)
#define HEAL_CHARGE				(1.0)

#define JUMP_COUNTMAX		 (40)		//ジャンプ長押しの最大入力フレーム

#define CONT_DEADZONE		(1.8)
#define WINDOW_CENTERX		40
#define MAP_CENTERY			12.5

#define BULLET_SHOT		0
#define BULLET_BOMB		1

void InitPlayer(void)
{
	memset(&g_tPlayer,0, sizeof(PLAYER));
	g_pnPlayerMap = GetMap();

	g_tPlayer.fPosX = g_tPlayer.fPosOldX = PLAYER_START_X; // X座標初期位置
	g_tPlayer.fPosY = g_tPlayer.fPosOldY = 13; // Y座標初期位置
	g_tPlayer.angle.x = 1;
	g_tPlayer.angle.y = -1;
	g_tPlayer.fShotAngle = 45.0f;
	g_tPlayer.fSizeX = 1.0f;
	g_tPlayer.fSizeY = 2.0f;
	g_tPlayer.bDoubleJump = true;
	g_tPlayer.fCharge = CHARGE_MAX;
	g_tPlayer.nBombCnt = BOMB_COOLTIME;
	g_tPlayer.nHP = PLAYER_HP_MAX;


	if (inport(PJ1_RPOS) == -1)
	{
		isMouse = true;
	}
}

void UnInitPlayer(void)
{
	SetMapOffset(g_tPlayer.fPosOldX, g_tPlayer.fPosOldY, TILE_Blank);
	SetMapOffset(g_tPlayer.fPosX, g_tPlayer.fPosY, TILE_Blank);
}

void UpdatePlayer(void)
{
	//現在の座標位置を保存
	g_tPlayer.fPosOldX = g_tPlayer.fPosX;
	g_tPlayer.fPosOldY = g_tPlayer.fPosY;

	//移動量のリセット
	g_tPlayer.fSpeedX = 0;

	//入力処理
	if (g_tPlayer.nRespawn == 0)
	{//死亡中は操作不能
		//WASDで移動
		if (inport(PK_D))
		{
			g_tPlayer.fSpeedX = PLAYER_MOVE_SPEED_X;
		}
		if (inport(PK_A))
		{
			g_tPlayer.fSpeedX = -PLAYER_MOVE_SPEED_X;
			//g_tPlayer.fAngle = 180.0f;
		}

		if (inport(PK_W) && g_tPlayer.bWall)
		{
			g_tPlayer.fSpeedY = -PLAYER_MOVE_SPEED_Y;
			g_tPlayer.bWallUp = true;
		}
		else
		{
			g_tPlayer.bWallUp = false;
		}

		//ジャンプ
		if (GetKeyTrigger(KEY_JUMP) && !g_tPlayer.bJump && !g_tPlayer.bWallUp)
		{
			g_tPlayer.fSpeedY = -VALUE_JUMP;
			g_fOldJumpY = g_tPlayer.fPosY;
			g_tPlayer.bJump = true;
			g_tPlayer.bUp = true;
		}
		//ジャンプ(二段目)
		if (GetKeyTrigger(KEY_JUMP) && (!g_tPlayer.bDoubleJump && (g_tPlayer.bJumpEnd || (!g_tPlayer.bLand && g_tPlayer.fPosY < SCREEN_BOTTOM - 0.5f - (g_tPlayer.fSizeY - 0.5f)))))
		{
			//条件1 キー入力
			//条件2 ジャンプ(2回目)が未使用
			//条件3 一度ジャンプ入力が終わっている もしくは フィールドに乗っていないかつプレイヤーが1番下の地面にいない
			g_tPlayer.fSpeedY = -VALUE_JUMP;
			g_fOldJumpY = g_tPlayer.fPosY;
			g_tPlayer.bJump = true;
			g_tPlayer.bUp = true;
			g_tPlayer.bDoubleJump = true;
			g_tPlayer.bJumpEnd = false;
			g_tPlayer.nJumpCnt = 0;
		}

		if (GetKeyPress(KEY_JUMP) && g_tPlayer.bJump && g_tPlayer.bUp)
		{//長押し
			if (g_tPlayer.nJumpCnt <= JUMP_COUNTMAX)
			{
				g_tPlayer.nJumpCnt++;
				//y方向のスピードをほんの少しずつ減らしながら設定していく
				g_tPlayer.fSpeedY = -VALUE_JUMP + (float)g_tPlayer.nJumpCnt / VALUE_JUMP_GRAVITY;
			}
		}

		//ジャンプ入力終了もしくは最大到達点に到達
		if ((GetKeyRelease(KEY_JUMP) || g_tPlayer.fPosY < g_fOldJumpY - VALUE_JUMP_MAX) && !g_tPlayer.bJumpEnd)
		{
			g_tPlayer.fSpeedY = 0.0f;
			g_tPlayer.nJumpCnt = 0;
			g_tPlayer.bJumpEnd = true;
		}

		//ダッシュ
		if (GetKeyTrigger(KEY_DASH))
		{
			switch (g_tPlayer.nState)
			{
			case PS_Idle://アイドル→ダッシュ
				g_tPlayer.nState = PS_Dash;
				g_tPlayer.fSizeY = 1.0f;
				g_tPlayer.fPosY += 1.0f;
				break;

			case PS_Dash://ダッシュ→アイドル
				g_tPlayer.nState = PS_Idle;
				g_tPlayer.fSizeY = 2.0f;
				break;

			default:
				break;
			}
		}

		//ダッシュ中の処理
		if (g_tPlayer.nState == PS_Dash)
		{
			g_tPlayer.fSpeedX *= PLAYER_DASH_MULTIPLIER;
			if (g_tPlayer.fCharge < CHARGE_MAX)
			{
				g_tPlayer.fCharge += HEAL_CHARGE;
			}
		}


		//弾発射
		if (GetKeyPress(KEY_SHOT) /*&& g_tPlayer.nState != PS_Dash*/)
		{
			g_tPlayer.nShotCnt++;
			if (g_tPlayer.nShotCnt % SHOT_COOLTIME == 0)
			{
				if (g_tPlayer.fCharge > SHOT_COST)
				{
					Shot(BULLET_SHOT);
					g_tPlayer.fCharge -= SHOT_COST;
				}
				else if(g_tPlayer.nShotCnt %  ( SHOT_COOLTIME * 5 )== 0)
				{
					SoundPlay(SE_TAMAGIRE,0);
				}
			}
		}
		//ボタン離したら初期化
		if (GetKeyRelease(KEY_SHOT))
		{
			g_tPlayer.nShotCnt = 0;
		}
	}
	//単押し（テスト用）
	//if (GetKeyTrigger(KEY_SHOT))
	//{
	//	Shot();
	//}
	
	if (GetKeyTrigger(KEY_BOMB) && g_tPlayer.nState != PS_Dash)
	{
		if (g_tPlayer.fCharge > BOMB_COST && g_tPlayer.nBombCnt > BOMB_COOLTIME)
		{
			Shot(BULLET_BOMB);
			g_tPlayer.fCharge -= BOMB_COST;
			g_tPlayer.nBombCnt = 0;
		}
	}
	else
	{
		if (g_tPlayer.nBombCnt > BOMB_COOLTIME)
		{
			g_tPlayer.nBombCnt = BOMB_COOLTIME + 10;
		}
		else
		{
			g_tPlayer.nBombCnt++;
		}
	}
	

	//座標更新
	//X座標制御
	g_tPlayer.fPosX += g_tPlayer.fSpeedX;

	//Y座標制御
	if (!g_tPlayer.bWallUp)
	{
		g_tPlayer.fSpeedY += VALUE_GRAVITY;
	}
	g_tPlayer.fPosY += g_tPlayer.fSpeedY;
	/*
	毎フレーム重力分下にスピードを増やしていく(gtの式に近い)
	ジャンプしたときスピードはY-方向（上）にかかるが、それが毎フレームごとに重力分減っていくことによって
	頂点付近で減速、そこからY+方向(下)に加速していく
	*/

	//着地
	if (g_tPlayer.fPosY >= SCREEN_BOTTOM - (g_tPlayer.fSizeY - 0.5f))
	{
		g_tPlayer.fPosY = SCREEN_BOTTOM - g_tPlayer.fSizeY;
		g_tPlayer.fSpeedY = 0.0f;
		g_tPlayer.bJump = false;
		g_tPlayer.bJumpEnd = false;
		g_tPlayer.bDoubleJump = false;
	}

	//地形との当たり判定
	if (PlayerCollisionField(&g_tPlayer) || PlayerCollisionSpawnerF(&g_tPlayer))
	{
		g_tPlayer.fSpeedY = 0.0f;
		g_tPlayer.bJump = false;
		g_tPlayer.bJumpEnd = false;
		g_tPlayer.bLand = true;
		g_tPlayer.bDoubleJump = false;
	}
	else
	{
		if (g_tPlayer.bLand)
		{
			g_tPlayer.bJump = true;
			g_tPlayer.bLand = false;
		}
	}

	if (GetMap()[((int)(g_tPlayer.fPosY - 1) * MAP_X + (int)g_tPlayer.fPosX - 1) + 2] == TILE_PlayerColor || GetMap()[((int)(g_tPlayer.fPosY - 1) * MAP_X + (int)g_tPlayer.fPosX - 1) - 1] == TILE_PlayerColor)
	{
		if (g_tPlayer.nState == PS_Dash)
		{
			g_tPlayer.bWall = true;
		}
		else
		{
			g_tPlayer.bWall = false;
		}
	}
	else
	{
		g_tPlayer.bWall = false;
	}

	if (PlayerCollisionCeiling(&g_tPlayer) || PlayerCollisionSpawnerC(&g_tPlayer))
	{
		g_tPlayer.fSpeedY = VALUE_GRAVITY;
		g_tPlayer.fPosY += 0.2f;
		g_tPlayer.bWall = false;
	}
	if (PlayerCollisionRight(&g_tPlayer) || PlayerCollisionSpawnerR(&g_tPlayer))
	{
		g_tPlayer.fSpeedX = 0.0f;
	}


	if (PlayerCollisionLeft(&g_tPlayer) || PlayerCollisionSpawnerL(&g_tPlayer))
	{
		g_tPlayer.fSpeedX = 0.0f;
	}

	//座標制御
	if (g_tPlayer.fPosX < SCREEN_LEFT)
	{
		g_tPlayer.fPosX = SCREEN_LEFT;
	}
	if (g_tPlayer.fPosX > MAP_X - 2)
	{
		g_tPlayer.fPosX = MAP_X - 2;
	}

	if (g_tPlayer.fPosY < SCREEN_TOP + 1)
	{
		g_tPlayer.fPosY = SCREEN_TOP + 1;
		g_tPlayer.fSpeedY = VALUE_GRAVITY;
	}

	if (!g_tPlayer.bHit && g_tPlayer.nRespawn == 0)
	{
		g_tPlayer.bHit = PlayerCollisionEnemy(&g_tPlayer);
		if (g_tPlayer.bHit)
		{
			g_tPlayer.nHP--;
		}
	}

	if (g_tPlayer.bExplHit)
	{
		g_tPlayer.nHP -= 2;
		g_tPlayer.bHit = true;
		g_tPlayer.bExplHit = false;
		if (g_tPlayer.nHP <= 0)
		{
			g_tPlayer.nHP = 0;
		}
	}

	//死亡
	if (g_tPlayer.nHP <= 0)
	{
		g_tPlayer.nRespawn++;
		if (g_tPlayer.nRespawn == 1)
		{
			CreateAnimation(EXPLOSION_FRAME,g_tPlayer.fPosX,g_tPlayer.fPosY,A_EcolorExpl);
		}
		if (g_tPlayer.nRespawn >= PLAYER_RESPAWN_TIME)
		{
			//復活
			g_tPlayer.nHP = PLAYER_HP_MAX;
			g_tPlayer.nRespawn = 0;
			g_tPlayer.bHit = true;
		}
	}

	if (g_tPlayer.bHit)
	{
		g_tPlayer.nMutekiCnt++;
	}

	if (g_tPlayer.nMutekiCnt >= PLAYER_MUTEKIJIKAN)
	{
		g_tPlayer.bHit = false;
		g_tPlayer.nMutekiCnt = 0;
	}

	if (g_tPlayer.nRespawn == 0)
	{
		g_tPlayer.bVisible = Blinking(g_tPlayer.nMutekiCnt);
	}
	else
	{
		g_tPlayer.bVisible = Blinking(g_tPlayer.nRespawn);
	}

	if (g_tPlayer.bUp && g_tPlayer.fSpeedY > 0)
	{
		g_tPlayer.bUp = false;
	}
}

void DrawPlayer(void)
{
	if ((g_tPlayer.fPosX != g_tPlayer.fPosOldX) ||
		(g_tPlayer.fPosY != g_tPlayer.fPosOldY))
	{
		SetMapOffset(g_tPlayer.fPosOldX, g_tPlayer.fPosOldY,TILE_Blank);
		SetMapOffset(g_tPlayer.fPosOldX + 1, g_tPlayer.fPosOldY,TILE_Blank);
	}

	if (g_tPlayer.bVisible)
	{
		switch (g_tPlayer.nState)
		{
		case PS_Idle:
			SetMapOffset(g_tPlayer.fPosX, g_tPlayer.fPosY, TILE_Player);
			SetMapOffset(g_tPlayer.fPosX + 1, g_tPlayer.fPosY, TILE_Zenkaku);
			break;

		case PS_Dash:
			SetMapOffset(g_tPlayer.fPosX, g_tPlayer.fPosY, TILE_P_Dash);
			SetMapOffset(g_tPlayer.fPosX + 1, g_tPlayer.fPosY, TILE_Zenkaku);
			break;

		default:
			break;
		}
	}
	else
	{
		SetMapOffset(g_tPlayer.fPosX, g_tPlayer.fPosY, TILE_Blank);
		SetMapOffset(g_tPlayer.fPosX + 1, g_tPlayer.fPosY, TILE_Blank);
	}
}

PLAYER* GetPlayer(void)
{
	return &g_tPlayer;
}

void Shot(int nBulletType)
{
	if (isMouse)
	{//マウス使用(コントローラー未接続時)
		float mouseX = inport(PM_CURX);
		float mouseY = inport(PM_CURY);

		//ウィンドウ内におけるプレイヤーの位置を算出
		float fWindowPlayerPosX;
		if (g_tPlayer.fPosX >= WINDOW_CENTERX && g_tPlayer.fPosX + WINDOW_CENTERX <= MAP_X)
		{//プレイヤーが中央の時
			fWindowPlayerPosX = WINDOW_CENTERX;
		}
		else if (g_tPlayer.fPosX < WINDOW_CENTERX)
		{//左端
			fWindowPlayerPosX = g_tPlayer.fPosX;
		}
		else
		{//右端
			//プレイヤーの座標からwindowの中央の座標(MAP_X - WINDOW_CENTERX)を引くことで中央～プレイヤー間の距離を算出、それを中央の座標に足すことで求める
			fWindowPlayerPosX = WINDOW_CENTERX + (g_tPlayer.fPosX - (MAP_X - WINDOW_CENTERX));
		}

		//マウスの位置と中央の位置からangleを算出(終点-始点)
		g_tPlayer.angle.x = mouseX - fWindowPlayerPosX;
		g_tPlayer.angle.y = mouseY - g_tPlayer.fPosY;

	}
	else
	{//コントローラー接続時

		int fPadX = inport(PJ1_ZPOS);
		int fPadY = inport(PJ1_RPOS);

		//int padX = inport(PJ1_XPOS);
		//int padY = inport(PJ1_YPOS);
		////int padZ = inport(PJ1_ZPOS);
		////int padR = inport(PJ1_RPOS);
		//int padU = inport(PJ1_UPOS);
		//int padV = inport(PJ1_VPOS);

		//gotoxy(2, 4);
		//std::cout << padX << "     ";
		//gotoxy(2, 5);
		//std::cout << padY << "     ";


		//gotoxy(2, 6);
		//std::cout << fPadX << "     ";
		//gotoxy(2, 7);
		//std::cout << fPadY << "     ";

		////gotoxy(2, 6);
		////std::cout << padZ << "     ";
		////gotoxy(2, 7);
		////std::cout << padR << "     ";

		//gotoxy(2, 8);
		//std::cout << padU << "     ";
		//gotoxy(2, 9);
		//std::cout << padV << "     ";

//#ifdef _DEBUG
		//releaseだとMAX255で出るためこれで補正
		fPadX /= 256;
		fPadY /= 256;
//#endif // _DEBUG

		fPadX -= 127;
		fPadY -= 127;

		//デッドゾーンの設定
		if (abs(fPadX) >= CONT_DEADZONE && abs(fPadY) >= CONT_DEADZONE)
		{
			g_tPlayer.angle.x = fPadX;
			g_tPlayer.angle.y = fPadY;
		}


	}

	//ベクトルを正規化する
	VECTOR2 shotDir = Vec2_Normarize(g_tPlayer.angle);

	switch (nBulletType)
	{
	case BULLET_SHOT:
			CreateBullet(g_tPlayer.fPosX, g_tPlayer.fPosY, shotDir.x, shotDir.y);
			break;

	case BULLET_BOMB:
			CreateBomb(g_tPlayer.fPosX, g_tPlayer.fPosY, shotDir.x, shotDir.y);
			break;

	default:
		break;
	}
}