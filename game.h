#pragma once
#define ACTION
//#define SHOOTING

#define PLAYER_COLOR	1
#define ENEMY_COLOR		2

//マクロ定義
#define SCREEN_LEFT		1		//画面の大きさを定義 (左端)
#define SCREEN_RIGHT	80		//画面の大きさを定義 (右端)
#define SCREEN_TOP		1		//画面の大きさを定義 (上端)
#define SCREEN_BOTTOM	25		//画面の大きさを定義 (下端)
#define MAP_X			200		//マップサイズ

#define MOVE_SPEED_X (0.5)		//X座標の移動量(全体)
#define MOVE_SPEED_Y (0.25)		//Y座標の移動量(全体)

#define VALUE_RESIST_X		 (0.5)	  //地上での移動摩擦抵抗
#define VALUE_RESIST_X_AIR	 (0.1)	  //空中での移動摩擦抵抗
#define VALUE_GRAVITY		 (0.035)  //重力

#define TIMELIMIT	(60)			  //制限時間(秒)

void InitGame(void);
void UnInitGame(void);
void UpdateGame(void);
void DrawGame(void);
