/**
* @file PauseScene.cpp
* @brief ポーズ画面
* @author Toshiya Matsuoka
*/
#include "PauseScene.h"



PauseScene::PauseScene(DirectX* pDirectX, SoundOperater* pSoundOperater):Scene(pDirectX, pSoundOperater)
{

}


PauseScene::~PauseScene()
{
}

SCENE_NUM PauseScene::Update() {
	CursorMove();
	m_pXinputDevice->DeviceUpdate();
	static int CursorAnimeInterval = 0;
	++CursorAnimeInterval;
	static bool CursorColorOn = false;
	if (CursorAnimeInterval > 20) {

		m_CursorColor += (0xFF << 24) * ((CursorColorOn) ? +1 : -1);

		CursorColorOn = !CursorColorOn;
		CursorAnimeInterval = 0;
	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_RETURN) || KeyRelease == m_pDirectX->GetKeyStatus(DIK_NUMPADENTER) || PadPush == m_pXinputDevice->GetButton(ButtonA))
	{
		TransrateScene();
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_UP) || PadPush == m_pXinputDevice->GetButton(ButtonUP) || PadPush == m_pXinputDevice->GetAnalogLState(ANALOGUP))
	{
		MoveUp();
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_DOWN) || PadPush == m_pXinputDevice->GetButton(ButtonDOWN) || PadPush == m_pXinputDevice->GetAnalogLState(ANALOGDOWN))
	{
		MoveDown();
	}

	return m_NextScene;
}

void PauseScene::Render() {
	CUSTOMVERTEX PauseMenuVertex[4];
	m_pDirectX->DrawTexture("PAUSE_BG_TEX", m_BackgroundVertex);

	CreateSquareVertex(PauseMenuVertex, m_Menu);
	m_pDirectX->DrawTexture("PAUSEMENU_TEX", PauseMenuVertex);
	CreateSquareVertex(PauseMenuVertex, m_MenuTitle);
	m_pDirectX->DrawTexture("PAUSETITLE_TEX", PauseMenuVertex);
	CreateSquareVertex(PauseMenuVertex, m_Cursor, m_CursorColor);
	m_pDirectX->DrawTexture("SV_CURSOR_TEX", PauseMenuVertex);

}

void PauseScene::TransrateScene() {
	switch (m_CursolPos) {
	case RETURN_GAME:
		ReturnScene();
		break;
	case GOTO_SOUNDSETTING:
		m_SoundSetting = true;
		break;
	case GOTO_TITLE:
		SetNextScene(TITLE_SCENE);
		break;
	case END_GAME:
		EndGame();
		break;
	}

}

void PauseScene::CursorMove() {
	switch (m_CursolPos) {
	case RETURN_GAME:
		m_Cursor.y = 220;
		break;
	case GOTO_SOUNDSETTING:
		m_Cursor.y = 330;
		break;
	case GOTO_TITLE:
		m_Cursor.y = 450;
		break;
	case END_GAME:
		m_Cursor.y = 560;
		break;
	}
}
void PauseScene::MoveUp() {
	switch (m_CursolPos) {
	case RETURN_GAME:
		m_CursolPos = END_GAME;
		break;
	case GOTO_SOUNDSETTING:
		m_CursolPos = RETURN_GAME;
		break;
	case GOTO_TITLE:
		m_CursolPos = GOTO_SOUNDSETTING;
		break;
	case END_GAME:
		m_CursolPos = GOTO_TITLE;
		break;
	}
}

void PauseScene::MoveDown() {
	switch (m_CursolPos) {
	case RETURN_GAME:
		m_CursolPos = GOTO_SOUNDSETTING;
		break;
	case GOTO_SOUNDSETTING:
		m_CursolPos = GOTO_TITLE;
		break;
	case GOTO_TITLE:
		m_CursolPos = END_GAME;
		break;
	case END_GAME:
		m_CursolPos = RETURN_GAME;
		break;
	}
}

