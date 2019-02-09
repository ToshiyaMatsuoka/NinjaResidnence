/**
* @file GAMEMANAGER.h
* @brief Mainループ処理,Window関連
* @author Toshiya Matsuoka
*/
#pragma once

#include "DirectX.h"
#include "SceneManager.h"
#include "SoundOperater.h"
#include<iostream>
#include <fstream>
#include<sstream>
#include <algorithm>

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define DISPLAY_WIDTH 1280
#define DISPLAY_HEIGHT 720

void WriteLog(std::string Text);

class SceneManager;

class GAMEMANAGER
{
public:
	GAMEMANAGER(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);
	~GAMEMANAGER();
	int MessageLoop();
	static void ChangeDisplayMode(void);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
private:
	const char ApiName[10] = "忍者屋敷";
	static DirectX* pDirectX;
	static SceneManager* pSceneManager;
	static HWND hWnd;
	static bool isWindowMode;	//true:Window　false:Full
	static bool isDeviceLost;
	static RECT WinRect;			//Window Mode での位置大きさ
	static SoundOperater* pSoundOperater;
};
