/**
* @file STAGESELECTSCENE.cpp
* @brief ステージセレクトシーン
* @author Toshiya Matsuoka
*/
#include "STAGESELECTSCENE.h"

StageSelectScene::StageSelectScene(DirectX* pDirectX, SoundOperater* pSoundOperater) :Scene(pDirectX,pSoundOperater)
{
	m_StageNum = 0;
	m_pScene = this;
	CreateSquareVertex(m_BackgroundVertex, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	//初期値を決まる関数
	Initialize();
}

StageSelectScene::~StageSelectScene()
{
	m_pDirectX->ClearTexture();
	m_pDirectX->ClearFont();
}

void StageSelectScene::Initialize() {
	InitPosStageImage();
	InitPosStageSelectNumber();
	InitStageSelectNumberkey();

}
void StageSelectScene::InitPosStageImage()
{
	const float STAGEIMAGE_SCALE_X = 180.f;
	const float STAGEIMAGE_SCALE_Y = 80.f;
	for (int i = 0; i < 6; i++)
	{
		m_StageImage[i].scale_x = STAGEIMAGE_SCALE_X;
		m_StageImage[i].scale_y = STAGEIMAGE_SCALE_Y;
		m_StageImage[i].y = static_cast<float>((i / 2) + 1) * 200.f;
		if (i % 2)
		{
			m_StageImage[i].x = OddNumPosX;
		}
		else
		{
			m_StageImage[i].x = EvenNumPosX;
		}
	}
}

void StageSelectScene::InitPosStageSelectNumber()
{
	const float STAGESELECTNUMBER_SCALE_X = 150.f;
	const float STAGESELECTNUMBER_SCALE_Y = 50.f;
	for (int i = 0; i < 6; i++)
	{
		m_StageSelectNumber[i].scale_x = STAGESELECTNUMBER_SCALE_X;
		m_StageSelectNumber[i].scale_y = STAGESELECTNUMBER_SCALE_Y;
		m_StageSelectNumber[i].y = (static_cast<float>((i / 2) + 1) * 200.f) + 5.f;
		if (i % 2)
		{
			m_StageSelectNumber[i].x = OddNumPosX;
		}
		else
		{
			m_StageSelectNumber[i].x = EvenNumPosX;
		}
	}
}


void StageSelectScene::InitStageSelectNumberkey()
{
	m_StageSelectNumberkey[0] = "STAGESELECTNUMBERT_TEX";
	m_StageSelectNumberkey[1] = "STAGESELECTNUMBER1_TEX";
	m_StageSelectNumberkey[2] = "STAGESELECTNUMBER2_TEX";
	m_StageSelectNumberkey[3] = "STAGESELECTNUMBER3_TEX";
	m_StageSelectNumberkey[4] = "STAGESELECTNUMBER4_TEX";
	m_StageSelectNumberkey[5] = "STAGESELECTNUMBER5_TEX";
}


SCENE_NUM  StageSelectScene::Update()
{
	m_pXinputDevice->DeviceUpdate();

	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_RETURN) || KeyPush == m_pDirectX->GetKeyStatus(DIK_NUMPADENTER)|| PadPush == m_pXinputDevice->GetButton(ButtonA)) {
		if (m_StageNum == StageTitle)
		{
			SetNextScene(TITLE_SCENE);
		}
		else
		{
			m_pSoundOperater->Stop("TITLE");
			SetNextScene(GAME_SCENE);
		}
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_BACKSPACE) || PadPush == m_pXinputDevice->GetButton(ButtonB)) {
		m_StageNum = StageTitle;
		m_SelectCursol.y = 70.f;
		m_SelectCursol.x = 330.f;

	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_SPACE)) {
		m_StageNum = 8;
	}
	

	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_RIGHT)|| KeyPush == m_pDirectX->GetKeyStatus(DIK_D) || PadPush == m_pXinputDevice->GetButton(ButtonRIGHT)|| m_pXinputDevice->GetAnalogL(ANALOGRIGHT))
	{
		if (m_StageNum == Stage0)
		{
			m_StageNum = Stage1;
			m_SelectCursol.x += KUNAI_MOVEMENT_X;
		}
		if (m_StageNum == Stage2)
		{
			m_StageNum = Stage3;
			m_SelectCursol.x += KUNAI_MOVEMENT_X;
		}
		if (m_StageNum == Stage4)
		{
			m_StageNum = Stage5;
			m_SelectCursol.x += KUNAI_MOVEMENT_X;
		}
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_LEFT)|| KeyPush == m_pDirectX->GetKeyStatus(DIK_A) || PadPush == m_pXinputDevice->GetButton(ButtonLEFT)|| m_pXinputDevice->GetAnalogL(ANALOGLEFT))
	{
		if (m_StageNum == Stage1)
		{
			m_StageNum = Stage0;
			m_SelectCursol.x -= KUNAI_MOVEMENT_X;
		}
		if (m_StageNum == Stage3)
		{
			m_StageNum = Stage2;
			m_SelectCursol.x -= KUNAI_MOVEMENT_X;
		}
		if (m_StageNum == Stage5)
		{
			m_StageNum = Stage4;
			m_SelectCursol.x -= KUNAI_MOVEMENT_X;
		}
	}

	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_UP) || KeyPush == m_pDirectX->GetKeyStatus(DIK_W) || PadPush == m_pXinputDevice->GetButton(ButtonUP) || m_pXinputDevice->GetAnalogL(ANALOGUP)) {
		if (m_StageNum == Stage0)
		{
			m_StageNum = StageTitle;
			m_SelectCursol.y -= MOVEMENT_X_TO_BACK;
			m_SelectCursol.x -= MOVEMENT_Y_TO_BACK;
		}
		if (m_StageNum == Stage1)
		{
			m_StageNum = StageTitle;
			m_SelectCursol.y -= MOVEMENT_X_TO_BACK;
			//
			m_SelectCursol.x -= 770;
		}
		if (m_StageNum == Stage2)
		{
			m_StageNum = Stage0;
			m_SelectCursol.y -= KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage3)
		{
			m_StageNum = Stage1;
			m_SelectCursol.y -= KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage4)
		{
			m_StageNum = Stage2;
			m_SelectCursol.y -= KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage5)
		{
			m_StageNum = Stage3;
			m_SelectCursol.y -= KUNAI_MOVEMENT_Y;
		}
	}
	if (KeyPush == m_pDirectX->GetKeyStatus(DIK_DOWN) || KeyPush == m_pDirectX->GetKeyStatus(DIK_S) || PadPush == m_pXinputDevice->GetButton(ButtonDOWN) || m_pXinputDevice->GetAnalogL(ANALOGDOWN)) {
		if (m_StageNum == Stage3)
		{
			m_StageNum = Stage5;
			m_SelectCursol.y += KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage2)
		{
			m_StageNum = Stage4;
			m_SelectCursol.y += KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage0)
		{
			m_StageNum = Stage2;
			m_SelectCursol.y += KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == Stage1)
		{
			m_StageNum = Stage3;
			m_SelectCursol.y += KUNAI_MOVEMENT_Y;
		}
		if (m_StageNum == StageTitle)
		{
			m_StageNum = Stage0;
			m_SelectCursol.y += MOVEMENT_X_TO_BACK;
			m_SelectCursol.x += MOVEMENT_Y_TO_BACK;
		}
	}
	return GetNextScene();
}

void StageSelectScene::Render()
{
	static int CursorAnimeInterval = 0;
	++CursorAnimeInterval;
	static bool CursorColorOn = false;
	if (CursorAnimeInterval > 20) {

		m_CursorAlfa += (0xFF << 24) * ((CursorColorOn) ? +1 : -1);

		CursorColorOn = !CursorColorOn;
		CursorAnimeInterval = 0;
	}

	m_pDirectX->DrawTexture("SELECT_BG_TEX", m_BackgroundVertex);

	CUSTOMVERTEX StageImage[4];
	if (m_StageNum != 8) 	{
		for (int i = 0; i < 6; ++i) {
			CreateSquareVertex(StageImage, m_StageImage[i]);
			m_pDirectX->DrawTexture("STAGEIMAGE_TEX", StageImage);

			CreateSquareVertex(StageImage, m_StageSelectNumber[i]);
			m_pDirectX->DrawTexture(m_StageSelectNumberkey[i], StageImage);
		}
	}
	else {
		CreateSquareVertex(StageImage, m_StageImage[0]);
		m_pDirectX->DrawTexture("STAGEIMAGED_TEX", StageImage);
	}

	CreateSquareVertex(StageImage, m_StageSelectBack);
	m_pDirectX->DrawTexture("STAGESELECTBACK_TEX", StageImage);

	CreateSquareVertex(StageImage, m_SelectCursol, m_CursorAlfa);
	m_pDirectX->DrawTexture("KUNAI_TEX", StageImage);
#ifdef _DEBUG
	RECT testName = { 0, 100, 1250, 720 };
	char TestName[ArrayLong];
	sprintf_s(TestName, ArrayLong, "Cursol X:%.2f Y:%.2f", m_SelectCursol.x, m_SelectCursol.y);
	m_pDirectX->DrawWord(testName, TestName, "DEBUG_FONT", DT_RIGHT, 0xffffffff);

#endif
}

void StageSelectScene::LoadResouce()
{
	m_pDirectX->LoadTexture("texture/SelectBG.png", "SELECT_BG_TEX");
	m_pDirectX->LoadTexture("texture/StageImage.png", "STAGEIMAGE_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumberT.png", "STAGESELECTNUMBERT_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumber1.png", "STAGESELECTNUMBER1_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumber2.png", "STAGESELECTNUMBER2_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumber3.png", "STAGESELECTNUMBER3_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumber4.png", "STAGESELECTNUMBER4_TEX");
	m_pDirectX->LoadTexture("texture/StageSelectNumber5.png", "STAGESELECTNUMBER5_TEX");
	m_pDirectX->LoadTexture("texture/Kunai.png", "KUNAI_TEX");
	m_pDirectX->LoadTexture("texture/StagexSelectBack.png", "STAGESELECTBACK_TEX");
	m_pDirectX->LoadTexture("texture/StageImageD.jpg", "STAGEIMAGED_TEX");

	m_pDirectX->SetFont(50, 20, "DEBUG_FONT");

}

