#include "stdafx.h"
#include "Scene.h"

#include "Framework.h"

#define SOLID 0
#define SHADED 1
#define BLENDED 2
#define SECOND 2

Scene g_Scene;
Parsing parsing_dt;

static ESceneType s_nextScene = SCENE_NULL;
static int32 Index = 0;
static float Volume = 0.35f;
static float TextDelay = 1.2f;

// 교수님의 titleScene
/*
#pragma region TitleScene


const wchar_t* str[] = {
	L"여기는 타이틀씬입니다. 텍스트와 관련된 여러가지를 테스트해봅시다.",
	L"B키를 누르면 폰트가 굵게 변합니다.",
	L"I키를 누르면 폰트가 이탤릭체로 변합니다.",
	L"U키를 누르면 텍스트에 밑줄이 생깁니다.",
	L"S키를 누르면 텍스트에 취소선이 생깁니다.",
	L"N키를 누르면 다시 원래대로 돌아옵니다.",
	L"C키를 누르면 렌더 모드가 바뀝니다. (Solid -> Shaded -> Blended)",
	L"1키를 누르면 텍스트가 작아집니다.",
	L"2키를 누르면 텍스트가 커집니다.",
	L"스페이스 키를 누르면 다음 씬으로 넘어갑니다.",
};

typedef struct TitleSceneData
{
	Text	GuideLine[10];
	Text	TestText;
	int32	FontSize;
	int32	RenderMode;
	Image	TestImage;
} TitleSceneData;

void init_title(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));

	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	for (int32 i = 0; i < 10; ++i)
	{
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, str[i], wcslen(str[i]));
	}

	data->FontSize = 24;
	Text_CreateText(&data->TestText, "d2coding.ttf", data->FontSize, L"이 텍스트가 변합니다.", 13);

	data->RenderMode = SOLID;

	Image_LoadImage(&data->TestImage, "Background.jfif");
}

void update_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('B'))
	{
		Text_SetFontStyle(&data->TestText, FS_BOLD);
	}

	if (Input_GetKeyDown('I'))
	{
		Text_SetFontStyle(&data->TestText, FS_ITALIC);
	}

	if (Input_GetKeyDown('U'))
	{
		Text_SetFontStyle(&data->TestText, FS_UNDERLINE);
	}

	if (Input_GetKeyDown('S'))
	{
		Text_SetFontStyle(&data->TestText, FS_STRIKETHROUGH);
	}

	if (Input_GetKeyDown('N'))
	{
		Text_SetFontStyle(&data->TestText, FS_NORMAL);
	}

	if (Input_GetKeyDown('C'))
	{
		data->RenderMode = (data->RenderMode + 1) % 3;
	}

	if (Input_GetKey('1'))
	{
		--data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
	}

	if (Input_GetKey('2'))
	{
		++data->FontSize;
		Text_SetFont(&data->TestText, "d2coding.ttf", data->FontSize);
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_MAIN);
	}
}

void render_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	for (int32 i = 0; i < 10; ++i)
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 10, 20 * i, color);
	}
	
	switch (data->RenderMode)
	{
	case SOLID:
	{
		SDL_Color color = { .a = 255 };
		Renderer_DrawTextSolid(&data->TestText, 400, 400, color);
	}
	break;
	case SHADED:
	{
		SDL_Color bg = { .a = 255 };
		SDL_Color fg = { .r = 255, .g = 255, .a = 255 };
		Renderer_DrawTextShaded(&data->TestText, 400, 400, fg, bg);
	}
	break;
	case BLENDED:
	{
		Renderer_DrawImage(&data->TestImage, 400, 400);
		SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
		Renderer_DrawTextBlended(&data->TestText, 400, 400, color);
	}
	break;
	}
}

void release_title(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 10; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}
	Text_FreeText(&data->TestText);

	Image_FreeImage(&data->TestImage);

	SafeFree(g_Scene.Data);
}
#pragma endregion
*/
// 교수님의 mainScene
/*
#pragma region MainScene
const wchar_t* str2[] = {
	L"여기서는 사운드와 이미지 블렌딩에 대해서 알아봅시다.",
	L"화살표키로 이미지를 이동시킬 수 있습니다.",
	L"E키를 누르면 이펙트를 재생시킬 수 있습니다. 이펙트 소리가 작으니 볼륨을 낮춘 후 진행하세요.",
	L"M키로 음악을 끄거나 켤 수 있습니다.",
	L"P키로 음악을 멈추거나 재개할 수 있습니다.",
	L"1번과 2번으로 볼륨을 조절할 수 있습니다.",
	L"WASD로 이미지의 스케일을 조정할 수 있습니다.",
	L"KL키로 이미지의 투명도를 조절할 수 있습니다."
};

#define GUIDELINE_COUNT 8

typedef struct MainSceneData
{
	Text		GuideLine[GUIDELINE_COUNT];
	Music		BGM;
	float		Volume;
	SoundEffect Effect;
	Image		BackGround;
	float		Speed;
	int32		X;
	int32		Y;
	int32		Alpha;
} MainSceneData;

void logOnFinished(void)
{
	LogInfo("You can show this log on stopped the music");
}

void log2OnFinished(int32 channel)
{
	LogInfo("You can show this log on stopped the effect");
}

void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		Text_CreateText(&data->GuideLine[i], "d2coding.ttf", 16, str2[i], wcslen(str2[i]));
	}

	Image_LoadImage(&data->BackGround, "background.jfif");

	Audio_LoadMusic(&data->BGM, "powerful.mp3");
	Audio_HookMusicFinished(logOnFinished);
	Audio_LoadSoundEffect(&data->Effect, "effect2.wav");
	Audio_HookSoundEffectFinished(log2OnFinished);
	Audio_PlayFadeIn(&data->BGM, INFINITY_LOOP, 3000);

	data->Volume = 1.0f;

	data->Speed = 400.0f;
	data->X = 400;
	data->Y = 400;
	data->Alpha = 255;
}

void update_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	if (Input_GetKeyDown('E'))
	{
		Audio_PlaySoundEffect(&data->Effect, 1);
	}

	if (Input_GetKeyDown('M'))
	{
		if (Audio_IsMusicPlaying())
		{
			Audio_Stop();
		}
		else
		{
			Audio_Play(&data->BGM, INFINITY_LOOP);
		}
	}

	if (Input_GetKeyDown('P'))
	{
		if (Audio_IsMusicPaused())
		{
			Audio_Resume();
		}
		else
		{
			Audio_Pause();
		}
	}

	if (Input_GetKey('1'))
	{
		data->Volume -= 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey('2'))
	{
		data->Volume += 0.01f;
		Audio_SetVolume(data->Volume);
	}

	if (Input_GetKey(VK_DOWN))
	{
		data->Y += data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_UP))
	{
		data->Y -= data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_LEFT))
	{
		data->X -= data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey(VK_RIGHT))
	{
		data->X += data->Speed * Timer_GetDeltaTime();
	}

	if (Input_GetKey('W'))
	{
		data->BackGround.ScaleY -= 0.05f;
	}

	if (Input_GetKey('S'))
	{
		data->BackGround.ScaleY += 0.05f;
	}

	if (Input_GetKey('A'))
	{
		data->BackGround.ScaleX -= 0.05f;
	}

	if (Input_GetKey('D'))
	{
		data->BackGround.ScaleX += 0.05f;
	}

	if (Input_GetKey('K'))
	{
		data->Alpha = Clamp(0, data->Alpha - 1, 255);
		Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}

	if (Input_GetKey('L'))
	{
		data->Alpha = Clamp(0, data->Alpha + 1, 255);
		Image_SetAlphaValue(&data->BackGround, data->Alpha);
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_START);
	}
}

void render_main(void)
{

	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		SDL_Color color = { .a = 0 };
		Renderer_DrawTextSolid(&data->GuideLine[i], 10, 20 * i, color);
	}

	Renderer_DrawImage(&data->BackGround, data->X, data->Y);
}

void release_main(void)
{
	MainSceneData* data = (MainSceneData*)g_Scene.Data;

	for (int32 i = 0; i < GUIDELINE_COUNT; ++i)
	{
		Text_FreeText(&data->GuideLine[i]);
	}
	Audio_FreeMusic(&data->BGM);
	Audio_FreeSoundEffect(&data->Effect);

	Image_FreeImage(&data->BackGround);

	SafeFree(g_Scene.Data);
}
#pragma endregion
*/ 

#pragma region extraScene

typedef struct SceneData
{
	int32			nowIndex;
	int32			ChooseCount;
	int32			LineCount;
	int32			delayCount;
	int32			blackoutAlpha;
	int32			FontSize;
	int32			TextAlpha;
	int32			TextCheck;
	float			EffectSoundDelay;
	float			elapsedTime;
	float			EffectTime;
	SoundEffect		EffectSound;
	Image			BackGround;
	Image			BlackOutImage;
	Music			Main_BGM;
	Text			GuideLine[30];
	Text			Choose_1;
	Text			Choose_2;
	Text			Choose_3;
	Text			Coursur;
	bool			isChoice_1;
	bool			isChoice_2;
	bool			isChoice_3;
	bool			isUp;
	bool			isDown;
	bool			isSkip;
	bool			goNextScene;
	bool			isPlayedEffetSound;
	bool			gotoMain;
} SceneData;

void init_Extra(void)
{
	g_Scene.Data = malloc(sizeof(SceneData));
	memset(g_Scene.Data, 0, sizeof(SceneData));
	SceneData* data = (SceneData*)g_Scene.Data;

	data->nowIndex = Index;

	data->ChooseCount = 0;
	data->LineCount = 1;
	data->delayCount = 0;
	data->FontSize = 22;
	data->TextCheck = 0;
	
	data->elapsedTime = 0.0f;
	data->EffectTime = 0.0f;

	data->EffectSoundDelay = parsing_dt.sceneData[data->nowIndex].EFFECT_COUNT;
	if (parsing_dt.sceneData[data->nowIndex].IMG_OUTPUT_STYLE == 2)
	{
		data->blackoutAlpha = 255;
		data->TextAlpha = 0;
	}
	else
	{
		data->blackoutAlpha = 0;
		data->TextAlpha = 255;
	}

	Text_CreateText(&data->Coursur, "Disital.ttf", 24, L" ▶", wcslen(L" ▶")); //커서 생성

	// 개행문자 처리부
	wchar_t* rawString = parsing_dt.sceneData[data->nowIndex].TEXT;
	wchar_t* lineStart = rawString;
	wchar_t* lineEnd = lineStart;

	for (int32 i = 0; rawString[i] != L'\0'; i++)
	{
		if (rawString[i] == L'\n') {
			data->LineCount++;
		}
	}

	//유니코드에 개행이 있을때 카운트해서 반복문 돌리기
	for (int32 i = 0; i < data->LineCount; ++i)
	{
		while (true)
		{
			if (*lineEnd == L'\n' || *lineEnd == L'\0')
			{
				break;
			}

			++lineEnd;
		}

		int32 len = lineEnd - lineStart;

		Text_CreateText(&data->GuideLine[i], "GmarketSansTTFLight.ttf", 18, lineStart, len);


		lineStart = lineEnd + 1;
		lineEnd = lineStart;
	}

	// 선택지 처리부
	if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1) != L'\0')
	{
		Text_CreateText(&data->Choose_1, "1.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1));
		data->ChooseCount++;
	}
	if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2) != L'\0')
	{
		Text_CreateText(&data->Choose_2, "1.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2));
		data->ChooseCount++;
	}
	if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3) != L'\0')
	{
		Text_CreateText(&data->Choose_3, "1.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3));
		data->ChooseCount++;
	}
	// 이미지 처리부
	Image_LoadImage(&data->BackGround, parsing_dt.sceneData[data->nowIndex].MAIN_IMAGE);
	Image_LoadImage(&data->BlackOutImage, "blackout.png");

	//사운드
	if (*parsing_dt.sceneData[data->nowIndex].SOUND_NAME != NULL)
	{
		Audio_LoadMusic(&data->Main_BGM, parsing_dt.sceneData[data->nowIndex].SOUND_NAME);
		Audio_GetVolume();
		Audio_SetVolume(Volume);
		Audio_Play(&data->Main_BGM, INFINITY_LOOP);
	}
	if (*parsing_dt.sceneData[data->nowIndex].EFFECT_SOUND_NAME != NULL)
	{
		Audio_LoadSoundEffect(&data->EffectSound, parsing_dt.sceneData[data->nowIndex].EFFECT_SOUND_NAME);
		Audio_SetEffectVolume(&data->EffectSound, 0.5f);
	}

	data->isUp = true;
	data->isDown = false;
	data->isSkip = false;
	data->goNextScene = false;
	data->isChoice_1 = true;
	data->isChoice_2 = false;
	data->isChoice_3 = false;
	data->isPlayedEffetSound = false;
	data->gotoMain = false;
}

void update_Extra(void)
{
	SceneData* data = (SceneData*)g_Scene.Data;

	data->elapsedTime += Timer_GetDeltaTime();
	data->EffectTime += Timer_GetDeltaTime();

	// PadeIn 처리
	if (parsing_dt.sceneData[data->nowIndex].IMG_OUTPUT_STYLE == 2)
	{
		if (data->TextAlpha < 255 && data->blackoutAlpha > 0)
		{
			data->TextAlpha += 5;
			data->blackoutAlpha -= 5;

		}
	}

	// 볼륨 처리부
	if (Input_GetKey('1'))
	{
		Volume -= 0.01f;
		Audio_SetVolume(Volume);
	}
	if (Input_GetKey('2'))
	{
		Volume += 0.01f;
		Audio_SetVolume(Volume);
	}
	if (Input_GetKeyDown('3')) //음소거
	{
		Volume = 0.0f;
		Audio_SetVolume(Volume);
	}

	// 이펙트 사운드 처리부
	if (!data->isPlayedEffetSound)
	{
		if (data->EffectTime >= data->EffectSoundDelay)
		{
			Audio_PlaySoundEffect(&data->EffectSound, 0);
			data->isPlayedEffetSound = true;
		}
	}


	// 텍스트 출력 지연 처리
	if (data->elapsedTime >= TextDelay)
	{
		if (data->delayCount <= data->LineCount)
		{
			data->delayCount++;
			data->TextCheck++;
		}
		data->elapsedTime = 0;
	}
	if (Input_GetKey('9'))
	{
		TextDelay += 0.05f;
	}
	if (Input_GetKey('0'))
	{
		TextDelay -= 0.05f;
	}

	// 텍스트 출력 스킵처리과정
	if (Input_GetKeyDown(VK_RETURN))
	{
		if (!data->isSkip)
		{
			data->isSkip = true;
		}
		data->TextCheck = data->LineCount;
	}

	// 스페이스 입력시 다음 화면 인덱스 저장
	if (Input_GetKeyDown(VK_SPACE))
	{
		if (parsing_dt.sceneData[data->nowIndex].CHOOSE_1_NEXT_SCENE == NULL && parsing_dt.sceneData[data->nowIndex].CHOOSE_2_NEXT_SCENE == NULL && parsing_dt.sceneData[data->nowIndex].CHOOSE_3_NEXT_SCENE == NULL)
		{
			data->gotoMain = true;
		}
		else
		{
			if (data->ChooseCount == 1)
			{
				Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_1_NEXT_SCENE) - 1;
			}
			if (data->ChooseCount == 2)
			{
				if (data->isUp)
				{
					Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_1_NEXT_SCENE) - 1;
				}
				if (!data->isUp)
				{
					Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_2_NEXT_SCENE) - 1;
				}
			}
			if (data->ChooseCount == 3)
			{
				if (data->isUp)
				{
					Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_1_NEXT_SCENE) - 1;
				}
				if (!data->isUp && !data->isDown)
				{
					Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_2_NEXT_SCENE) - 1;
				}
				if (data->isDown)
				{
					Index = (parsing_dt.sceneData[data->nowIndex].CHOOSE_3_NEXT_SCENE) - 1;
				}
			}
		}
		if (data->TextCheck >= data->LineCount)
		{
			data->goNextScene = true;
		}
	}
	// PadeOut 처리부
	if (data->goNextScene)
	{
		if (data->gotoMain)
		{
			Scene_SetNextScene(SCENE_TITLESCENE);
		}
		else if (parsing_dt.sceneData[data->nowIndex].IMG_OUTPUT_STYLE == 1)
		{
			if (data->blackoutAlpha < 255 && data->TextAlpha > 0)
			{
				data->blackoutAlpha += 5;
				data->TextAlpha -= 5;
				Text_CreateText(&data->Choose_1, "GmarketSansTTFLight.ttf", 0, L"", wcslen(L""));
				Text_CreateText(&data->Choose_2, "GmarketSansTTFLight.ttf", 0, L"", wcslen(L""));
				Text_CreateText(&data->Choose_3, "GmarketSansTTFLight.ttf", 0, L"", wcslen(L""));
			}
			else
			{
				Scene_SetNextScene(SCENE_EXTRA);
			}
		}
		else
		{
			Scene_SetNextScene(SCENE_EXTRA);
		}
	}

	// 선택지 위아래 키보드 입력 처리부
	if (Input_GetKeyDown(VK_UP))
	{
		if (data->ChooseCount == 2)
		{
			if (!data->isUp)
			{
				data->isUp = !data->isUp;
				data->isChoice_1 = true;
				data->isChoice_2 = false;

			}
		}
		else if (data->ChooseCount == 3)
		{
			if (!data->isUp && !data->isDown)
			{
				data->isUp = !data->isUp;
				data->isChoice_1 = true;
				data->isChoice_2 = false;
			}
			else if (!data->isUp && data->isDown)
			{
				data->isDown = !data->isDown;
				data->isChoice_1 = false;
				data->isChoice_2 = true;
				data->isChoice_3 = false;
			}
		}
	}
	if (Input_GetKeyDown(VK_DOWN))
	{
		if (data->ChooseCount == 2)
		{
			if (data->isUp)
			{
				data->isUp = !data->isUp;
				data->isChoice_1 = false;
				data->isChoice_2 = true;
			}
		}
		else if (data->ChooseCount == 3)
		{
			if (data->isUp && !data->isDown)
			{
				data->isUp = !data->isUp;
				data->isChoice_1 = false;
				data->isChoice_2 = true;
				data->isChoice_3 = false;
			}
			else if (!data->isUp && !data->isDown)
			{
				data->isDown = !data->isDown;
				data->isChoice_2 = false;
				data->isChoice_3 = true;
			}
		}
	}

}

void render_Extra(void)
{
	SceneData* data = (SceneData*)g_Scene.Data;
	SDL_Color Text = { .r = 255, .g = 255, .b = 255, .a = data->TextAlpha };
	SDL_Color main = { .r = 255, .g = 255, .b = 255, .a = 255 };
	SDL_Color notchoice = { .r = 255, .g = 255, .b = 255, .a = 90 };

	//이미지 출력
	Renderer_DrawImage(&data->BackGround, 0, 0);
	Image_SetAlphaValue(&data->BlackOutImage, data->blackoutAlpha);
	Renderer_DrawImage(&data->BlackOutImage, 0, 0);

	
	//텍스트 출력
	if (data->isSkip)
	{
		for (int32 i = 0; i < data->LineCount; i++)
		{
			Renderer_DrawTextBlended(&data->GuideLine[i], 982, 82 + (30 * i), Text);
		}
	}
	else
	{
		for (int32 i = 0; i < data->delayCount; i++)
		{
			Renderer_DrawTextBlended(&data->GuideLine[i], 982, 82 + (30 * i), Text);
		}
	}

	if (data->TextCheck >= data->LineCount)
	{
		//커서 출력
		if (data->ChooseCount == 1)
		{
			Renderer_DrawTextBlended(&data->Coursur, 975, 790, Text);
			Text_SetFontStyle(&data->Choose_1, FS_UNDERLINE);
		}
		if (data->ChooseCount == 2)
		{
			if (data->isUp)
			{
				Renderer_DrawTextBlended(&data->Coursur, 975, 790, Text);
				Text_SetFontStyle(&data->Choose_1, FS_UNDERLINE);
				Text_SetFontStyle(&data->Choose_2, FS_NORMAL);
			}
			else
			{
				Renderer_DrawTextBlended(&data->Coursur, 975, 820, Text);
				Text_SetFontStyle(&data->Choose_1, FS_NORMAL);
				Text_SetFontStyle(&data->Choose_2, FS_UNDERLINE);
			}
		}
		if (data->ChooseCount == 3)
		{
			if (data->isUp)
			{
				Renderer_DrawTextBlended(&data->Coursur, 975, 790, Text);
				Text_SetFontStyle(&data->Choose_1, FS_UNDERLINE);
				Text_SetFontStyle(&data->Choose_2, FS_NORMAL);
				Text_SetFontStyle(&data->Choose_3, FS_NORMAL);
			}
			else if (!data->isUp && !data->isDown)
			{
				Renderer_DrawTextBlended(&data->Coursur, 975, 820, Text);
				Text_SetFontStyle(&data->Choose_1, FS_NORMAL);
				Text_SetFontStyle(&data->Choose_2, FS_UNDERLINE);
				Text_SetFontStyle(&data->Choose_3, FS_NORMAL);
			}
			else
			{
				Renderer_DrawTextBlended(&data->Coursur, 975, 850, Text);
				Text_SetFontStyle(&data->Choose_1, FS_NORMAL);
				Text_SetFontStyle(&data->Choose_2, FS_NORMAL);
				Text_SetFontStyle(&data->Choose_3, FS_UNDERLINE);
			}
		}

		//선택지 출력
		if (data->ChooseCount == 1)
		{
			Renderer_DrawTextBlended(&data->Choose_1, 1000, 790, main);
		}
		else
		{
			if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1) != L"")
			{
				if (data->isChoice_1)
				{
					Renderer_DrawTextBlended(&data->Choose_1, 1000, 790, main);
				}
				else
				{
					Renderer_DrawTextBlended(&data->Choose_1, 1000, 790, notchoice);
				}
			}
			if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2) != L"")
			{
				if (data->isChoice_2)
				{
					Renderer_DrawTextBlended(&data->Choose_2, 1000, 820, main);
				}
				else
				{
					Renderer_DrawTextBlended(&data->Choose_2, 1000, 820, notchoice);
				}
			}
			if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3) != L"")
			{
				if (data->isChoice_3)
				{
					Renderer_DrawTextBlended(&data->Choose_3, 1000, 850, main);
				}
				else
				{
					Renderer_DrawTextBlended(&data->Choose_3, 1000, 850, notchoice);
				}
			}
		}
	}
}

void release_Extra(void)
{
	SceneData* data = (SceneData*)g_Scene.Data;
	Text_FreeText(&data->Choose_1);
	Text_FreeText(&data->Choose_2);
	Text_FreeText(&data->Choose_3);
	Text_FreeText(&data->Coursur);

	for (int32 i = 0; i < 20; i++)
	{
		Text_FreeText(&data->GuideLine[i]);
	}

	if (*parsing_dt.sceneData[Index].SOUND_NAME != NULL)
	{
		Audio_FreeMusic(&data->Main_BGM);
	}
	Audio_FreeSoundEffect(&data->EffectSound);
	Image_FreeImage(&data->BackGround);
	Image_FreeImage(&data->BlackOutImage);

	SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region MainScreen
typedef struct MainScreenData
{
	Text	RoadingText[3];
	Text	Recording;
	Text	Barcode;
	Text	Success;
	Text	Rocation;
	Text	Infomation[3];
	Text	LoadConformMent;
	Text	LoadConform[4];
	Text	BioLink[3];
	Text	BrainLink;
	int32	RoadingCheck;
	int32	ConformCheck;
	int32	BioLinkCheck;
	int32	NextText;
	bool	isNext;
	bool	canShow;
	Image	BackGround;
	Music	BGM;
} MainScreenData;

void init_MainScreen(void)
{
	g_Scene.Data = malloc(sizeof(MainScreenData));
	memset(g_Scene.Data, 0, sizeof(MainScreenData));
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	Text_CreateText(&data->RoadingText[0], "LoadingSceneText.ttf", 24, L"바이오 칩셋 불러오는 중 .", wcslen(L"바이오 칩셋 불러오는 중 ."));
	Text_CreateText(&data->RoadingText[1], "LoadingSceneText.ttf", 24, L"바이오 칩셋 불러오는 중 . .", wcslen(L"바이오 칩셋 불러오는 중 . ."));
	Text_CreateText(&data->RoadingText[2], "LoadingSceneText.ttf", 24, L"바이오 칩셋 불러오는 중 . . .", wcslen(L"바이오 칩셋 불러오는 중 . . ."));
	Text_CreateText(&data->Barcode, "LoadingSceneText.ttf", 24, L"쎁쎚쎂쎃쎁쎚쎂쎃쎁쎚쎚쎁", wcslen(L"쎁쎚쎂쎃쎁쎚쎂쎃쎁쎚쎚쎁"));
	Text_CreateText(&data->Success, "LoadingSceneText.ttf", 24, L"보안 인증 [완료]", wcslen(L"보안 인증 [완료]"));
	Text_CreateText(&data->Recording, "LoadingSceneText.ttf", 24, L"기록자 – 타카네 준", wcslen(L"기록자 – 타카네 준"));
	Text_CreateText(&data->Rocation, "LoadingSceneText.ttf", 24, L"기록 좌표 - 東京 , 日本 ", wcslen(L"기록 좌표 - 東京 , 日本 "));
	Text_CreateText(&data->Infomation[0], "LoadingSceneText.ttf", 24, L"━━━━━━ 썿 ━━━━━━", wcslen(L"━━━━━━ 썿 ━━━━━━"));
	Text_CreateText(&data->Infomation[1], "LoadingSceneText.ttf", 24, L"본 칩셋은 내각관방 직할 내각정보조사실 존치기록물 입니다.", wcslen(L"본 칩셋은 내각관방 직할 내각정보조사실 존치기록물 입니다."));
	Text_CreateText(&data->Infomation[2], "LoadingSceneText.ttf", 24, L"このチップセットは、쎛閣情報調査室存썾記쎜物です。", wcslen(L"このチップセットは、쎛閣情報調査室存썾記쎜物です。"));
	Text_CreateText(&data->LoadConformMent, "LoadingSceneText.ttf", 24, L"로드 컨펌중", wcslen(L"로드 컨펌중"));
	Text_CreateText(&data->LoadConform[0], "LoadingSceneText.ttf", 24, L"Progress [20%] : 쎃쎃", wcslen(L"Progress [20%] : 쎃쎃"));
	Text_CreateText(&data->LoadConform[1], "LoadingSceneText.ttf", 24, L"Progress [40%] : 쎃쎃쎃쎃쎃쎃", wcslen(L"Progress [40%] : 쎃쎃쎃쎃쎃쎃"));
	Text_CreateText(&data->LoadConform[2], "LoadingSceneText.ttf", 24, L"Progress [80%] : 쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃", wcslen(L"Progress [80%] : 쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃"));
	Text_CreateText(&data->LoadConform[3], "LoadingSceneText.ttf", 24, L"Progress [100%] : 쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃", wcslen(L"Progress [100%] : 쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃쎃"));
	Text_CreateText(&data->BioLink[0], "LoadingSceneText.ttf", 24, L"대뇌 감정 링크 확인 .", wcslen(L"대뇌 감정 링크 확인 ."));
	Text_CreateText(&data->BioLink[1], "LoadingSceneText.ttf", 24, L"대뇌 감정 링크 확인 . .", wcslen(L"대뇌 감정 링크 확인 . ."));
	Text_CreateText(&data->BioLink[2], "LoadingSceneText.ttf", 24, L"대뇌 감정 링크 확인 . . .", wcslen(L"대뇌 감정 링크 확인 . . ."));
	Text_CreateText(&data->BrainLink, "LoadingSceneText.ttf", 24, L"바이오 링크 [완료]", wcslen(L"바이오 링크 [완료]"));
	data->BioLinkCheck = 0;
	data->ConformCheck = 1;
	data->RoadingCheck = 1;
	data->NextText = 0;
	data->isNext = false;
	data->canShow = false;

	Image_LoadImage(&data->BackGround,"loading.png");
	Audio_LoadMusic(&data->BGM, "prologue.mp3");
	Audio_GetVolume();
	Audio_SetVolume(Volume);
	Audio_Play(&data->BGM, INFINITY_LOOP);
}

void update_MainScreen(void)
{
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	static float elapsedTime;
	static float gotoNextScene;
	static float infomationTextCheckTime;
	elapsedTime += Timer_GetDeltaTime();
	infomationTextCheckTime += Timer_GetDeltaTime();

	// 볼륨 처리부
	if (Input_GetKey('1'))
	{
		Volume -= 0.01f;
		Audio_SetVolume(Volume);
	}

	if (Input_GetKey('2'))
	{
		Volume += 0.01f;
		Audio_SetVolume(Volume);
	}

	if (infomationTextCheckTime >= 1.1f)
	{
		data->canShow = true;
	}

	if (elapsedTime >= 0.7f)
	{
		if (data->RoadingCheck < 3)
		{
			data->RoadingCheck++;
		}
		else if (data->NextText == 6 && data->ConformCheck < 4)
		{
			data->ConformCheck++;
		}
		else if (data->NextText == 7 && data->BioLinkCheck < 3)
		{
			data->BioLinkCheck++;
		}
		if (data->isNext)
		{
			data->NextText++;
			data->isNext = false;
		}
		elapsedTime = 0;
	}
	if (data->NextText == 8)
	{
		gotoNextScene += Timer_GetDeltaTime();
		if (gotoNextScene >= 3.5f)
		{
			Scene_SetNextScene(SCENE_EXTRA);
		}
	}
}

void render_MainScreen(void)
{
	SDL_Color text = { .r = 255, .g = 255, .b = 255, .a = 255 };
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	Renderer_DrawImage(&data->BackGround, 0, 0);

	if (data->canShow)
	{
		Renderer_DrawTextBlended(&data->Infomation[0], 640, 710, text);
		Renderer_DrawTextBlended(&data->Infomation[1], 520, 740, text);
		Renderer_DrawTextBlended(&data->Infomation[2], 530, 770, text);
	}

	for (int32 i = 0; i < data->RoadingCheck; i++)
	{
		Renderer_DrawTextBlended(&data->RoadingText[i], 150, 150, text);
		if (i > 0)
		{
			Text_FreeText(&data->RoadingText[i - 1]);
		}
	}
	if (data->NextText == 0 && data->RoadingCheck == 3)
	{
		data->isNext = true;
	}
	if (data->NextText > 0)
	{
		Renderer_DrawTextBlended(&data->Barcode, 160, 180, text);
		if (data->NextText == 1)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 1)
	{
		Renderer_DrawTextBlended(&data->Success, 160, 210, text);
		if (data->NextText == 2)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 2)
	{
		Renderer_DrawTextBlended(&data->Recording, 1200, 120, text);
		if (data->NextText == 3)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 3)
	{
		Renderer_DrawTextBlended(&data->Rocation, 1200, 150, text);
		if (data->NextText == 4)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 4)
	{
		Renderer_DrawTextBlended(&data->LoadConformMent, 30, 480, text);
		if (data->NextText == 5)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 5)
	{
		for (int32 i = 0; i < data->ConformCheck; i++)
		{
			Renderer_DrawTextBlended(&data->LoadConform[i], 30, 510, text);
			if (i > 0)
			{
				Text_FreeText(&data->LoadConform[i - 1]);
			}
		}
		if (data->NextText == 6 && data->ConformCheck == 4)
		{
			data->isNext = true;
		}
		
	}
	if (data->NextText > 6)
	{
		for (int32 i = 0; i < data->BioLinkCheck; i++)
		{
			Renderer_DrawTextBlended(&data->BioLink[i], 1100, 480, text);
			if (i > 0)
			{
				Text_FreeText(&data->BioLink[i - 1]);
			}
		}
		if (data->NextText == 7 && data->BioLinkCheck == 3)
		{
			data->isNext = true;
		}
		
	}
	if (data->NextText > 7)
	{
		Renderer_DrawTextBlended(&data->BrainLink, 1350, 480, text);
	}
}

void release_MainScreen(void)
{
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	Image_FreeImage(&data->BackGround);

	for (int32 i = 0; i < 3; i++)
	{
		Text_FreeText(&data->RoadingText[i]);
		Text_FreeText(&data->BioLink[i]);
		Text_FreeText(&data->Infomation[i]);
		Text_FreeText(&data->LoadConform[i]);
	}
	Text_FreeText(&data->LoadConform[3]);
	Text_FreeText(&data->LoadConformMent);
	Text_FreeText(&data->Barcode);
	Text_FreeText(&data->Success);
	Text_FreeText(&data->Recording);
	Text_FreeText(&data->Rocation);
	Text_FreeText(&data->BrainLink);

	Audio_FreeMusic(&data->BGM);

	SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region TitleScene
typedef struct TitleSceneData
{
	Image			BackGround;
	Text			Menu[4];
	Music			BGM;
	bool			isChoice_1;
	bool			isChoice_2;
	bool			isChoice_3;
	bool			isChoice_4;
} TitleSceneData;

void init_TitleScene(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	Image_LoadImage(&data->BackGround, "main_title.png");

	Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 36, L"게임시작", wcslen(L"게임시작"));
	Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 32, L"제작진", wcslen(L"제작진"));
	Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 32, L"조작법", wcslen(L"조작법"));
	Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));

	Audio_LoadMusic(&data->BGM, "main.mp3");
	Audio_GetVolume();
	Audio_SetVolume(Volume);
	Audio_Play(&data->BGM, INFINITY_LOOP);
	
	data->isChoice_1 = true;
	data->isChoice_2 = false;
	data->isChoice_3 = false;
	data->isChoice_4 = false;
}

void update_TitleScene(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	if (Input_GetKeyDown(VK_SPACE))
	{
		if (data->isChoice_1)
		{
			Index = 0;
			Scene_SetNextScene(SCENE_MAINSCREEN);
		}
		if (data->isChoice_2)
		{
			Scene_SetNextScene(SCENE_CEADITSSCENE);
		}
		if (data->isChoice_3)
		{
			Scene_SetNextScene(SCENE_CONTROLSCENE);
		}
		if (data->isChoice_4)
		{
			exit(0);
		}
	}
	// 볼륨 처리부
	if (Input_GetKey('1'))
	{
		Volume -= 0.01f;
		Audio_SetVolume(Volume);
	}

	if (Input_GetKey('2'))
	{
		Volume += 0.01f;
		Audio_SetVolume(Volume);
	}

	if (Input_GetKeyDown(VK_UP))
	{
			if (!data->isChoice_1 && data->isChoice_2 && !data->isChoice_3 && !data->isChoice_4)
			{
				data->isChoice_1 = true;
				data->isChoice_2 = false;
				data->isChoice_3 = false;
				data->isChoice_4 = false;

				Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 36, L"게임시작", wcslen(L"게임시작"));
				Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 32, L"제작진", wcslen(L"제작진"));
				Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 32, L"조작법", wcslen(L"조작법"));
				Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));
			}
			else if (!data->isChoice_1 && !data->isChoice_2 && data->isChoice_3 && !data->isChoice_4)
			{
				data->isChoice_1 = false;
				data->isChoice_2 = true;
				data->isChoice_3 = false;
				data->isChoice_4 = false;

				Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 32, L"게임시작", wcslen(L"게임시작"));
				Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 36, L"제작진", wcslen(L"제작진"));
				Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 32, L"조작법", wcslen(L"조작법"));
				Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));
			}
			else if (!data->isChoice_1 && !data->isChoice_2 && !data->isChoice_3 && data->isChoice_4)
			{
				data->isChoice_1 = false;
				data->isChoice_2 = false;
				data->isChoice_3 = true;
				data->isChoice_4 = false;

				Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 32, L"게임시작", wcslen(L"게임시작"));
				Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 32, L"제작진", wcslen(L"제작진"));
				Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 36, L"조작법", wcslen(L"조작법"));
				Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));
			}
	}
	if (Input_GetKeyDown(VK_DOWN))
	{
		if (data->isChoice_1 && !data->isChoice_2 && !data->isChoice_3 && !data->isChoice_4)
		{
			data->isChoice_1 = false;
			data->isChoice_2 = true;
			data->isChoice_3 = false;
			data->isChoice_4 = false;

			Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 32, L"게임시작", wcslen(L"게임시작"));
			Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 36, L"제작진", wcslen(L"제작진"));
			Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 32, L"조작법", wcslen(L"조작법"));
			Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));
		}
		else if (!data->isChoice_1 && data->isChoice_2 && !data->isChoice_3 && !data->isChoice_4)
		{
			data->isChoice_1 = false;
			data->isChoice_2 = false;
			data->isChoice_3 = true;
			data->isChoice_4 = false;

			Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 32, L"게임시작", wcslen(L"게임시작"));
			Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 32, L"제작진", wcslen(L"제작진"));
			Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 36, L"조작법", wcslen(L"조작법"));
			Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 32, L"게임종료", wcslen(L"게임종료"));
		}
		else if (!data->isChoice_1 && !data->isChoice_2 && data->isChoice_3 && !data->isChoice_4)
		{
			data->isChoice_1 = false;
			data->isChoice_2 = false;
			data->isChoice_3 = false;
			data->isChoice_4 = true;

			Text_CreateText(&data->Menu[0], "BareunBatangL.ttf", 32, L"게임시작", wcslen(L"게임시작"));
			Text_CreateText(&data->Menu[1], "BareunBatangL.ttf", 32, L"제작진", wcslen(L"제작진"));
			Text_CreateText(&data->Menu[2], "BareunBatangL.ttf", 32, L"조작법", wcslen(L"조작법"));
			Text_CreateText(&data->Menu[3], "BareunBatangL.ttf", 36, L"게임종료", wcslen(L"게임종료"));
		}
			
	}
}

void render_TitleScene(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	SDL_Color main = { .r = 255, .g = 255, .b = 255, .a = 255 };
	SDL_Color notchoice = { .r = 255, .g = 255, .b = 255, .a = 90 };

	Renderer_DrawImage(&data->BackGround, 0, 0);
	
	if (data->isChoice_1)
	{
		Renderer_DrawTextBlended(&data->Menu[0], 730, 600, main);
	}
	else
	{
		Renderer_DrawTextBlended(&data->Menu[0], 735, 600, notchoice);
	}
	if (data->isChoice_2)
	{
		Renderer_DrawTextBlended(&data->Menu[1], 743, 650, main);
	}
	else
	{
		Renderer_DrawTextBlended(&data->Menu[1], 748, 650, notchoice);
	}
	if (data->isChoice_3)
	{
		Renderer_DrawTextBlended(&data->Menu[2], 743, 700, main);
	}
	else
	{
		Renderer_DrawTextBlended(&data->Menu[2], 748, 700, notchoice);
	}

	if (data->isChoice_4)
	{
		Renderer_DrawTextBlended(&data->Menu[3], 730, 750, main);
	}
	else
	{
		Renderer_DrawTextBlended(&data->Menu[3], 735, 750, notchoice);
	}
}

void release_TitleScene(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	Image_FreeImage(&data->BackGround);
	Audio_FreeMusic(&data->BGM);

	for (int32 i = 0; i < 3; i++)
	{
		Text_FreeText(&data->Menu[i]);
	}

	SafeFree(g_Scene.Data);
}
#pragma endregion

#pragma region CreditScene
typedef struct CreditsSceneData
{
	Text		Staff[16];
	Text		guide[2];
	int32		rocation;
} CreditsSceneData;
void init_CreditsScene(void)
{
	g_Scene.Data = malloc(sizeof(CreditsSceneData));
	memset(g_Scene.Data, 0, sizeof(CreditsSceneData));
	CreditsSceneData* data = (CreditsSceneData*)g_Scene.Data;

	Text_CreateText(&data->Staff[0], "12LotteMartHappyLight.ttf", 22, L"<<기획반 - 3김>>", wcslen(L"<<기획반 - 3김>>"));
	Text_CreateText(&data->Staff[1], "12LotteMartHappyLight.ttf", 22, L"팀원 : 김대진(아트)", wcslen(L"팀원 : 김대진(아트)"));
	Text_CreateText(&data->Staff[2], "12LotteMartHappyLight.ttf", 22, L"소감 - 사람은 생각보다 쉽게 죽지 않는다.", wcslen(L"소감 - 사람은 생각보다 쉽게 죽지 않는다."));
	Text_CreateText(&data->Staff[3], "12LotteMartHappyLight.ttf", 22, L"팀원 : 김효진(작가)", wcslen(L"팀원 : 김효진(작가)"));
	Text_CreateText(&data->Staff[4], "12LotteMartHappyLight.ttf", 22, L"소감 - 글이든, 장르든 전부 처음 도전하는 분야라 실력이 매우 부족했지만 기획PD님에게 지도와 영감을 많이 받았고, 덕분에 마무리를 지을 수 있었습니다.", wcslen(L"소감 - 글이든, 장르든 전부 처음 도전하는 분야라 실력이 매우 부족했지만 기획PD님에게 지도와 영감을 많이 받았고, 덕분에 마무리를 지을 수 있었습니다."));
	Text_CreateText(&data->Staff[5], "12LotteMartHappyLight.ttf", 22, L"제 스토리를 더 재밌고 풍성하게 연출해준 기획 3김 팀, 그리고 프로그래밍 소년탐정단 팀에게 감사합니다.", wcslen(L"제 스토리를 더 재밌고 풍성하게 연출해준 기획 3김 팀, 그리고 프로그래밍 소년탐정단 팀에게 감사합니다."));
	Text_CreateText(&data->Staff[6], "12LotteMartHappyLight.ttf", 22, L"팀장 : 김동열 (잡업무)", wcslen(L"팀장 : 김동열 (잡업무)"));
	Text_CreateText(&data->Staff[7], "12LotteMartHappyLight.ttf", 22, L"소감 - 감사하다. 나는 아름답고 쿨한 팀원에 . 하지만 몇일 샙니다 밤을 . 하지만 재밌었다면 OK 입니다.", wcslen(L"소감 - 감사하다. 나는 아름답고 쿨한 팀원에 . 하지만 몇일 샙니다 밤을 . 하지만 재밌었다면 OK 입니다."));
	Text_CreateText(&data->Staff[8], "12LotteMartHappyLight.ttf", 22, L" ", wcslen(L" "));
	Text_CreateText(&data->Staff[9], "12LotteMartHappyLight.ttf", 22, L"<<플밍반 - 아름이없는 소년탐정단>>", wcslen(L"<<플밍반 - 아름이없는 소년탐정단>>"));
	Text_CreateText(&data->Staff[10], "12LotteMartHappyLight.ttf", 22, L"팀원 : 김영훈(CSV파서 담당)", wcslen(L"팀원 : 김영훈(CSV파서 담당)"));
	Text_CreateText(&data->Staff[11], "12LotteMartHappyLight.ttf", 22, L"소감 - 스토리도 충실하고, 개발자와 기획자간의 갈등하나없이 진행할수 있어서 좋았습니다. 앞으로 더 좋은 게임으로 찾아뵙겠습니다. 감사합니다.", wcslen(L"소감 - 스토리도 충실하고, 개발자와 기획자간의 갈등하나없이 진행할수 있어서 좋았습니다. 앞으로 더 좋은 게임으로 찾아뵙겠습니다. 감사합니다."));
	Text_CreateText(&data->Staff[12], "12LotteMartHappyLight.ttf", 22, L"팀원 : 이승일(사운드 및 연출 담당)", wcslen(L"팀원 : 이승일(사운드 및 연출 담당)"));
	Text_CreateText(&data->Staff[13], "12LotteMartHappyLight.ttf", 22, L"소감 - 이번 프로젝트처럼 실력 있고 아이디어 좋은 기획자를 만났을 때 구현 능력이 부족하지 않도록 앞으로 더 열심히 공부해야겠다고 생각했습니다.", wcslen(L"소감 - 이번 프로젝트처럼 실력 있고 아이디어 좋은 기획자를 만났을 때 구현 능력이 부족하지 않도록 앞으로 더 열심히 공부해야겠다고 생각했습니다."));
	Text_CreateText(&data->Staff[14], "12LotteMartHappyLight.ttf", 22, L"팀장 : 박도일(잡업무 및 메인코딩)", wcslen(L"팀장 : 박도일(잡업무 및 메인코딩)"));
	Text_CreateText(&data->Staff[15], "12LotteMartHappyLight.ttf", 22, L"소감 - 힘들었지만 재미있는 경험이었고, 밤새 작업해서 주신 기획팀원분들 모두, 그리고 조금 모자란 팀장 믿고 따라와준 팀원들 전부 고맙습니다.", wcslen(L"소감 - 힘들었지만 재미있는 경험이었고, 밤새 작업해서 주신 기획팀원분들 모두, 그리고 조금 모자란 팀장 믿고 따라와준 팀원들 전부 고맙습니다."));

	Text_CreateText(&data->guide[0], "BareunBatangL.ttf", 18, L"메인화면으로 돌아가기 : SPACE", wcslen(L"메인화면으로 돌아가기 : SPACE"));
	Text_CreateText(&data->guide[1], "BareunBatangL.ttf", 18, L"스크롤 빨리 감기 : ENTER", wcslen(L"스크롤 빨리 감기 : ENTER"));

	data->rocation = 900;
} 
void update_CreditsScene(void)
{
	CreditsSceneData* data = (CreditsSceneData*)g_Scene.Data;

	if (Input_GetKey(VK_RETURN))
	{
		data->rocation -= 10;
	}
	else
	{
		data->rocation -= 2;
	}

	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_TITLESCENE);
	}
}
void render_CreditsScene(void)
{
	CreditsSceneData* data = (CreditsSceneData*)g_Scene.Data;

	SDL_Color fgw = { .r = 255, .g = 255, .b = 255, .a = 255 };
	SDL_Color fgy = { .r = 255, .g = 255, .b = 0, .a = 255 };
	SDL_Color fgr = { .r = 255, .g = 0, .b = 0, .a = 255 };
	SDL_Color bg = { .r = 0,.g = 0,.b = 0,.a = 255 };

	Renderer_DrawTextShaded(&data->guide[0], 1340, 5, fgw, bg);
	Renderer_DrawTextShaded(&data->guide[1], 1380, 25, fgw, bg);

	for (int32 i = 0; i < 16; i++)
	{
		if (i == 1 || i == 3 || i == 6 || i == 10 || i == 12 || i == 14)
		{
			Renderer_DrawTextShaded(&data->Staff[i], 15, data->rocation + (i * 50), fgy, bg);
		}
		else if (i == 0 || i == 9)
		{
			Renderer_DrawTextShaded(&data->Staff[i], 15, data->rocation + (i * 50), fgr, bg);
		}
		else
		{
			Renderer_DrawTextShaded(&data->Staff[i], 15, data->rocation + (i * 50), fgw, bg);
		}
	}
}
void release_CreditsScene(void)
{
	CreditsSceneData* data = (CreditsSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 16; i++)
	{
		Text_FreeText(&data->Staff[i]);
	}
	Text_FreeText(&data->guide);
	SafeFree(g_Scene.Data);

}
#pragma endregion

#pragma region controlScene
typedef struct ControlSceneData
{
	Text	Tutorial[6];
	Text	guide;
} ControlSceneData;
void init_ControlScene(void)
{
	g_Scene.Data = malloc(sizeof(ControlSceneData));
	memset(g_Scene.Data, 0, sizeof(ControlSceneData));
	ControlSceneData* data = (ControlSceneData*)g_Scene.Data;

	Text_CreateText(&data->Tutorial[0], "BareunBatangL.ttf", 24, L"1 : 볼륨 감소", wcslen(L"1 : 볼륨 감소"));
	Text_CreateText(&data->Tutorial[1], "BareunBatangL.ttf", 24, L"2 : 볼륨 증가", wcslen(L"2 : 볼륨 증가"));
	Text_CreateText(&data->Tutorial[2], "BareunBatangL.ttf", 24, L"3 : 음소거", wcslen(L"3 : 음소거"));
	Text_CreateText(&data->Tutorial[3], "BareunBatangL.ttf", 24, L"ENTER : 텍스트 출력 스킵", wcslen(L"ENTER : 텍스트 출력 스킵"));
	Text_CreateText(&data->Tutorial[4], "BareunBatangL.ttf", 24, L"SPACE : 선택지 선택", wcslen(L"SPACE : 선택지 선택"));
	Text_CreateText(&data->Tutorial[5], "BareunBatangL.ttf", 24, L"위,아래 방향키 : 선택지 고르기", wcslen(L"위,아래 방향키 : 선택지 고르기"));

	Text_CreateText(&data->guide, "BareunBatangL.ttf", 18, L"메인화면으로 돌아가기 : SPACE", wcslen(L"메인화면으로 돌아가기 : SPACE"));
}
void update_ControlScene(void)
{
	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_TITLESCENE);
	}
}
void render_ControlScene(void)
{
	ControlSceneData* data = (ControlSceneData*)g_Scene.Data;
	SDL_Color main = { .r = 255, .g = 255, .b = 255, .a = 255 };

	for (int32 i = 0; i < 6; i++)
	{
		if (i < 2)
		{
			Renderer_DrawTextBlended(&data->Tutorial[i], 720, 270 + (30 * i), main);
		}
		else if (i == 2)
		{
			Renderer_DrawTextBlended(&data->Tutorial[i], 730, 270 + (30 * i), main);
		}
		else if (i == 3)
		{
			Renderer_DrawTextBlended(&data->Tutorial[i], 650, 270 + (30 * i), main);
		}
		else if (i == 4)
		{
			Renderer_DrawTextBlended(&data->Tutorial[i], 670, 270 + (30 * i), main);
		}
		else
		{
			Renderer_DrawTextBlended(&data->Tutorial[i], 625, 270 + (30 * i), main);
		}
	}

	Renderer_DrawTextBlended(&data->guide, 670 , 50, main);
}
void release_ControlScene(void)
{
	ControlSceneData* data = (ControlSceneData*)g_Scene.Data;

	for (int32 i = 0; i < 6; i++)
	{
		Text_FreeText(&data->Tutorial[i]);
	}
	Text_FreeText(&data->guide);
}
#pragma endregion

bool Scene_IsSetNextScene(void)
{
	if (SCENE_NULL == s_nextScene)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Scene_SetNextScene(ESceneType scene)
{
	assert(s_nextScene == SCENE_NULL);
	assert(SCENE_NULL < scene&& scene < SCENE_MAX);

	s_nextScene = scene;
}

void Scene_Change(void)
{
	assert(s_nextScene != SCENE_NULL);

	if (g_Scene.Release)
	{
		g_Scene.Release();
	}

	switch (s_nextScene)
	{
	case SCENE_EXTRA:
		g_Scene.Init = init_Extra;
		g_Scene.Update = update_Extra;
		g_Scene.Render = render_Extra;
		g_Scene.Release = release_Extra;
		break;
	case SCENE_MAINSCREEN:
		g_Scene.Init = init_MainScreen;
		g_Scene.Update = update_MainScreen;
		g_Scene.Render = render_MainScreen;
		g_Scene.Release = release_MainScreen;
		break;
	case SCENE_TITLESCENE:
		g_Scene.Init = init_TitleScene;
		g_Scene.Update = update_TitleScene;
		g_Scene.Render = render_TitleScene;
		g_Scene.Release = release_TitleScene;
		break;
	case SCENE_CEADITSSCENE:
		g_Scene.Init = init_CreditsScene;
		g_Scene.Update = update_CreditsScene;
		g_Scene.Render = render_CreditsScene;
		g_Scene.Release = release_CreditsScene;
		break;
	case SCENE_CONTROLSCENE:
		g_Scene.Init = init_ControlScene;
		g_Scene.Update = update_ControlScene;
		g_Scene.Render = render_ControlScene;
		g_Scene.Release = release_ControlScene;
		break;
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}