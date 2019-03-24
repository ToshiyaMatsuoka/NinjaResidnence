/**
* @file TitleScene.cpp
* @brief タイトルシーン
* @author Toshiya Matsuoka
*/
#include "Scene.h"
#include "TitleScene.h"
#include "Object/TitleCursol.h"


TitleScene::TitleScene(DirectX* pDirectX, SoundOperater* pSoundOperater) :Scene(pDirectX,pSoundOperater)
{
	m_pScene = this;
	m_pCursol = new TitleCursol(m_pDirectX, m_pSoundOperater);

	CreateSquareVertex(m_BackgroundVertex, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

TitleScene::~TitleScene()
{
	delete m_pCursol;
	m_pCursol = NULL;
	m_pDirectX->ClearTexture();
	m_pDirectX->ClearFont();
}

SCENE_NUM  TitleScene::Update()
{
	if (SoundLib::Playing != m_pSoundOperater->GetStatus("TITLE")) {
		bool buff = m_pSoundOperater->Start("TITLE", true);
	}
	m_pXinputDevice->DeviceUpdate();

	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_UP)|| KeyPush == m_pDirectX->GetKeyStatus(DIK_W) || PadPush == m_pXinputDevice->GetButton(ButtonUP)|| PadPush == m_pXinputDevice->GetAnalogLState(ANALOGUP))
	{
		m_pCursol->KeyOperation(UP);
		CursorAnimeInterval = 0;
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_DOWN)|| KeyPush == m_pDirectX->GetKeyStatus(DIK_S) || PadPush == m_pXinputDevice->GetButton(ButtonDOWN)|| PadPush == m_pXinputDevice->GetAnalogLState(ANALOGDOWN))
	{
		m_pCursol->KeyOperation(DOWN);
		CursorAnimeInterval = 0;
	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_RETURN) || KeyRelease == m_pDirectX->GetKeyStatus(DIK_NUMPADENTER)|| PadPush == m_pXinputDevice->GetButton(ButtonA)) {
		ChoseMenu();
	}
	m_pCursol->Update();
	CursorResize();
	return GetNextScene();
}

void TitleScene::Render()
{	
	m_pDirectX->DrawTexture("BACKGROUND_TEX", m_BackgroundVertex);

	CUSTOMVERTEX vertex[4];
	CreateSquareVertex(vertex, m_Logo);
	m_pDirectX->DrawTexture("LOGO_TEX", vertex);

	CreateSquareVertex(vertex, m_StartSize, m_CursorAlfa[0]);
	m_pDirectX->DrawTexture("TITLE_MENU_START_TEX", vertex);

	CreateSquareVertex(vertex, m_SettingSize, m_CursorAlfa[1]);
	m_pDirectX->DrawTexture("TITLE_MENU_SETTING_TEX", vertex);

	CreateSquareVertex(vertex, m_EndSize, m_CursorAlfa[2]);
	m_pDirectX->DrawTexture("TITLE_MENU_END_TEX", vertex);
}

void TitleScene::LoadResouce()
{
	m_pDirectX->LoadTexture("Resource/Texture/TitleLogo.png", "LOGO_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/Title_Start.png", "TITLE_MENU_START_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/Title_Setting.png", "TITLE_MENU_SETTING_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/Title_End.png", "TITLE_MENU_END_TEX");

	m_pDirectX->LoadTexture("Resource/Texture/Title_BG.jpg", "BACKGROUND_TEX");

	m_pDirectX->SetFont(100, 50, "DEBUG_FONT");
	m_pDirectX->SetFont(100, 40, "MENU_FONT");

	m_pSoundOperater->AddFile("Resource/Sound/bgm/title_bgm.mp3", "TITLE", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/tutorial_BGM.mp3", "TUTORIAL", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/stage_bgm_1.mp3", "STAGE_1", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/stage_bgm_2.mp3", "STAGE_2", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/stage_bgm_3.mp3", "STAGE_3", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/stage_bgm_4.mp3", "STAGE_4", BGM);
	m_pSoundOperater->AddFile("Resource/Sound/bgm/stage_bgm_5.mp3", "STAGE_5", BGM);

	m_pSoundOperater->AddFile("Resource/Sound/se/burning.mp3", "BURNING", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/dash.mp3", "DASH", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/door.mp3", "DOOR", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/drainage.mp3", "DRAINAGE", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/fire.mp3", "FIRE", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/itemget.mp3", "GET_ITEM", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/jump.wav", "JUMP", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/knifes.mp3", "SHURIKEN", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/landingon.mp3", "SET_DOWN", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/metal.mp3", "CLAWSHOT", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/reverse.mp3", "REVERSE", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/slash.mp3", "CUT_OFF", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/switch.mp3", "TARGET_ACTIVE", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/watershoes.mp3", "SET_DOWN_WATER", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/Failure.mp3", "FAILURE_SE", SE);
	m_pSoundOperater->AddFile("Resource/Sound/se/Goal.mp3", "GOAL_SE", SE);
}

void TitleScene::CursorResize() {
	static int SelectingCursor = 0;
	switch (SelectingCursor = m_pCursol->GetCursolPosition())
	{
	case Cursol::START:
		m_StartSize.scaleX = SelectingXScale;
		m_StartSize.scaleY = SelectingYScale;

		m_SettingSize.scaleX = InitXScale;
		m_SettingSize.scaleY = InitYScale;

		m_EndSize.scaleX = InitXScale;
		m_EndSize.scaleY = InitYScale;
		m_CursorAlfa[1] = InitColor;
		m_CursorAlfa[2] = InitColor;
		break;
	case Cursol::OPTION:
		m_SettingSize.scaleX = SelectingXScale;
		m_SettingSize.scaleY = SelectingYScale;

		m_StartSize.scaleX = InitXScale;
		m_StartSize.scaleY = InitYScale;

		m_EndSize.scaleX = InitXScale;
		m_EndSize.scaleY = InitYScale;
		m_CursorAlfa[0] = InitColor;
		m_CursorAlfa[2] = InitColor;
		break;
	case Cursol::END:
		m_EndSize.scaleX = SelectingXScale;
		m_EndSize.scaleY = SelectingYScale;

		m_StartSize.scaleX = InitXScale;
		m_StartSize.scaleY = InitYScale;

		m_SettingSize.scaleX = InitXScale;
		m_SettingSize.scaleY = InitYScale;
		m_CursorAlfa[0] = InitColor;
		m_CursorAlfa[1] = InitColor;
		break;
	}
	++CursorAnimeInterval;
	static bool CursorColorOn = false;
	if (CursorAnimeInterval > 20) {
		m_CursorAlfa[SelectingCursor] += (0xFF << 24) * ((CursorColorOn) ? +1 : -1);
		CursorColorOn = !CursorColorOn;
		CursorAnimeInterval = 0;
	}
}

void TitleScene::ChoseMenu() {
	switch (m_pCursol->GetCursolPosition()) {
	case Cursol::START:
		SetNextScene(STAGESELECT_SCENE);
		break;
	case Cursol::OPTION:
		m_SoundSetting = true;
		break;
	case Cursol::END:
		EndGame();
		break;

	}
}

