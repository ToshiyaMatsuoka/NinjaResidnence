/**
* @file VOLUMESELECTSCENE.cpp
* @brief 音量設定画面
* @author Toshiya Matsuoka
*/
#include "VOLUMESELECTSCENE.h"
#include <cmath>


VOLUMESELECTSCENE::VOLUMESELECTSCENE(DirectX* pDirectX, SoundOperater* pSoundOperater) :Scene(pDirectX, pSoundOperater)
{

	for (int i = 0; i < VolumeMaxNum; ++i) {
		//! 仮画像のため仮置きの数字
		m_BGMVolumeNum[i].y = 200;
		m_SEVolumeNum[i].y = 350;
		m_ALLVolumeNum[i].y = 500;

		m_BGMVolumeNum[i].x = VolumeNumNeutral.x - (i * DigitWidth);
		m_SEVolumeNum[i].x = VolumeNumNeutral.x - (i * DigitWidth);
		m_ALLVolumeNum[i].x = VolumeNumNeutral.x - (i * DigitWidth);

		m_LeftCursol[i].y = LeftCursolNeutral.y + (i * TriangleCursolHeght);
		m_RightCursol[i].y = RightCursolNeutral.y + 10 + (i * TriangleCursolHeght);
	}
}


VOLUMESELECTSCENE::~VOLUMESELECTSCENE()
{

}

SCENE_NUM VOLUMESELECTSCENE::Update() {
	CursorMove();
	if (SoundLib::Playing != m_pSoundOperater->GetStatus("TITLE")) {
		bool buff = m_pSoundOperater->Start("TITLE", true);
	}
	if (KeyRelease == m_pDirectX->GetKeyStatus(DIK_RETURN) || KeyRelease == m_pDirectX->GetKeyStatus(DIK_NUMPADENTER) || PadPush == m_pXinputDevice->GetButton(ButtonA))
	{
		ReturnScene();
	}
	m_pXinputDevice->DeviceUpdate();
	static int CursorAnimeInterval = 0;
	++CursorAnimeInterval;
	static bool CursorColorOn = false;
	if (CursorAnimeInterval > 20) {

		m_CursorColor += (0xFF << 24) * ((CursorColorOn) ? +1 : -1);

		CursorColorOn = !CursorColorOn;
		CursorAnimeInterval = 0;
	}
	if (KeyOn == m_pDirectX->GetKeyStatus(DIK_LEFT) || PadOn == m_pXinputDevice->GetButton(ButtonLEFT) || PadOn == m_pXinputDevice->GetAnalogLState(ANALOGLEFT))
	{
		static int KeyInterval = 0;
		++KeyInterval;
		if (KeyInterval > 4) {
			DecreaseVolume();
			KeyInterval = 0;
		}
	}
	if (KeyOn == m_pDirectX->GetKeyStatus(DIK_RIGHT) || PadOn == m_pXinputDevice->GetButton(ButtonRIGHT) || PadOn == m_pXinputDevice->GetAnalogLState(ANALOGRIGHT))
	{
		static int KeyInterval = 0;
		++KeyInterval;
		if (KeyInterval > 4) {
			IncreaseVolume();
			KeyInterval = 0;
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

void VOLUMESELECTSCENE::Render() {
	CUSTOMVERTEX SetVolumeVertex[4];
	m_pDirectX->DrawTexture("VOLUME_BG_TEX", m_BackgroundVertex);

	CreateSquareVertex(SetVolumeVertex, m_Menu, WHITE, 0, 0, 1, MenuHight);
	m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);

	CreateSquareVertex(SetVolumeVertex, m_Menu, WHITE, 0, 0, 1, MenuHight);
	m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);

	CreateSquareVertex(SetVolumeVertex, m_Cursor, m_CursorColor);
	m_pDirectX->DrawTexture("SV_CURSOR_TEX", SetVolumeVertex);
	VolumeCursorRender();
	for (int i = 0; i < VolumeMaxNum; ++i) {

		int DigitBuff = DigitCalc(m_BGMvolume, i);
		CreateSquareVertex(SetVolumeVertex, m_BGMVolumeNum[i], WHITE, NumTu * DigitBuff, 0, NumTu, NumTv);
		m_pDirectX->DrawTexture("NUMBER_TEX", SetVolumeVertex);

		DigitBuff = DigitCalc(m_SEvolume, i);
		CreateSquareVertex(SetVolumeVertex, m_SEVolumeNum[i], WHITE, NumTu * DigitBuff, 0, NumTu, NumTv);
		m_pDirectX->DrawTexture("NUMBER_TEX", SetVolumeVertex);

		DigitBuff = DigitCalc(m_ALLvolume, i);
		CreateSquareVertex(SetVolumeVertex, m_ALLVolumeNum[i], WHITE, NumTu * DigitBuff, 0, NumTu, NumTv);
		m_pDirectX->DrawTexture("NUMBER_TEX", SetVolumeVertex);
	}
}

void VOLUMESELECTSCENE::VolumeCursorRender() {
	CUSTOMVERTEX SetVolumeVertex[4];
	if (m_BGMvolume <100) {
		CreateSquareVertex(SetVolumeVertex, m_RightCursol[0], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}
	if (m_BGMvolume > 0) {
		RevolveZ(SetVolumeVertex, DegToRad(180), m_LeftCursol[0], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}
	if (m_SEvolume <100) {
		CreateSquareVertex(SetVolumeVertex, m_RightCursol[1], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}
	if (m_SEvolume > 0) {
		RevolveZ(SetVolumeVertex, DegToRad(180), m_LeftCursol[1], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}
	if (m_ALLvolume <100) {
		CreateSquareVertex(SetVolumeVertex, m_RightCursol[2], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}
	if (m_ALLvolume > 0) {
		RevolveZ(SetVolumeVertex, DegToRad(180), m_LeftCursol[2], WHITE, TriangleWidth, MenuHight, TriangleWidth, 1 - MenuHight);
		m_pDirectX->DrawTexture("SV_MENU_TEX", SetVolumeVertex);
	}

}

void VOLUMESELECTSCENE::IncreaseVolume() {
	bool SoundSuccess = false;
	switch (m_CursolPos) {
	case Target_BGM:
		++m_BGMvolume;
		if (m_BGMvolume> 100) {
			m_BGMvolume = 100;
		}
		SoundSuccess = m_pSoundOperater->BGMSetVolume(m_BGMvolume);
		m_pSoundOperater->Start("TITLE", true);
		break;
	case Target_SE:
		++m_SEvolume;
		if (m_SEvolume> 100) {
			m_SEvolume = 100;
		}
		SoundSuccess = m_pSoundOperater->SESetVolume(m_SEvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	case Target_ALL:
		++m_ALLvolume;
		if (m_ALLvolume> 100) {
			m_ALLvolume = 100;
		}
		m_BGMvolume = m_ALLvolume;
		m_SEvolume = m_ALLvolume;
		SoundSuccess = m_pSoundOperater->AllSetVolume(m_ALLvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	}
}
void VOLUMESELECTSCENE::DecreaseVolume() {
	bool SoundSuccess = false;
	switch (m_CursolPos) {
	case Target_BGM:
		--m_BGMvolume;
		if (m_BGMvolume <= 0) {
			m_BGMvolume = 0;
		}
		SoundSuccess = m_pSoundOperater->BGMSetVolume(m_BGMvolume);
		m_pSoundOperater->Start("TITLE", true);
		break;
	case Target_SE:
		--m_SEvolume;
		if (m_SEvolume <= 0) {
			m_SEvolume = 0;
		}
		SoundSuccess = m_pSoundOperater->SESetVolume(m_SEvolume);
		m_pSoundOperater->Start("SET_DOWN_WATER");
		break;
	case Target_ALL:
		--m_ALLvolume;
		if (m_ALLvolume <= 0) {
			m_ALLvolume = 0;
		}
		m_BGMvolume = m_ALLvolume;
		m_SEvolume = m_ALLvolume;
		SoundSuccess = m_pSoundOperater->AllSetVolume(m_ALLvolume);
		m_pSoundOperater->Start("TITLE");
		break;
	}
}
int VOLUMESELECTSCENE::DigitCalc(int Value, int DigitNum) {

	int m_SoundKeyBuff = (Value / static_cast<int>(std::pow(10, DigitNum)));
	return m_SoundKeyBuff % 10;
}

void VOLUMESELECTSCENE::CursorMove() {
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
void VOLUMESELECTSCENE::MoveUp() {
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
void VOLUMESELECTSCENE::MoveDown() {
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

