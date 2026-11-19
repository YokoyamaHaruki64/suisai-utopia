#pragma once

#define LOOP		1
#define NOTLOOP		0

#define SOUND_RANGE	50

enum SOUNDNAME
{
	TITLEBGM,
	BGM,
	RESULTBGM,
	SE_EXPLOSION,
	SE_MAPFILL,
	SE_ENTER,
	SE_TITLE_FIRE,
	SE_GAMEEND,
	SE_CURSOL,
	SE_TAMAGIRE,

	SOUND_MAX
};

void InitSound();
void UnInitSound();

void SoundPlay(SOUNDNAME nName, int nLoop);
void SoundVolume(SOUNDNAME nName, int nVolume);
void SoundEnd(SOUNDNAME nName);
