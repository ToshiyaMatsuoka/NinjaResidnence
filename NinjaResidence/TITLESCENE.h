/**
* @file TITLESCENE.h
* @brief タイトルシーン
* @author Toshiya Matsuoka
*/
#pragma once

#include "SCENE.h"

enum SCENE_NUM;
class Scene;
class TitleCursol;

class TitleScene : public Scene
{
public:
	TitleScene(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~TitleScene();
	SCENE_NUM Update();
	void Render();
	void LoadResouce();


private:
	Scene* m_pScene = NULL;
	
	CENTRAL_STATE m_Logo = {350,200,250,100};
	CENTRAL_STATE m_StartSize   = {350,400,90,50};
	CENTRAL_STATE m_SettingSize = {350,500,90,50};
	CENTRAL_STATE m_EndSize     = {350,600,90,50};
	const DWORD InitColor = 0xFFFFFFFF;
	DWORD m_CursorAlfa[3] = { InitColor ,InitColor , InitColor };
	int m_timecount = 0;
	
	int CursorAnimeInterval = 0;

	TitleCursol* m_pCursol = NULL;
	const float InitYScale = 50.f;
	const float InitXScale = 100.f;
	const float SelectingYScale = 80.f;
	const float SelectingXScale = 130.f;
	void ChoseMenu();
	void CursorResize();

};
