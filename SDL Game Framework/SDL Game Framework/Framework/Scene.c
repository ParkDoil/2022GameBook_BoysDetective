#include "stdafx.h"
#include "Scene.h"
#include "Framework.h"
#include "Csv.h"
#include "Csvparser.h"

Scene g_Scene;
static ESceneType s_nextScene = SCENE_NULL;

parsing parsing_dt;

const wchar_t* Sentence[] =
{
	L"TitleScene입니다.",
	L"MainScene입니다.",
	L"잘있어요."
};

typedef struct TitleSceneData
{
	Text TitleText;
	Music TitleMusic;
	SoundEffect TitleSoundEffect;
} TitleSceneData;

#pragma region TitleScene
void init_title(void)
{
	g_Scene.Data = malloc(sizeof(TitleSceneData));
	memset(g_Scene.Data, 0, sizeof(TitleSceneData));

	TitleSceneData* Data = (TitleSceneData*)g_Scene.Data;

	Text_CreateText(&Data->TitleText, "MaruBuri-Light.ttf", 21, parsing_dt.sceneData[1].TEXT, wcslen(parsing_dt.sceneData[0].TEXT));
	Audio_LoadMusic(&Data->TitleMusic, "powerful.mp3");
	Audio_Play(&Data->TitleMusic, INFINITY_LOOP);
}

void update_title(void)
{
	if (Input_GetKeyDown(VK_RETURN))
	{
		Scene_SetNextScene(SCENE_MAIN);
	}
}

void render_title(void)
{
	TitleSceneData* Data = (TitleSceneData*)g_Scene.Data;

	SDL_Color Color = { .a = 255 };
	Renderer_DrawTextSolid(&Data->TitleText, 50, 100, Color);
}

void release_title(void)
{
	TitleSceneData* Data = (TitleSceneData*)g_Scene.Data;

	Text_FreeText(&Data->TitleText);
}
#pragma endregion

typedef struct MainSceneData
{
	Text MainText;
	Music MainMusic;
	SoundEffect MainSoundEffect;
} MainSceneData;

#pragma region MainScene
void init_main(void)
{
	g_Scene.Data = malloc(sizeof(MainSceneData));
	memset(g_Scene.Data, 0, sizeof(MainSceneData));

	MainSceneData* Data = (MainSceneData*)g_Scene.Data;

	Text_CreateText(&Data->MainText, "MaruBuri-Light.ttf", 21, Sentence[1], wcslen(Sentence[1]));
	/*Audio_LoadMusic(&Data->MainMusic, "powerful.mp3");
	Audio_Play(&Data->MainMusic, INFINITY_LOOP);*/
}

void update_main(void)
{
	if (Input_GetKeyDown(VK_RETURN))
	{
		Scene_SetNextScene(SCENE_TITLE);
	}
}

void render_main(void)
{
	MainSceneData* Data = (MainSceneData*)g_Scene.Data;

	SDL_Color Color = { .a = 255 };
	Renderer_DrawTextSolid(&Data->MainText, 50, 100, Color);
}

void release_main(void)
{
	MainSceneData* Data = (MainSceneData*)g_Scene.Data;

	Text_FreeText(&Data->MainText);
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
	case SCENE_TITLE:
		g_Scene.Init = init_title;
		g_Scene.Update = update_title;
		g_Scene.Render = render_title;
		g_Scene.Release = release_title;
		break;
	case SCENE_MAIN:
		g_Scene.Init = init_main;
		g_Scene.Update = update_main;
		g_Scene.Render = render_main;
		g_Scene.Release = release_main;
		break;
	}

	g_Scene.Init();

	s_nextScene = SCENE_NULL;
}