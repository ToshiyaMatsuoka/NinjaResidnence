/**
* @file VolumeSelectScene.h
* @brief 音量設定画面
* @author Toshiya Matsuoka
*/
#pragma once

#include "Scene.h"

enum SCENE_NUM;
class Scene;

enum TargetingSoundType {
	Target_BGM,
	Target_SE,
	Target_ALL,
};

class VolumeSelectScene :public Scene
{
public:
	VolumeSelectScene(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~VolumeSelectScene();

	SCENE_NUM Update();
	void Render();
	void LoadResouce() {};

	void VolumeCursorRender();

	/**
	* @brief ボリュームの上昇
	*/
	void IncreaseVolume();
	
	/**
	* @brief ボリュームの下降
	*/
	void DecreaseVolume();
	
	/**
	* @brief 数字から指定する桁が10のn乗掛けるいくつか返す
	* @param value もとの数字
	* @param digitNum ほしい桁数-1
	* @return 計算された桁数の数
	*/
	int DigitCalc(int value, int digitNum);
	
	/**
	* @brief カーソルの今いる位置のY座標の設定
	*/
	void CursorMove();
	
	/**
	* @brief カーソルの上昇
	*/
	void MoveUp();
	
	/**
	* @brief カーソウルの下降
	*/
	void MoveDown();
	void ReturnScene() {
		m_pSoundOperater->Stop("TITLE");
		m_ExitScene = true;
	};
	bool GetExitScene() {
		return m_ExitScene;
	}
	void InitExitScene() {
		m_ExitScene = false;
	}
	
private:
	int m_BGMvolume = 100;
	int m_SEvolume = 100;
	int m_ALLvolume = 100;

	//! 音量の最大桁数と設定項目数
	static const int VOLUME_MAX_NUM = 3;
	const float NUM_TU = (102.f / 1024.f);
	const float NUM_TV = (186.f / 256.f);
	const float MENU_HIGHT = (810.f / 1024.f);
	const float TRIANGLE_WIDTH = (137.f / 512.f);
	const float DIGIT_WIDTH = 75.f;
	const float TRIANGLE_CURSOL_HEGHT = 150.f;

	const DWORD WHITE = 0xFFFFFFFF;
	DWORD m_CursorColor = 0xFFDA8C0D;
	
	const CENTRAL_STATE LEFT_CURSOL_NEUTRAL = { 500,200,40,30 };
	const CENTRAL_STATE RIGHT_CURSOL_NEUTRAL = { 850,200,40,30 };
	const float NEUTRAL_CURSOR_Y = 195;
	TargetingSoundType m_CursolPos = Target_BGM;
	CENTRAL_STATE m_LeftCursol[VOLUME_MAX_NUM] = { { LEFT_CURSOL_NEUTRAL },{ LEFT_CURSOL_NEUTRAL },{ LEFT_CURSOL_NEUTRAL } };
	CENTRAL_STATE m_RightCursol[VOLUME_MAX_NUM] = { { RIGHT_CURSOL_NEUTRAL },{ RIGHT_CURSOL_NEUTRAL },{ RIGHT_CURSOL_NEUTRAL } };
	CENTRAL_STATE m_Menu = { 300,360,100,250 };
	CENTRAL_STATE m_Cursor = { 675,NEUTRAL_CURSOR_Y,250,100 };


	const CENTRAL_STATE VOLUME_NUM_NEUTRAL = { 750,0,50,90 };
	CENTRAL_STATE m_BGMVolumeNum[VOLUME_MAX_NUM] = { { VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL } };
	CENTRAL_STATE m_SEVolumeNum[VOLUME_MAX_NUM] = { { VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL } };
	CENTRAL_STATE m_ALLVolumeNum[VOLUME_MAX_NUM] = { { VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL },{ VOLUME_NUM_NEUTRAL } };
	bool m_ExitScene = false;

};

