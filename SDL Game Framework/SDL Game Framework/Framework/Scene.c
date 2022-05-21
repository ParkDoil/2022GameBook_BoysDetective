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
static float Volume = 1.0f;
static float TextDelay = 1.5f;

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
	float			EffectSoundDelay;
	float			elapsedTime;
	float			EffectTime;
	SoundEffect		EffectSound;
	Image			BackGround;
	Image			BlackOutImage;
	Music			Main_BGM;
	Text			GuideLine[20];
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
		Text_CreateText(&data->Choose_1, "GmarketSansTTFLight.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1));
		data->ChooseCount++;
	}
	if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2) != L'\0')
	{
		Text_CreateText(&data->Choose_2, "GmarketSansTTFLight.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2));
		data->ChooseCount++;
	}
	if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3) != L'\0')
	{
		Text_CreateText(&data->Choose_3, "GmarketSansTTFLight.ttf", data->FontSize, parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3, wcslen(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3));
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
	}

	// 스페이스 입력시 다음 화면 인덱스 저장
	if (Input_GetKeyDown(VK_SPACE))
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
		data->goNextScene = true;
	}
	// PadeOut 처리부
	if (data->goNextScene)
	{
		if (parsing_dt.sceneData[data->nowIndex].IMG_OUTPUT_STYLE == 1)
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
			Renderer_DrawTextBlended(&data->GuideLine[i], 982, 82 + (35 * i), Text);
		}
	}
	else
	{
		for (int32 i = 0; i < data->delayCount; i++)
		{
			Renderer_DrawTextBlended(&data->GuideLine[i], 982, 82 + (35 * i), Text);
		}
	}
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
		Renderer_DrawTextBlended(&data->Choose_1, 1010, 790, main);
	}
	else
	{
		if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_1) != L"")
		{
			if (data->isChoice_1)
			{
				Renderer_DrawTextBlended(&data->Choose_1, 1010, 790, main);
			}
			else
			{
				Renderer_DrawTextBlended(&data->Choose_1, 1010, 790, notchoice);
			}
		}
		if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_2) != L"")
		{
			if (data->isChoice_2)
			{
				Renderer_DrawTextBlended(&data->Choose_2, 1010, 820, main);
			}
			else
			{
				Renderer_DrawTextBlended(&data->Choose_2, 1010, 820, notchoice);
			}
		}
		if (*(parsing_dt.sceneData[data->nowIndex].CHOOSE_TEXT_3) != L"")
		{
			if (data->isChoice_3)
			{
				Renderer_DrawTextBlended(&data->Choose_3, 1010, 850, main);
			}
			else
			{
				Renderer_DrawTextBlended(&data->Choose_3, 1010, 850, notchoice);
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
	Text	Date;
	Text	Rocation;
	Text	Hanja;
	Text	Infomation;
	Text	RoadConform[4];
	Text	BioLink[3];
	Text	BrainLink;
	int32	RoadingCheck;
	int32	ConformCheck;
	int32	BioLinkCheck;
	int32	NextText;
	bool	isNext;
	Music	BGM;
} MainScreenData;

void init_MainScreen(void)
{
	g_Scene.Data = malloc(sizeof(MainScreenData));
	memset(g_Scene.Data, 0, sizeof(MainScreenData));
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	Text_CreateText(&data->RoadingText[0], "Disital.ttf", 24, L"바이오 칩셋 로딩중 .", wcslen(L"바이오 칩셋 로딩중 ."));
	Text_CreateText(&data->RoadingText[1], "Disital.ttf", 24, L"바이오 칩셋 로딩중 . .", wcslen(L"바이오 칩셋 로딩중 . ."));
	Text_CreateText(&data->RoadingText[2], "Disital.ttf", 24, L"바이오 칩셋 로딩중 . . .", wcslen(L"바이오 칩셋 로딩중 . . ."));
	Text_CreateText(&data->Recording, "Disital.ttf", 24, L"기록자 - [타카네 준]", wcslen(L"기록자 - [타카네 준]"));
	Text_CreateText(&data->Date, "Disital.ttf", 24, L"2067년 8 월 12 일", wcslen(L"2067년 8 월 12 일"));
	Text_CreateText(&data->Rocation, "Disital.ttf", 24, L"위치 좌표 - ", wcslen(L"위치 좌표 - "));
	Text_CreateText(&data->Hanja, "12LotteMartHappyLight.ttf", 22, L"[東京 , 日本]", wcslen(L"[東京 , 日本]"));
	Text_CreateText(&data->Infomation, "Disital.ttf", 24, L"본 칩셋은 내각관방 직할 내각정보조사실 존치기록물 입니다-", wcslen(L"본 칩셋은 내각관방 직할 내각정보조사실 존치기록물 입니다-"));
	Text_CreateText(&data->RoadConform[0], "Disital.ttf", 24, L"로드 컨펌중 -", wcslen(L"로드 컨펌중 -"));
	Text_CreateText(&data->RoadConform[1], "Disital.ttf", 24, L"로드 컨펌중 - -", wcslen(L"로드 컨펌중 - -"));
	Text_CreateText(&data->RoadConform[2], "Disital.ttf", 24, L"로드 컨펌중 - - -", wcslen(L"로드 컨펌중 - - -"));
	Text_CreateText(&data->RoadConform[3], "Disital.ttf", 24, L"로드 컨펌중 - - - -", wcslen(L"로드 컨펌중 - - - -"));
	Text_CreateText(&data->BioLink[0], "Disital.ttf", 24, L"바이오 링크 완료 .", wcslen(L"바이오 링크 완료 ."));
	Text_CreateText(&data->BioLink[1], "Disital.ttf", 24, L"바이오 링크 완료 . .", wcslen(L"바이오 링크 완료 . ."));
	Text_CreateText(&data->BioLink[2], "Disital.ttf", 24, L"바이오 링크 완료 . . .", wcslen(L"바이오 링크 완료 . . ."));
	Text_CreateText(&data->BrainLink, "Disital.ttf", 24, L"대뇌 감정 링크 확인", wcslen(L"대뇌 감정 링크 확인"));
	data->BioLinkCheck = 0;
	data->ConformCheck = 1;
	data->RoadingCheck = 1;
	data->NextText = 0;
	data->isNext = false;

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
	elapsedTime += Timer_GetDeltaTime();

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

	if (elapsedTime >= 0.7f)
	{
		if (data->RoadingCheck < 3)
		{
			data->RoadingCheck++;
		}
		else if (data->NextText == 5 && data->ConformCheck < 4)
		{
			data->ConformCheck++;
		}
		else if (data->NextText == 6 && data->BioLinkCheck < 3)
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
	SDL_Color bg = { .r = 0, .g = 0, .b = 0 };
	SDL_Color fg = { .r = 255, .g = 255, .b = 255 };
	MainScreenData* data = (MainScreenData*)g_Scene.Data;
	for (int32 i = 0; i < data->RoadingCheck; i++)
	{
		Renderer_DrawTextShaded(&data->RoadingText[i], 30, 30 , fg, bg);
	}
	if (data->NextText == 0 && data->RoadingCheck == 3)
	{
		data->isNext = true;
	}
	if (data->NextText > 0)
	{
		Renderer_DrawTextShaded(&data->Recording, 30, 70, fg, bg);
		if (data->NextText == 1)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 1)
	{
		Renderer_DrawTextShaded(&data->Date, 770, 430, fg, bg);
		if (data->NextText == 2)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 2)
	{
		Renderer_DrawTextShaded(&data->Rocation, 770, 470, fg, bg);
		Renderer_DrawTextShaded(&data->Hanja, 880, 470, fg, bg);
		if (data->NextText == 3)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 3)
	{
		Renderer_DrawTextShaded(&data->Infomation, 50, 600, fg, bg);
		if (data->NextText == 4)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 4)
	{
		for (int32 i = 0; i < data->ConformCheck; i++)
		{
			Renderer_DrawTextShaded(&data->RoadConform[i], 60, 650, fg, bg);
			
		}
		if (data->NextText == 5 && data->ConformCheck == 4)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 5)
	{
		for (int32 i = 0; i < data->BioLinkCheck; i++)
		{
			Renderer_DrawTextShaded(&data->BioLink[i], 940, 750, fg, bg);
		}
		if (data->NextText == 6 && data->BioLinkCheck == 3)
		{
			data->isNext = true;
		}
	}
	if (data->NextText > 6)
	{
		Renderer_DrawTextShaded(&data->BrainLink, 1150, 750, fg, bg);
		if (data->NextText == 7)
		{
			data->isNext = true;
		}
	}
}

void release_MainScreen(void)
{
	MainScreenData* data = (MainScreenData*)g_Scene.Data;

	for (int32 i = 0; i < 3; i++)
	{
		Text_FreeText(&data->RoadingText[i]);
		Text_FreeText(&data->BioLink[i]);
		Text_FreeText(&data->RoadConform[i]);
	}
	Text_FreeText(&data->RoadConform[3]);
	Text_FreeText(&data->Recording);
	Text_FreeText(&data->Date);
	Text_FreeText(&data->Rocation);
	Text_FreeText(&data->Hanja);
	Text_FreeText(&data->Infomation);
	Text_FreeText(&data->BrainLink);
	Audio_FreeMusic(&data->BGM);

	SafeFree(g_Scene.Data);
}

#pragma endregion

#pragma region TitleScene
typedef struct TitleSceneData
{
	Image	BackGround;
	Music	BGM;
} TitleSceneData;

void init_TitleScene(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	Image_LoadImage(&data->BackGround, "main_title.png");

	Audio_LoadMusic(&data->BGM, "main.mp3");
	Audio_GetVolume();
	float sound = 1.0f;
	Audio_SetVolume(sound);
	Audio_Play(&data->BGM, INFINITY_LOOP);
}

void update_TitleScene(void)
{
	if (Input_GetKeyDown(VK_SPACE))
	{
		Scene_SetNextScene(SCENE_MAINSCREEN);
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
}

void render_TitleScene(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;

	Renderer_DrawImage(&data->BackGround, 0, 0);
}

void release_TitleScene(void)
{
	TitleSceneData* data = (TitleSceneData*)g_Scene.Data;
	Image_FreeImage(&data->BackGround);
	Audio_FreeMusic(&data->BGM);
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
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}