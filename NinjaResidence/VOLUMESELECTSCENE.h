/**
* @file VOLUMESELECTSCENE.h
* @brief 音量設定画面
* @author Toshiya Matsuoka
*/
#pragma once

#include "SCENE.h"

enum SCENE_NUM;
class Scene;

enum TargetingSoundType {
	Target_BGM,
	Target_SE,
	Target_ALL,
};

class VOLUMESELECTSCENE :public Scene
{
public:
	VOLUMESELECTSCENE(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~VOLUMESELECTSCENE();

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
	* @param Value もとの数字
	* @param DigitNum ほしい桁数-1
	* @return 計算された桁数の数
	*/
	int DigitCalc(int Value, int DigitNum);
	
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
	static const int VolumeMaxNum = 3;
	const float NumTu = (102.f / 1024.f);
	const float NumTv = (186.f / 256.f);
	const float MenuHight = (810.f / 1024.f);
	const float TriangleWidth = (137.f / 512.f);
	const float DigitWidth = 75.f;
	const float TriangleCursolHeght = 150.f;

	const DWORD WHITE = 0xFFFFFFFF;
	DWORD m_CursorColor = 0xFFDA8C0D;
	
	const CENTRAL_STATE LeftCursolNeutral = { 500,200,40,30 };
	const CENTRAL_STATE RightCursolNeutral = { 850,200,40,30 };
	const float NeutralCursorY = 195;
	TargetingSoundType m_CursolPos = Target_BGM;
	CENTRAL_STATE m_LeftCursol[VolumeMaxNum] = { { LeftCursolNeutral },{ LeftCursolNeutral },{ LeftCursolNeutral } };
	CENTRAL_STATE m_RightCursol[VolumeMaxNum] = { { RightCursolNeutral },{ RightCursolNeutral },{ RightCursolNeutral } };
	CENTRAL_STATE m_Menu = { 300,360,100,250 };
	CENTRAL_STATE m_Cursor = { 675,NeutralCursorY,250,100 };


	const CENTRAL_STATE VolumeNumNeutral = { 750,0,50,90 };
	CENTRAL_STATE m_BGMVolumeNum[VolumeMaxNum] = { { VolumeNumNeutral },{ VolumeNumNeutral },{ VolumeNumNeutral } };
	CENTRAL_STATE m_SEVolumeNum[VolumeMaxNum] = { { VolumeNumNeutral },{ VolumeNumNeutral },{ VolumeNumNeutral } };
	CENTRAL_STATE m_ALLVolumeNum[VolumeMaxNum] = { { VolumeNumNeutral },{ VolumeNumNeutral },{ VolumeNumNeutral } };
	bool m_ExitScene = false;

};

