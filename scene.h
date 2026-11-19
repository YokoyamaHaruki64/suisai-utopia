#pragma once
enum SCENE
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_PRACTICE,

	SCENE_NUM,
};

void InitScene(void);
void UnInitScene(void);
void UpdataScene(void);
void DrawScene(void);

SCENE GetScene();
void SetScene(SCENE next);
void CheckScene(void);
