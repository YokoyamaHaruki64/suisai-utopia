#define CONIOEX
#include "conioex.h"
#include <iostream>
#include <fstream>
#include "define.h"
#include "color.h"

#define BLINKING_INTERVAL	6

VECTOR2 AngleToVector2(float fAngle)
{

	//float dir_x = -cosf(fAngle);	//X方向
	//float dir_y = -sinf(fAngle);											//Y方向

	//ベクトルを作る
	float dir_x = (abs(fAngle - 180.0f)) / 180.0f * 4 - 2;	//X方向
	float dir_y;											//Y方向

	//Y方向は二つのパターンに分けて算出
	if (fAngle <= 270)
	{
		dir_y = (abs(fAngle - 90.0f)) / 180.0f * 2 - 1;
	}
	else
	{
		dir_y = (abs(360.0f - fAngle)) / 90.0f;
	}

	VECTOR2 ret = { dir_x,dir_y };

	//ベクトルを正規化する
	ret = Vec2_Normarize(ret);
	return ret;
}

VECTOR2 Vec2_Normarize(VECTOR2 vec)
{
	float fMagnitude = Vec2_Magnitude(vec);
	vec.x = vec.x / fMagnitude;
	vec.y = vec.y / fMagnitude;

	return vec;
}
VECTOR2 RandomVector()
{
	VECTOR2 ret = {RandomRange(0,100),RandomRange(0,100)};
	if (_1d2() == 1)
	{
		ret.x *= -1;
	}
	if (_1d2() == 1)
	{
		ret.y *= -1;
	}
	return Vec2_Normarize(ret);
}


float Vec2_Magnitude(VECTOR2 vec)
{
	return sqrtf((vec.x * vec.x) + (vec.y * vec.y));//ベクトルの大きさ
}

int ClampInt(int nMin, int nMax, int nNum)
{
	if (nMin >= nNum)
	{
		return nMin;
	}
	if (nMax <= nNum)
	{
		return nMax;
	}

	return nNum;
}

int StringToInt(char* str)
{
	int ret = 0;
	int length = strlen(str);

	if (length > 0)
	{
		for (int nCnt = 0; nCnt < length; nCnt++)
		{
			ret = ret * 10 + (str[nCnt] - 48);
		}
	}
	else
	{
		ret = -1;
	}

	return ret;

}

void ReadCSV(int nSize_x,int nSize_y,int* pnArray,const char* sFileName)
{
	std::string str;

	std::ifstream file; // CSVファイルを開く
	file.open(sFileName);

	if (file)
	{
		char dustBox[1000];
		file >> dustBox;//１行目のカスを捨てる

		//ファイル読み込み
		for (int y = 0; y < nSize_y; y++)
		{
			std::string sWork;
			file >> sWork;
			str += sWork + ",";//後の処理の都合上最後の文字の後に,を足す
		}

		int mapPointer = 0;
		//データ加工
		for (int y = 0; y < nSize_y; y++)
		{
			for (int x = 0; x < nSize_x; x++)
			{
				bool bLoop = true;
				int nCnt = 0;
				char sWork[16];//16は1データの最大桁数
				memset(sWork, 0, sizeof(char) * 16);

				while (bLoop)
				{
					if (str[mapPointer] == ',')
					{
						bLoop = false;
					}
					else
					{
						sWork[nCnt] = str[mapPointer];
					}
					mapPointer++;
					nCnt++;
				}
				pnArray[y * nSize_x + x] = StringToInt(sWork);
			}
		}
		file.close();
	}
	else
	{
		std::cout << "ファイルを開けませんでした。" << std::endl;
	}

}

void ReadCSV_Anim(int nSize_x, int nSize_y,int nFrame, int* pnArray, const char* sFileName)
{

	std::fstream file; // CSVファイルを開く
	file.open(sFileName);

	if (file)
	{
		char dustBox[1000];
		file >> dustBox;//１行目のカスを捨てる

		for (int nFrameCnt = 0; nFrameCnt < nFrame; nFrameCnt++)
		{
			std::string str;
			//ファイル読み込み
			for (int y = 0; y < nSize_y; y++)
			{
				std::string sWork;
				file >> sWork;
				str += sWork + ",";//後の処理の都合上最後の文字の後に,を足す
			}
			file >> dustBox;//間のカスを捨てる

			int mapPointer = 0;
			//データ加工
			for (int y = 0; y < nSize_y; y++)
			{//1行単位
				for (int x = 0; x < nSize_x; x++)
				{//1セル単位
					bool bLoop = true;
					int nCnt = 0;
					char sWork[16];//16は1データの最大桁数
					memset(sWork, 0, sizeof(char) * 16);

					//,が来るまでデータを読み込んでsWorkに格納する
					while (bLoop)
					{
						if (str[mapPointer] == ',')
						{
							bLoop = false;
						}
						else
						{
							sWork[nCnt] = str[mapPointer];
						}
						mapPointer++;
						nCnt++;
					}
					//sWorkを指定の位置に書き込む
					pnArray[(nFrameCnt * nSize_x * nSize_y) + (y * nSize_x) + x] = StringToInt(sWork);
				}
			}
		}

		file.close();
	}
	else
	{
		std::cout << "ファイルを開けませんでした。" << std::endl;
	}

}

int RandomRange(int min,int MAX)
{
	return rand() % (MAX - min + 1) + min;
}

int _1d2()
{
	return rand() % 2 + 1;
}

float Normarize0to1(float min,float max,float num)
{
	return (num - min) / (max - min);
}

bool Blinking(int nCount)
{
	int nWork = nCount / BLINKING_INTERVAL;

	if (nWork % 2 == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}