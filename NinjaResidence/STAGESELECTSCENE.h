/**
* @file STAGESELECTSCENE.h
* @brief ステージセレクトシーン
* @author Toshiya Matsuoka
*/
#pragma once

#include "SCENE.h"

enum SCENE_NUM;
class Scene;


class StageSelectScene : public Scene
{
public:
	StageSelectScene(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~StageSelectScene();
	void Initialize();
	SCENE_NUM Update();
	void Render();
	void LoadResouce();

private:
	void InitPosStageImage();
	void InitPosStageSelectNumber();
	void InitStageSelectNumberkey();

	Scene* m_pScene = NULL;
	
	int m_timecount = 0;
	
	const float OddNumPosX = 840.f;
	const float EvenNumPosX = 200.f;
	//ステージ選択時のクナイ（カーソル）の移動量
	const float KUNAI_MOVEMENT_X = 650.f;
	const float KUNAI_MOVEMENT_Y = 200.f;
	//戻るボタンに行き来するクナイの移動量
	const float MOVEMENT_X_TO_BACK = 140.f;
	const float MOVEMENT_Y_TO_BACK = 120.f;


	enum m_StageNumber
	{
		Stage0,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
		StageTitle,
	};
	DWORD m_CursorAlfa = 0xFFFFFFFF;
	CENTRAL_STATE m_SelectCursol = { 450.f,210.f,140,100 };
	CENTRAL_STATE m_StageSelectBack = { 120.f,70.f,80.f,50.f };
	CENTRAL_STATE m_StageImage[6];
	CENTRAL_STATE m_StageSelectNumber[6];
	CENTRAL_STATE m_StageFrame = { DISPLAY_WIDTH / 2,DISPLAY_HEIGHT / 2,270.f,270.f };
	std::string m_CursolImagekey;
	std::string m_BackImagekey;
	std::string m_StageImagekey[6];
	std::string m_StageSelectNumberkey[6];
};
