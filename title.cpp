#define CONIOEX
#include "conioex.h"
#include "inputKey.h"
#include "scene.h"
#include "animation.h"
#include "drawWindow.h"
#include "sound.h"
#include <iostream>

enum MENU
{
	ToGame,
	ToPractice,

	MENU_MAX
};

int g_SelectMenu;

void InitTitle(void)
{
	//ƒƒjƒ…[\¬‚ÌŠÇ—@‘±‚«‚©‚ç‚ª‚ ‚é‚©`“™
	std::cout << "\x1b[48;2;0;0;0m";
	clrscr();

	InitAnimation();
	CreateAnimation(TITLE_FRAME,0,0,A_Title);
	InitDrawWindow();
	SoundPlay(TITLEBGM,LOOP);
}
void UnInitTitle(void)
{
	//BGM‚ğ~‚ß‚éˆ—‚Æ‚©
	UnInitAnimation();
	SoundEnd(TITLEBGM);
}

void UpdateTitle(void)
{
	UpdateAnimation();

	if (GetKeyTrigger(KEY_ENTER) || GetKeyTrigger(KEY_SHOT))
	{
		SoundPlay(SE_ENTER, 0);
		msleep(500);

		switch (g_SelectMenu)
		{
		case ToGame:
			SetScene(SCENE_GAME);
			break;

		case ToPractice:
			SetScene(SCENE_PRACTICE);
			break;

		default:
			break;
		}

	}

	if (GetKeyTrigger(KEY_UP))
	{
		g_SelectMenu--;
		SoundPlay(SE_CURSOL,NOTLOOP);
	}
	else if (GetKeyTrigger(KEY_DOWN))
	{
		g_SelectMenu++;
		SoundPlay(SE_CURSOL, NOTLOOP);
	}

	if (g_SelectMenu >= MENU_MAX)
	{
		g_SelectMenu = MENU_MAX - 1;
	}
	else if (g_SelectMenu < 0)
	{
		g_SelectMenu = 0;
	}
}


void DrawTitle(void)
{
	DrawAnimation();

	UpdateDrawWindow();
	DrawDrawWindow();

	std::cout << "\x1b[38;2;0;0;0m";
	switch (g_SelectMenu)
	{
	case ToGame:
		gotoxy(5, 22);
		std::cout << ConvertSkyColor(22);
		std::cout << u8"\u25B6 ƒQ[ƒ€‚ğ‚Í‚¶‚ß‚é";

		gotoxy(5, 24);
		std::cout << ConvertSkyColor(24);
		std::cout << u8"  ‘€ì—ûK‚ğ‚Í‚¶‚ß‚é";

		break;

	case ToPractice:
		gotoxy(5, 22);
		std::cout << ConvertSkyColor(22);
		std::cout << u8"  ƒQ[ƒ€‚ğ‚Í‚¶‚ß‚é";

		gotoxy(5, 24);
		std::cout << ConvertSkyColor(24);
		std::cout << u8"\u25B6 ‘€ì—ûK‚ğ‚Í‚¶‚ß‚é";
		break;

	default:
		break;
	}

}