#define CONIOEX
#include "conioex.h"

#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "practice.h"

//
// グローバル宣言
// 

SCENE g_Scene = SCENE_TITLE;		//現在動作中のシーン
SCENE g_SceneNext = SCENE_TITLE;	//移行先のシーン

// g_が付いてる奴はグローバル変数


//シーンの初期化
void InitScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		//タイトル画面初期化
		InitTitle();
		break;
	case SCENE_GAME:
		//ゲーム画面初期化
		InitGame();
		break;
	case SCENE_RESULT:
		//リザルト画面の初期化
		InitResult();
		break;
	case SCENE_PRACTICE:
		//リザルト画面の初期化
		InitPractice();
		break;
	default:
		break;
	}

}

void UnInitScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		//タイトル画面終了処理
		UnInitTitle();
		break;
	case SCENE_GAME:
		//ゲーム画面終了処理
		UnInitGame();
		break;
	case SCENE_RESULT:
		//リザルト画面終了処理
		UnInitResult();
		break;
	case SCENE_PRACTICE:
		//リザルト画面の初期化
		UnInitPractice();
		break;
	default:
		break;
	}

}

void UpdataScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		//タイトル画面更新処理
		UpdateTitle();
		break;
	case SCENE_GAME:
		//ゲーム画面更新処理
		UpdateGame();
		break;
	case SCENE_RESULT:
		//リザルト画面更新処理
		UpdateResult();
		break;
	case SCENE_PRACTICE:
		//リザルト画面の初期化
		UpdatePractice();
		break;
	default:
		break;
	}

}

void DrawScene(void)
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		//タイトル画面描画処理
		DrawTitle();
		break;
	case SCENE_GAME:
		//ゲーム画面描画処理
		DrawGame();
		break;
	case SCENE_RESULT:
		//リザルト画面描画処理
		DrawResult();
		break;
	case SCENE_PRACTICE:
		//リザルト画面の初期化
		DrawPractice();
		break;
	default:
		break;
	}

}

SCENE GetScene()
{
	return g_Scene;
}

void SetScene(SCENE next)
{
#ifdef _DEBUG
	// テキストファイル出力
#endif
	g_SceneNext = next;
}

void CheckScene(void)
{
	if (g_Scene != g_SceneNext)
	{
		//現在動作中のシーンを終了する
		UnInitScene();

		//現在動作中のシーンを移行先のシーンで更新する
		g_Scene = g_SceneNext;

		//更新したシーンの初期化を行う
		InitScene();

		//画面のクリア
		//clrscr();
	}
}



