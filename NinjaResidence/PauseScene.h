/**
* @file PauseScene.h
* @brief ポーズ画面
* @author Toshiya Matsuoka
*/
#pragma once
#include "SCENE.h"


enum TargetingMenu {
	RETURN_GAME,
	GOTO_SOUNDSETTING,
	GOTO_TITLE,
	END_GAME,
};


class PauseScene :public Scene
{
public:
	PauseScene(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~PauseScene();
	SCENE_NUM Update();
	void Render();
	/**
	* @brief メニュー選択後の処理
	*/
	void TransrateScene();
	/**
	* @brief メニューカーソル移動処理
	*/
	void CursorMove();
	/**
	* @brief メニューカーソル上昇
	*/
	void MoveUp();
	/**
	* @brief メニューカーソル下降
	*/
	void MoveDown();
	/**
	* @brief ゲームシーンに戻るフラグ立て
	*/
	void ReturnScene() {
		m_ExitScene = true;
	};
	/**
	* @brief ゲームシーンに戻るフラグ確認
	*/
	bool GetExitScene() {
		return m_ExitScene;
	}
	void InitExitScene() {
		m_ExitScene = false;
	}
	void LoadResouce() {};



private:
	bool m_ExitScene = false;

	DWORD m_CursorColor = 0xFFDA8C0D;

	TargetingMenu m_CursolPos = RETURN_GAME;
	const float NeutralCursorY = 0;
	CENTRAL_STATE m_Cursor = { CENTRAL_X,NeutralCursorY,250,100 };
	CENTRAL_STATE m_Menu = { CENTRAL_X,420,150,250 };
	CENTRAL_STATE m_MenuTitle = { 150,100,150,100 };

};

