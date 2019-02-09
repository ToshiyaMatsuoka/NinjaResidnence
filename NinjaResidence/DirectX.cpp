/**
* @file DirectX.cpp
* @brief DirectX関連を行う
* @author Toshiya Matsuoka
*/
#include "DirectX.h"
#include "GAMEMANAGER.h"
#include <algorithm>

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define SAFE_RELEASE(p) {if(p){(p)->Release(); (p)=NULL;}}

using std::map;
using std::string;


DirectX::DirectX() {

}
DirectX::~DirectX() {
	ReleaseDx();
}
//メモリ開放
void DirectX::ReleaseDx()
{
	m_pTexture.clear();
	map<string, LPDIRECT3DTEXTURE9>().swap(m_pTexture);
	m_pFont.clear();
	map<string, LPD3DXFONT>().swap(m_pFont);
	if (m_pKeyDevice)
	{
		m_pKeyDevice->Unacquire();
	}
	SAFE_RELEASE(m_pKeyDevice);
	SAFE_RELEASE(m_pDinput);
	SAFE_RELEASE(m_pD3Device);
	SAFE_RELEASE(m_pDirect3D);
}

//ダイレクト3Dの初期化関数
HRESULT DirectX::InitD3d(HWND hWnd, LPCSTR FilePath)
{
	HRESULT hr = NULL;
	// 「Direct3D」オブジェクトの作成
	if (!(m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;

	if (FAILED(hr = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		&d3dpp, &m_pD3Device)))
	{
		MessageBox(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
		if (FAILED(hr = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
			D3DCREATE_MIXED_VERTEXPROCESSING,
			&d3dpp, &m_pD3Device)))
		{
			MessageBox(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
			return E_FAIL;
		}
	}
	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(m_pD3Device, FilePath, 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &m_pTexture["Test"])))
	{
		eraseTexture("Test");
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	eraseTexture("Test");
	return S_OK;
}

//ダイレクトインプットの初期化関数
HRESULT DirectX::InitDinput(HWND hWnd)
{
	HRESULT hr;

	//ダイレクトインプットのオブジェの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDinput, NULL)))
	{
		return hr;
	}
	//ダイレクトインプットのデバイスの作成
	if (FAILED(hr = m_pDinput->CreateDevice(GUID_SysKeyboard,
		&m_pKeyDevice, NULL)))
	{
		return hr;
	}
	//デバイスをキーボードの設定
	if (FAILED(hr = m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	//協調レベル
	if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
	return S_OK;
}

HRESULT DirectX::BuildDXDevice(HWND hWnd, bool WinMode, LPCSTR FilePath) {
	//ダイレクト３Dの初期化関数を呼ぶ
	if (FAILED(InitD3d(hWnd, FilePath)))
	{
		return E_FAIL;
	}
	//ダイレクトインプットの初期化関数を呼ぶ
	if (FAILED(InitDinput(hWnd)))
	{
		MessageBox(0, "DirectInputDeviceの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//DirectX オブジェクトの生成
	m_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);
	//成功チェック
	if (m_pDirect3D == NULL)
	{
		//生成に失敗したら終了する
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	m_D3dPresentParameters = (WinMode) ? m_d3dppWin : m_d3dppFull;

	m_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_D3dPresentParameters, &m_pD3Device);
	//生成チェック
	if (m_pD3Device == NULL)
	{
		//生成に失敗したらDirectXオブジェクトを開放して終了する
		m_pDirect3D->Release();
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	//描画設定
	m_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	m_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//頂点に入れるデータを設定
	m_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	return S_OK;
}

void DirectX::InitPresentParameters(HWND hWnd) {
	//Window
	ZeroMemory(&m_d3dppWin, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dppWin.BackBufferWidth = 0;
	m_d3dppWin.BackBufferHeight = 0;
	m_d3dppWin.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dppWin.BackBufferCount = 1;
	m_d3dppWin.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dppWin.MultiSampleQuality = 0;
	m_d3dppWin.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dppWin.hDeviceWindow = hWnd;
	m_d3dppWin.Windowed = TRUE;
	m_d3dppWin.EnableAutoDepthStencil = FALSE;
	m_d3dppWin.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	m_d3dppWin.Flags = 0;
	m_d3dppWin.FullScreen_RefreshRateInHz = 0;
	m_d3dppWin.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//Full
	ZeroMemory(&m_d3dppFull, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dppFull.BackBufferWidth = DISPLAY_WIDTH;
	m_d3dppFull.BackBufferHeight = DISPLAY_HEIGHT;
	m_d3dppFull.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dppFull.BackBufferCount = 1;
	m_d3dppFull.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_d3dppFull.MultiSampleQuality = 0;
	m_d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dppFull.hDeviceWindow = hWnd;
	m_d3dppFull.Windowed = FALSE;
	m_d3dppFull.EnableAutoDepthStencil = FALSE;
	m_d3dppFull.AutoDepthStencilFormat = D3DFMT_A1R5G5B5;
	m_d3dppFull.Flags = 0;
	m_d3dppFull.FullScreen_RefreshRateInHz = 0;
	m_d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
}
/*
*DirectInput
*/

void DirectX::CheckKeyStatus() {
	HRESULT hr = m_pKeyDevice->Acquire();
	if ((hr == DI_OK) || (hr == S_FALSE)) {
		m_pKeyDevice->GetDeviceState(sizeof(m_KeyState), &m_KeyState);
		for (int i = 0; i < MaxKeyNumber; i++) {
			if (m_KeyState[i] & 0x80)
			{
				if (m_KeyOldState[i] == KeyOn)
				{
					m_KeyOldState[i] = KeyOn;
					m_KeyState[i] = KeyOn;
					continue;
				}
				else
				{
					m_KeyOldState[i] = KeyOn;
					m_KeyState[i] = KeyPush;
					continue;
				}
			}
			else
			{
				if (m_KeyOldState[i] == KeyOn)
				{
					m_KeyOldState[i] = KeyOff;
					m_KeyState[i] = KeyRelease;
					continue;
				}
				else
				{
					m_KeyOldState[i] = KeyOff;
					m_KeyState[i] = KeyOff;
					continue;
				}
			}
		}
	}
}

bool DirectX::AnyKeyIsPressed() {
	BYTE* tmp = std::find_if(std::begin(m_KeyState), std::end(m_KeyState), [](BYTE KeyState) {return KeyState != KeyOff; });
	return *tmp;
}
/*
*
*/

void DirectX::ClearDisplay() {
	m_pD3Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0x00, 0x00, 0x00), 1.0, 0);
}
void DirectX::PresentsDevice() {
	DeviceState = m_pD3Device->Present(NULL, NULL, NULL, NULL);
}
void DirectX::DrawSceneBegin() {
	m_pD3Device->BeginScene();
}
void DirectX::DrawSceneEnd() {
	m_pD3Device->EndScene();
}

void DirectX::RenderingBegin() {
	ClearDisplay();
	DrawSceneBegin();
}

void DirectX::RenderingEnd() {
	DrawSceneEnd();
	PresentsDevice();
}

/*
*画像描画処理
*/

void DirectX::LoadTexture(LPCSTR FilePath, string TextureKey) {
	D3DXCreateTextureFromFile(
		m_pD3Device,
		FilePath,
		&m_pTexture[TextureKey]);
}
void DirectX::DrawTexture(string TextureKey, const CUSTOMVERTEX* TextureSize) {
	m_pD3Device->SetTexture(0, m_pTexture[TextureKey]);
	m_pD3Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, TextureSize, sizeof(CUSTOMVERTEX));
}
void DirectX::eraseTexture(string TexKey) {
	if (!m_pTexture[TexKey]) {
		return;
	}
	m_pTexture[TexKey]->Release();
	m_pTexture.erase(TexKey);
}
void DirectX::ClearTexture() {
	if (!m_pTexture.size()) {
		return;
	}
	for (auto i : m_pTexture) {
		if (!i.second)continue;
		(i.second)->Release();
	}
	m_pTexture.clear();
	map<string, LPDIRECT3DTEXTURE9>().swap(m_pTexture);
}
/*
*DxFont
*/

void DirectX::DrawWord(RECT rect, LPCSTR text, string FontNumber, int TextFormat, DWORD color) {
	m_pFont[FontNumber]->DrawText(
		NULL,		// NULL
		text,		// 描画テキスト
		-1,			// 全て表示
		&rect,		// 表示範囲
		TextFormat,	// 左寄せ
		color		// color
	);
}
void DirectX::SetFont(int height, int width, string FontKey, LPCSTR FontType, int CharSet) {
	D3DXCreateFont(
		m_pD3Device,
		height,
		width,
		FW_DEMIBOLD,
		NULL,
		FALSE,
		CharSet,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FIXED_PITCH | FF_SCRIPT,
		FontType,
		&m_pFont[FontKey]);
}
void DirectX::eraseFont(string FontKey) {
	if (!m_pFont[FontKey]) {
		return;
	}
	m_pFont[FontKey]->Release();
	m_pFont.erase(FontKey);
}
void DirectX::ClearFont() {
	if (!m_pFont.size()) {
		return;
	}
	for (auto i : m_pFont) {
		if (!i.second)continue;
		(i.second)->Release();
	}
	m_pFont.clear();
	map<string, LPD3DXFONT>().swap(m_pFont);

}
HRESULT DirectX::ResetDevice(bool isWindowMode, RECT* WinRect, HWND hWnd) {
	if (isWindowMode) {
		m_D3dPresentParameters = m_d3dppWin;
	}
	else {
		m_D3dPresentParameters = m_d3dppFull;
		GetWindowRect(hWnd, WinRect);
	}
	return m_pD3Device->Reset(&m_D3dPresentParameters);
}

HRESULT DirectX::RecoverDevice(HWND hWnd, bool WinMode, LPCSTR FilePath) {

	HRESULT hr = NULL;
	// 「Direct3D」オブジェクトの作成
	if (!(m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	m_D3dPresentParameters = (WinMode) ? m_d3dppWin : m_d3dppFull;

	m_pDirect3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_D3dPresentParameters, &m_pD3Device);
	//生成チェック
	if (m_pD3Device == NULL)
	{
		//生成に失敗したらDirectXオブジェクトを開放して終了する
		m_pDirect3D->Release();
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}


	//ダイレクトインプットのオブジェの作成
	if (FAILED(hr = DirectInput8Create(GetModuleHandle(NULL),
		DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDinput, NULL)))
	{
		return hr;
	}
	//ダイレクトインプットのデバイスの作成
	if (FAILED(hr = m_pDinput->CreateDevice(GUID_SysKeyboard,
		&m_pKeyDevice, NULL)))
	{
		return hr;
	}
	//デバイスをキーボードの設定
	if (FAILED(hr = m_pKeyDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return hr;
	}
	//協調レベル
	if (FAILED(hr = m_pKeyDevice->SetCooperativeLevel(
		hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
	{
		return hr;
	}
		//DirectX オブジェクトの生成
	m_pDirect3D = Direct3DCreate9(
		D3D_SDK_VERSION);
	//成功チェック
	if (m_pDirect3D == NULL)
	{
		//生成に失敗したら終了する
		MessageBox(0, "Direct3Dの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}

	//「テクスチャオブジェクト」の作成
	if (FAILED(D3DXCreateTextureFromFileEx(m_pD3Device, FilePath, 100, 100, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000000, NULL, NULL, &m_pTexture["Test"])))
	{
		eraseTexture("Test");
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	eraseTexture("Test");


	//描画設定
	m_pD3Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pD3Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);  //SRCの設定
	m_pD3Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//m_pD3Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//頂点に入れるデータを設定
	m_pD3Device->SetFVF(D3DFVF_CUSTOMVERTEX);
	return S_OK;

}
