/**
* @file GAMEMANAGER.cpp
* @brief Mainループ処理,Window関連
* @author Toshiya Matsuoka
*/
#include "GAMEMANAGER.h"
#include <ctype.h>
#include <cstdio>
#include <cstdlib>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib,"SoundLib.lib")

//static変数の実体化
DirectX* GAMEMANAGER::pDirectX = NULL;
SceneManager* GAMEMANAGER::pSceneManager = NULL;
SoundOperater* GAMEMANAGER::pSoundOperater = NULL;
HWND GAMEMANAGER::hWnd = NULL;
bool GAMEMANAGER::isWindowMode = true;
bool GAMEMANAGER::isDeviceLost;
RECT GAMEMANAGER::WinRect;

GAMEMANAGER::GAMEMANAGER(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	hWnd = NULL;
	isWindowMode = true;	
	isDeviceLost = false;
	pDirectX = new DirectX;
	WNDCLASS Wndclass;
	pSoundOperater = new SoundOperater;
	//Windows初期化情報の設定
	Wndclass.style = CS_HREDRAW | CS_VREDRAW;
	Wndclass.lpfnWndProc = WndProc;
	Wndclass.cbClsExtra = Wndclass.cbWndExtra = 0;
	Wndclass.hInstance = hInstance;
	Wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	Wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	Wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	Wndclass.lpszMenuName = NULL;
	Wndclass.lpszClassName = ApiName;	//クラス名
	//Windowの登録
	RegisterClass(&Wndclass);
	//Windowの生成
	hWnd = CreateWindow(
		ApiName,					//ウィンドウのクラス名
		ApiName, 					//ウィンドウのタイトル
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	//ウィンドウスタイル
		CW_USEDEFAULT,						// ウィンドウの横方向の位置x
		CW_USEDEFAULT,						// ウィンドウの縦方向の位置y
		DISPLAY_WIDTH,						// Width（幅）
		DISPLAY_HEIGHT,						// Height（高さ）
		NULL,
		NULL,
		hInstance,							// アプリケーションインスタンスのハンドル
		NULL
	);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	pDirectX->InitPresentParameters(hWnd);
	pDirectX->BuildDXDevice(hWnd, isWindowMode, "texture/ninja.png");
	pSoundOperater->Initialize();


}

GAMEMANAGER::~GAMEMANAGER()
{
	delete pSceneManager;
	pSceneManager = NULL;
	delete pDirectX;
	pDirectX = NULL;
	delete pSoundOperater;
	pSoundOperater = NULL;
}


// 画面モードの変更
void GAMEMANAGER::ChangeDisplayMode(void)
{
	HRESULT hr = NULL;
	isWindowMode = !isWindowMode;
	pDirectX->ClearDisplay();
	pDirectX->DrawSceneBegin();
	pDirectX->DrawSceneEnd();
	pDirectX->PresentsDevice();

	hr = pDirectX->ResetDevice(isWindowMode, &WinRect,hWnd);

	pDirectX->ReleaseDx();
	pDirectX->BuildDXDevice(hWnd, isWindowMode, "texture/ninja.png");
	pSoundOperater->Initialize();
	pSceneManager->LoadResouce();

	if (FAILED(hr)) {
		if (hr == D3DERR_DEVICELOST) {
			isDeviceLost = true;
		}
		if (hr == D3DERR_DRIVERINTERNALERROR) {
			DestroyWindow(hWnd);
		}
		return;
	}

	if (isWindowMode) {
		SetWindowLong(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetWindowPos(hWnd, HWND_NOTOPMOST,
			WinRect.left, WinRect.top,
			WinRect.right - WinRect.left,
			WinRect.bottom - WinRect.top,
			SWP_SHOWWINDOW);
	}
	else {
		SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}
}

//メッセージ処理　ウィンドウプロシージャWndProcの設定
LRESULT CALLBACK GAMEMANAGER::WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	//! Alt + 特殊キーの処理に使う
	case WM_SYSKEYDOWN:     
		switch (wp) {
		//! Alt + Enterを押すと切り替え
		case VK_RETURN:
			ChangeDisplayMode();
			return 0;
		//! Alt + F4を押すと実行終了
		case VK_F4:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		default:
			return 0;
		}
		return 0;
	}
	return DefWindowProc(hWnd, msg, wp, lp);
}

int GAMEMANAGER::MessageLoop()
{

	MSG msg;
	DWORD SyncOld = timeGetTime();	//	システム時間を取得
	DWORD SyncNow;
	timeBeginPeriod(1);
	ZeroMemory(&msg, sizeof(msg));
	pSceneManager = new SceneManager(pDirectX,pSoundOperater);
	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			SyncNow = timeGetTime();
			if (SyncNow - SyncOld >= 1000 / 60)//1秒間に60回この中に入るはず
			{
				pDirectX->CheckKeyStatus();
				pDirectX->RenderingBegin();

				msg.message = pSceneManager->Update();
				pSceneManager->Render();

				pDirectX->RenderingEnd();

				SyncOld = SyncNow;
			}
			//!非アクティブ状態からの復帰処理
			if (D3DERR_DEVICELOST == pDirectX->GetDeviceState()&&
				hWnd == GetActiveWindow()) {
				HRESULT hr = NULL;
				pDirectX->ClearDisplay();
				pDirectX->DrawSceneBegin();
				pDirectX->DrawSceneEnd();
				pDirectX->PresentsDevice();

				hr = pDirectX->ResetDevice(isWindowMode, &WinRect, hWnd);
				pDirectX->ReleaseDx();

				pDirectX->RecoverDevice(hWnd, isWindowMode, "texture/ninja.png");
				pSoundOperater->Initialize();
				pSceneManager->LoadResouce();

				SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			}
		}
	}
	timeEndPeriod(1);
	return (int)msg.wParam;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	GAMEMANAGER* pGameManager = new GAMEMANAGER(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
	pGameManager->MessageLoop();
	delete pGameManager;
	pGameManager = NULL;
}

void WriteLog(std::string Text)
{
	const char* fileName = "test.txt";
	std::ofstream ofs(fileName, std::ios::out);

	if (!ofs)
	{
		std::cout << "ファイルが開けませんでした。" << std::endl;
		std::cin.get();
		return;
	}

	ofs << Text << std::endl;
	std::cout << fileName << "に書き込みました。" << std::endl;

	std::cin.get();
}
