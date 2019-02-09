/**
* @file DirectX.h
* @brief DirectX関連を行う
* @author Toshiya Matsuoka
*/
#pragma once

#include <windows.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>


struct CUSTOMVERTEX
{
	FLOAT	x, y, z, rhw;
	DWORD	color;
	FLOAT	tu, tv;
};

enum KEYSTATE
{
	//! 押されていない
	KeyOff,
	//! 押されている
	KeyOn,
	//! 押された
	KeyPush,
	//! 離された
	KeyRelease
};


//TODO:Singletonにする
/**
* @class DirectX
* @brief DirectX
*/
class DirectX {
public:
	DirectX();
	~DirectX();

	/**
	* @brief ディスプレイクリア
	*/
	void ClearDisplay();
	/**
	* @brief デバイスプレゼント
	* @sa GetDeviceState
	* @details プレゼントメソッドを呼び、DeviceStateにデバイスの状態を記録する
	*/
	void PresentsDevice();

	/**
	* @brief 描画開始関数
	* @sa DrowSceneEnd
	* @details DrowSceneEndとセットで使うこと
	*/
	void DrawSceneBegin();
	/**
	* @brief 描画終了関数
	* @sa DrowSceneBegin()
	* @details DrowSceneBeginとセットで使うこと
	*/
	void DrawSceneEnd();


	/**
	* @brief 描画開始関数
	* @sa RenderingEnd
	* @details ClearDisplayとDrowSceneBeginを行う
	*/
	void RenderingBegin();
	/**
	* @brief 描画終了関数
	* @sa RenderingBegin
	* @details DrowSceneEndとPresentsDeviceを行う
	*/
	void RenderingEnd();

	/**
	* @brief キーボード情報の読み取り
	*/
	void CheckKeyStatus();

	/**
	* @brief いずれかのキーが押されているかの確認
	* @return 何か押されていればtrue
	*/
	bool AnyKeyIsPressed();
	/**
	* @brief 指定のキー情報取得
	* @param KeyName DXINPUTキーの名前
	*/
	int GetKeyStatus(int KeyName) {
		return m_KeyState[KeyName];
	};
	/**
	* @brief 画像読み込み
	* @param FilePath 画像ファイルパス
	* @param TextureKey 画像の格納キー
	*/
	void LoadTexture(LPCSTR FilePath, std::string TextureKey);
	/**
	* @brief 画像表示
	* @param TextureKey 画像の格納キー
	* @param TextureSize 頂点情報
	*/
	void DrawTexture(std::string TextureKey, const CUSTOMVERTEX* TextureSize);
	/**
	* @brief 画像消去
	* @param TexKey 画像の格納キー
	*/
	void eraseTexture(std::string TexKey);
	/**
	* @brief 画像全消去
	*/
	void ClearTexture();
	/**
	* @brief DXフォント描画設定
	* @param rect 表示範囲
	* @param text 表示内容
	* @param FontNumber 表示文字設定の格納キー
	* @param TextFormat フォーマット
	* @param color 色
	*/
	void DrawWord(RECT rect, LPCSTR text, std::string FontNumber, int TextFormat = DT_LEFT, DWORD color = 0xff000000);
	/**
	* @brief DXフォント文字設定
	* @param height 文字の高さ
	* @param width 文字の幅
	* @param FontKey 表示文字設定の格納先配列番号
	* @param FontType 使用するフォント名
	* @param CharSet キャラセット（英字ならDEFAULT_CHARSET，シフトJISならSHIFTJIS_CHARSET）
	*/
	void SetFont(int height, int width, std::string FontKey, LPCSTR FontType = "ＭＳ　ゴシック", int CharSet = DEFAULT_CHARSET);
	/**
	* @brief DXフォント消去
	* @param FontKey 表示文字設定の格納先配列番号
	*/
	void eraseFont(std::string FontKey);
	/**
	* @brief DXフォント全消去
	*/
	void ClearFont();

	/**
	* @brief DirectXデバイスの作成
	* @param hWnd ウィンドウハンドル
	* @param isWindowMode ウィンドウモード　true:WindowMode　false:FullscreenMode
	* @param FilePath デバイス生成チェック用画像ファイルパス
	*/
	HRESULT BuildDXDevice(HWND hWnd, bool isWindowMode, LPCSTR FilePath);
	/**
	* @brief 各ウィンドウモードのパラメータセッティング
	* @param hWnd ウィンドウハンドル
	*/
	void InitPresentParameters(HWND hWnd);
	/**
	* @brief 解放処理
	*/
	void ReleaseDx();
	/**
	* @brief デバイスリセット
	* @param isWindowMode ウィンドウのモード
	* @param WinRect ウィンドウサイズの保蔵領域
	* @param hWnd ウィンドウハンドル
	*/
	HRESULT ResetDevice(bool isWindowMode, RECT* WinRect, HWND hWnd);
	/**
	* @brief デバイスの状態取得
	* @details デバイスプレゼント関数で取得したDeviceStateを取得する
	*/
	HRESULT GetDeviceState() {
		return DeviceState;
	}
	/**
	* @brief DirectXデバイスの復帰
	* @param hWnd ウィンドウハンドル
	* @param isWindowMode ウィンドウモード　true:WindowMode　false:FullscreenMode
	* @param FilePath デバイス生成チェック用画像ファイルパス
	*/
	HRESULT RecoverDevice(HWND hWnd, bool isWindowMode, LPCSTR FilePath);

private:
	std::map<std::string, LPDIRECT3DTEXTURE9> m_pTexture; //画像の情報を入れておく為のポインタ配列
	LPDIRECTINPUTDEVICE8 m_pKeyDevice = NULL; 
	LPDIRECTINPUT8 m_pDinput = NULL;			
	IDirect3DDevice9*  m_pD3Device = NULL; //Direct3Dのデバイス
	IDirect3D9*    m_pDirect3D = NULL; //DIrect3Dのインターフェース
	D3DPRESENT_PARAMETERS m_D3dPresentParameters;		//	パラメータ
	//D3DDISPLAYMODE		  m_D3DdisplayMode;
	std::map<std::string, LPD3DXFONT> m_pFont;
	D3DPRESENT_PARAMETERS m_d3dppWin, m_d3dppFull;
	static 	const int MaxKeyNumber = 256;

	BYTE m_KeyOldState[MaxKeyNumber];
	BYTE m_KeyState[MaxKeyNumber];

	HRESULT InitD3d(HWND hWnd, LPCSTR FilePath);
	HRESULT InitDinput(HWND hWnd);
	HRESULT DeviceState = NULL;

};
