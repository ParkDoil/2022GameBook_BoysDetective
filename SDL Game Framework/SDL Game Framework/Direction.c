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
	// ���� �߰��� ������
	uint8     ImageAlpha;
} SceneData;

void Direction_DataSetting(int32 DirectionNumber) // int32 = ���� ��ȣ (�ش� �ε����� ���� ��ȣ �Ľ��ؼ� �ֱ�)
{
	switch (DirectionNumber)
	{
	case 0: // �⺻ ��
		data.ImageAlpha = 255;
		break;
	case 1: // ���̵��� ���� (��ο� ȭ�鿡�� ���� ������� ����)
		data.ImageAlpha = 0;
		break;
	case 2: // ���̵�ƿ� ���� (���� ȭ�鿡�� ���� ��ο����� ����)
		data.ImageAlpha = 255;
		break;
	}

	if (SOUND_NAME[nowIndex] != NULL) // SOUND_NAME �Ľ��ؼ� �ֱ�
	{
		Music* SceneMusic;
		char MusicName[] = parsing_dt->sceneData->SOUND_NAME[nowIndex];
		Audio_LoadMusic(&SceneMusic, MusicName);
		Audio_Play(&SceneMusic, INFINITY_LOOP);
	}
}

void Direction_DataChange(int32 DirectionNumber) // int32 = ���� ��ȣ (�ش� �ε����� ���� ��ȣ �Ľ��ؼ� �ֱ�)
{
	switch (DirectionNumber)
	{
	case 0: // �⺻ ��
		data.ImageAlpha = 255;
		break;
	case 1: // ���̵��� ���� (��ο� ȭ�鿡�� ���� ������� ����)
		data.ImageAlpha += (SECOND * GAMEFRAME) / GAMEFRAME;
		break;
	case 2: // ���̵�ƿ� ���� (���� ȭ�鿡�� ���� ��ο����� ����)
		data.ImageAlpha -= (SECOND * GAMEFRAME) / GAMEFRAME;
		break;
	}
}