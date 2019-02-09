/**
* @file SCENE.h
* @brief SCENEクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "enum_Scene.h"
#include "GAMEMANAGER.h"
#include "XinputDevice.h"
#include "SoundOperater.h"

#define CENTRAL_Y (DISPLAY_HEIGHT / 2)
#define CENTRAL_X (DISPLAY_WIDTH / 2)



struct CENTRAL_STATE
{
	float x, y, scale_x, scale_y;
};

class Scene {
public:
	SCENE_NUM GetNextScene()
	{
		return m_NextScene;
	}
	void SetNextScene(SCENE_NUM NextScene)
	{
		m_NextScene = NextScene;
	}
	Scene(DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual ~Scene();
	virtual SCENE_NUM Update() = 0;
	virtual void LoadResouce() = 0;
	virtual void Render() = 0;
	void EndGame() {
		m_GameState = WM_QUIT;
	}


	void LoadAnimation();
	/**
	*@brief CUSTOMVERTEXにパラメータを入れる
	* @param Vertex 値を入れる配列
	* @param Central 中心座標情報
	* @param color 色
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void CreateSquareVertex(CUSTOMVERTEX* Vertex, CENTRAL_STATE Central, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	/**
	*@brief CUSTOMVERTEXにパラメータを入れる
	* @param Vertex 値を入れる配列
	* @param x 原点からのX軸距離
	* @param y 原点からのY軸距離
	* @param color 色
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void CreateSquareVertex(CUSTOMVERTEX* Vertex, float x, float y, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	void GoToOptionScene();
	void RunOptionScene(int m_BGMvolume, int m_SEvolume);
	int GetStageNum() {
		return m_StageNum;
	}
	int GetGameState() {
		return m_GameState;
	}
	bool GetSoundSetting() {
		return m_SoundSetting;
	}
	void InactiveSoundSetting() {
		m_SoundSetting = false;
	}

protected:
	DirectX* m_pDirectX = NULL;
	XinputDevice* m_pXinputDevice = NULL;
	SoundOperater* m_pSoundOperater = NULL;
	CUSTOMVERTEX  m_BackgroundVertex[4];

	static int m_StageNum;
	int m_GameState = WM_NULL;
	static const int ArrayLong = 64;

	//! 後で名称変更
	static bool m_SoundSetting;

	float DegToRad(float deg) {
		return deg * (D3DX_PI / 180);
	}

	void RevolveZ(CUSTOMVERTEX* Vertex, float Rad, CENTRAL_STATE Central, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	//次のシーン
	SCENE_NUM m_NextScene = SCENE_NONE;
	//! テキストファイルに指定の文字列を上書きする
	void WriteLog(std::string Text);
private:

};
