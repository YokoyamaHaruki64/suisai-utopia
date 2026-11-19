#pragma once
#define CONIOEX
#include "conioex.h"
#include <iostream>

#define UI_BAR_POSX 13
#define UI_BAR_POSY 1
#define UI_BAR_LENGTH 25

#define UI_TANK_POSX 73
#define UI_TANK_POSY 20

void InitUI();
void UnInitUI();
void UpdateUI();
void DrawUI();

std::string StringVSBar();
std::string StringTime();
std::string StringCharge();
std::string StringHPBar();

int* GetColorCount();
int GetTimeLimit();
int GetGameTime();
