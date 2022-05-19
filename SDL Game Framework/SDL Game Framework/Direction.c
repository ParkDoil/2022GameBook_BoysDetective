#include "Direction.h"
#include "Framework/Audio.h"

parsing parsing_dt;

#define GAMEFRAME 60
#define SECOND 3

typedef struct SceneData
{
	int32      nowIndex;
	int32      ChooseCount;
	int32      LineCount;
	int32      delayCount;
	Image      BackGround;
	Text      GuideLine[20];
	Text      Choose_1;
	Text      Choose_2;
	Text      Choose_3;
	Text      Coursur;
	bool      isUp;
	bool      isDown;
	bool      isSkip;
	// 내가 추가한 데이터
	uint8     ImageAlpha;
} SceneData;

void Direction_DataSetting(int32 DirectionNumber) // int32 = 연출 번호 (해당 인덱스의 연출 번호 파싱해서 넣기)
{
	switch (DirectionNumber)
	{
	case 0: // 기본 값
		data.ImageAlpha = 255;
		break;
	case 1: // 페이드인 연출 (어두운 화면에서 점점 밝아지는 연출)
		data.ImageAlpha = 0;
		break;
	case 2: // 페이드아웃 연출 (밝은 화면에서 점점 어두워지는 연출)
		data.ImageAlpha = 255;
		break;
	}

	if (SOUND_NAME[nowIndex] != NULL) // SOUND_NAME 파싱해서 넣기
	{
		Music* SceneMusic;
		char MusicName[] = parsing_dt->sceneData->SOUND_NAME[nowIndex];
		Audio_LoadMusic(&SceneMusic, MusicName);
		Audio_Play(&SceneMusic, INFINITY_LOOP);
	}
}

void Direction_DataChange(int32 DirectionNumber) // int32 = 연출 번호 (해당 인덱스의 연출 번호 파싱해서 넣기)
{
	switch (DirectionNumber)
	{
	case 0: // 기본 값
		data.ImageAlpha = 255;
		break;
	case 1: // 페이드인 연출 (어두운 화면에서 점점 밝아지는 연출)
		data.ImageAlpha += (SECOND * GAMEFRAME) / GAMEFRAME;
		break;
	case 2: // 페이드아웃 연출 (밝은 화면에서 점점 어두워지는 연출)
		data.ImageAlpha -= (SECOND * GAMEFRAME) / GAMEFRAME;
		break;
	}
}