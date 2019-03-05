/**
* @file VolumeSelectScene.cpp
* @brief 音量設定画面
* @author Toshiya Matsuoka
*/
#include "VolumeSelectScene.h"
#include <cmath>


VolumeSelectScene::VolumeSelectScene(DirectX* pDirectX, SoundOperater* pSoundOperater) :Scene(pDirectX, pSoundOperater)
{

	for (int i = 0; i < VOLUME_MAX_NUM; ++i) {
		//! 仮画像のため仮置きの数字
		m_BGMVolumeNum[i].y = 200;
		m_SEVolumeNum[i].y = 350;
		m_ALLVolumeNum[i].y = 500;

		m_BGMVolumeNum[i].x = VOLUME_NUM_NEUTRAL.x - (i * DIGIT_WIDTH);
		m_SEVolumeNum[i].x = VOLUME_NUM_NEUTRAL.x - (i * DIGIT_WIDTH);
		m_ALLVolumeNum[i].x = VOLUME_NUM_NEUTRAL.x - (i * DIGIT_WIDTH);

		m_LeftCursol[i].y = LEFT_CURSOL_NEUTRAL.y + (i * TRIANGLE_CURSOL_HEGHT);
		m_RightCursol[i].y = RIGHT_CURSOL_NEUTRAL.y + 10 + (i * TRIANGLE_CURSOL_HEGHT);
	}
}


VolumeSelectScene::~VolumeSelectScene()
{

}

SCENE_NUM VolumeSelectScene::Update() {
	CursorMove();
	if (SoundLib::Playing != m_pSoundOperater->GetStatus("TITLE")) {
		bool buf = m_pSoundOperater->Start("TITLE", true);
	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_RETURN) || KeyRelease == m_pDirectX->GetKeyStatus(DIK_NUMPADENTER) || PadPush == m_pXinputDevice->GetButton(ButtonA))
	{
		ReturnScene();
	}
	m_pXinputDevice->DeviceUpdate();
	static int cursorAnimeInterval = 0;
	++cursorAnimeInterval;
	static bool cursorColorOn = false;
	if (cursorAnimeInterval > 20) {

		m_CursorColor += (0xFF << 24) * ((cursorColorOn) ? +1 : -1);

		cursorColorOn = !cursorColorOn;
		cursorAnimeInterval = 0;
	}
	if (KeyOn == m_pDirectX->GetKeyStatus(DIK_LEFT) || PadOn == m_pXinputDevice->GetButton(ButtonLEFT) || PadOn == m_pXinputDevice->GetAnalogLState(ANALOGLEFT))
	{
		static int keyInterval = 0;
		++keyInterval;
		if (keyInterval > 4) {
			DecreaseVolume();
			keyInterval = 0;
		}
	}
	if (KeyOn == m_pDirectX->GetKeyStatus(DIK_RIGHT) || PadOn == m_pXinputDevice->GetButton(ButtonRIGHT) || PadOn == m_pXinputDevice->GetAnalogLState(ANALOGRIGHT))
	{
		static int keyInterval = 0;
		++keyInterval;
		if (keyInterval > 4) {
			IncreaseVolume();
			keyInterval = 0;
		}

	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_UP) || PadRelease == m_pXinputDevice->GetButton(ButtonUP) || PadPush == m_pXinputDevice->GetAnalogLState(ANALOGUP))
	{
		MoveUp();
	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_DOWN) || PadRelease == m_pXinputDevice->GetButton(ButtonDOWN) || PadPush == m_pXinputDevice->GetAnalogLState(ANALOGDOWN))
	{
		MoveDown();
	}


	return SCENE_NONE;
}

void VolumeSelectScene::Render() {
	CUSTOMVERTEX setVolumeVertex[4];
	m_pDirectX->DrawTexture("VOLUME_BG_TEX", m_BackgroundVertex);

	CreateSquareVertex(setVolumeVertex, m_Menu, WHITE, 0, 0, 1, MENU_HIGHT);
	m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);

	CreateSquareVertex(setVolumeVertex, m_Menu, WHITE, 0, 0, 1, MENU_HIGHT);
	m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);

	CreateSquareVertex(setVolumeVertex, m_Cursor, m_CursorColor);
	m_pDirectX->DrawTexture("SV_CURSOR_TEX", setVolumeVertex);
	VolumeCursorRender();
	for (int i = 0; i < VOLUME_MAX_NUM; ++i) {

		int digitBuff = DigitCalc(m_BGMvolume, i);
		CreateSquareVertex(setVolumeVertex, m_BGMVolumeNum[i], WHITE, NUM_TU * digitBuff, 0, NUM_TU, NUM_TV);
		m_pDirectX->DrawTexture("NUMBER_TEX", setVolumeVertex);

		digitBuff = DigitCalc(m_SEvolume, i);
		CreateSquareVertex(setVolumeVertex, m_SEVolumeNum[i], WHITE, NUM_TU * digitBuff, 0, NUM_TU, NUM_TV);
		m_pDirectX->DrawTexture("NUMBER_TEX", setVolumeVertex);

		digitBuff = DigitCalc(m_ALLvolume, i);
		CreateSquareVertex(setVolumeVertex, m_ALLVolumeNum[i], WHITE, NUM_TU * digitBuff, 0, NUM_TU, NUM_TV);
		m_pDirectX->DrawTexture("NUMBER_TEX", setVolumeVertex);
	}
}

void VolumeSelectScene::VolumeCursorRender() {
	CUSTOMVERTEX setVolumeVertex[4];
	if (m_BGMvolume <100) {
		CreateSquareVertex(setVolumeVertex, m_RightCursol[0], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}
	if (m_BGMvolume > 0) {
		RevolveZ(setVolumeVertex, DegToRad(180), m_LeftCursol[0], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}
	if (m_SEvolume <100) {
		CreateSquareVertex(setVolumeVertex, m_RightCursol[1], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}
	if (m_SEvolume > 0) {
		RevolveZ(setVolumeVertex, DegToRad(180), m_LeftCursol[1], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}
	if (m_ALLvolume <100) {
		CreateSquareVertex(setVolumeVertex, m_RightCursol[2], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}
	if (m_ALLvolume > 0) {
		RevolveZ(setVolumeVertex, DegToRad(180), m_LeftCursol[2], WHITE, TRIANGLE_WIDTH, MENU_HIGHT, TRIANGLE_WIDTH, 1 - MENU_HIGHT);
		m_pDirectX->DrawTexture("SV_MENU_TEX", setVolumeVertex);
	}

}

void VolumeSelectScene::IncreaseVolume() {
	bool soundSuccess = false;
	switch (m_CursolPos) {
	case Target_BGM:
		++m_BGMvolume;
		if (m_BGMvolume> 100) {
			m_BGMvolume = 100;
		}
		soundSuccess = m_pSoundOperater->BGMSetVolume(m_BGMvolume);
		m_pSoundOperater->Start("TITLE", true);
		break;
	case Target_SE:
		++m_SEvolume;
		if (m_SEvolume> 100) {
			m_SEvolume = 100;
		}
		soundSuccess = m_pSoundOperater->SESetVolume(m_SEvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	case Target_ALL:
		++m_ALLvolume;
		if (m_ALLvolume> 100) {
			m_ALLvolume = 100;
		}
		m_BGMvolume = m_ALLvolume;
		m_SEvolume = m_ALLvolume;
		soundSuccess = m_pSoundOperater->AllSetVolume(m_ALLvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	}
}
void VolumeSelectScene::DecreaseVolume() {
	bool soundSuccess = false;
	switch (m_CursolPos) {
	case Target_BGM:
		--m_BGMvolume;
		if (m_BGMvolume <= 0) {
			m_BGMvolume = 0;
		}
		soundSuccess = m_pSoundOperater->BGMSetVolume(m_BGMvolume);
		m_pSoundOperater->Start("TITLE", true);
		break;
	case Target_SE:
		--m_SEvolume;
		if (m_SEvolume <= 0) {
			m_SEvolume = 0;
		}
		soundSuccess = m_pSoundOperater->SESetVolume(m_SEvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	case Target_ALL:
		--m_ALLvolume;
		if (m_ALLvolume <= 0) {
			m_ALLvolume = 0;
		}
		m_BGMvolume = m_ALLvolume;
		m_SEvolume = m_ALLvolume;
		soundSuccess = m_pSoundOperater->AllSetVolume(m_ALLvolume);
		m_pSoundOperater->Start("TITLE");
		break;
	}
}
int VolumeSelectScene::DigitCalc(int value, int digitNum) {

	int m_SoundKeyBuff = (value / static_cast<int>(std::pow(10, digitNum)));
	return m_SoundKeyBuff % 10;
}

void VolumeSelectScene::CursorMove() {
	switch (m_CursolPos) {
	case Target_BGM:
		m_Cursor.y = 195;
		break;
	case Target_SE:
		m_Cursor.y = 345;
		break;
	case Target_ALL:
		m_Cursor.y = 495;
		break;
	}
}
void VolumeSelectScene::MoveUp() {
	switch (m_CursolPos) {
	case Target_BGM:
		m_CursolPos = Target_ALL;
		break;
	case Target_SE:
		m_CursolPos = Target_BGM;
		break;
	case Target_ALL:
		m_CursolPos = Target_SE;
		break;
	}
}
void VolumeSelectScene::MoveDown() {
	switch (m_CursolPos) {
	case Target_BGM:
		m_CursolPos = Target_SE;
		break;
	case Target_SE:
		m_CursolPos = Target_ALL;
		break;
	case Target_ALL:
		m_CursolPos = Target_BGM;
		break;
	}
}

