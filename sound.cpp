#define CONIOEX
#include "conioex.h"
#include "sound.h"

#define BGM_TITLE "titleBGM.mp3"
#define BGM_GAME "BGM.mp3"
#define BGM_RESULT "resultBGM.mp3"
#define SEF_EXPLOSION "explosionSE.mp3"
#define SEF_FILL "paintSE.mp3"
#define SEF_ENTER "enter.mp3"
#define SEF_TITLEFIRE "title_Fire.mp3"
#define SEF_GAMEEND "gameEnd.mp3"
#define SEF_CURSOL "cursol.mp3"
#define SEF_TAMAGIRE "tamagire.mp3"


//#define Mute

int g_SoundHandle[SOUND_MAX];

void InitSound()
{
	char fileName[][64] = {
		{BGM_TITLE},
		{BGM_GAME},
		{BGM_RESULT},
		{SEF_EXPLOSION},
		{SEF_FILL},
		{SEF_ENTER},
		{SEF_TITLEFIRE},
		{SEF_GAMEEND},
		{SEF_CURSOL},
		{SEF_TAMAGIRE}
	};

	for (int nCnt = 0; nCnt < SOUND_MAX; nCnt++)
	{
		g_SoundHandle[nCnt] = opensound(fileName[nCnt]);
	}

	SoundVolume(TITLEBGM, 60);
	SoundVolume(BGM, 20);
	SoundVolume(RESULTBGM, 60);
	SoundVolume(SE_ENTER, 8);
	SoundVolume(SE_TITLE_FIRE, 10);
	SoundVolume(SE_MAPFILL, 60);
	SoundVolume(SE_EXPLOSION, 90);
	SoundVolume(SE_CURSOL, 20);
	SoundVolume(SE_TAMAGIRE, 60);
#ifdef Mute
	for (int nCnt = 0; nCnt < SOUND_MAX; nCnt++)
	{
		SoundVolume((SOUNDNAME)nCnt,0);
	}

#endif // Mute

}

void UnInitSound()
{
	for (int nCnt = 0;nCnt < SOUND_MAX;nCnt++)
	{
		closesound(g_SoundHandle[nCnt]);
	}
}

void SoundPlay(SOUNDNAME nName,int nLoop)
{
	playsound(g_SoundHandle[nName],nLoop);
}

void SoundVolume(SOUNDNAME nName, int nVolume)
{
	setvolume(g_SoundHandle[nName], nVolume);
}

void SoundEnd(SOUNDNAME nName)
{
	stopsound(g_SoundHandle[nName]);
}