/**
* @file SceneManager.h
* @brief SceneManagerクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "./Scene/Scene.h"

class VolumeSelectScene;

class SceneManager
{
public:
	SceneManager(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~SceneManager();
	int Update();
	void Render();
	static void LoadResouce();
	static DWORD WINAPI Thread(LPVOID *data);

private:
	void LoadAnimation();

	SCENE_NUM m_CurrentScene;
	void KeyOperation();

	static Scene*	m_pScene;
	SCENE_NUM m_NextScene;
	DirectX* m_pDirectX = NULL;
	SoundOperater* m_pSoundOperater = NULL;
	VolumeSelectScene* m_pVolumeSettingScene = NULL;

	HANDLE m_threadHandle = NULL;
	DWORD m_threadResult = NULL;
	//! ロードのマルチスレッド処理
	void LoadAction();

	bool isRunOnce = false;
	bool isThreadActive = false;
	int LoadTime = 0;
	int TestTime = 0;
};
