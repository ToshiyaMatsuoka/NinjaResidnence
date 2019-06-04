/**
* @file Object.h
* @brief 色々な物体の基底クラス
* @author Toshiya Matsuoka
*/
#pragma once
#include <vector>

#include "BlockInfo.h"
#include "Scene/Scene.h"


enum KeyDirection
{
	PUSH_NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	JUMP,
	MOVE_LEFT,
	MOVE_RIGHT,
	SPACE,
	THROW,
	FIRE,
	END_ART,
	INERTIA,
	BIT_X_UP,
	BIT_X_DOWN,
	BIT_X_RIGHT,
	BIT_X_LEFT,
	BIT_D_UP,
	BIT_D_DOWN,
	BIT_D_RIGHT,
	BIT_D_LEFT,
	MAP_RIGHT,
	MAP_LEFT,
	MAP_DOWN,
	MAP_UP,
};

struct Vec2Int {
	int X = 0;
	int Y = 0;
};
typedef Vec2Int MapScroll;
typedef Vec2Int MapPosition;

struct Vector2 {
	float X = 0;
	float Y = 0;
};

class Object
{
public:
	/*
	*キー入力時の内部処理関数
	*/
	virtual void KeyOperation();
	virtual void KeyOperation(KeyDirection vec) {};

	virtual bool Update();

	virtual void Render() {};
	
	/*
	* @brief マップ内ブロックの起動処理
	* @param X 起動するブロックのマップ内X座標
	* @param Y 起動するブロックのマップ内Y座標
	*/
	virtual void Activate(int X,int Y) {};
	/*
	* @brief テクスチャの貼り付け
	* @param textureKey テクスチャキー
	* @param textureSize 貼り付け座標配列のアドレス
	*/
	void TextureRender(std::string textureKey, CUSTOMVERTEX* textureSize);

	Object(DirectX* pDirectX, SoundOperater* pSoundOperater);
	//Object(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* mapChip) {};
	virtual ~Object();

	/**
	* @brief CUSTOMVERTEXにパラメータを入れる
	* @param vertex 値を入れる配列
	* @param central 中心座標情報
	* @param color 色
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void CreateSquareVertex(CUSTOMVERTEX* vertex, CENTRAL_STATE central, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	/**
	* @brief CUSTOMVERTEXにパラメータを入れる
	* @param vertex 値を入れる配列
	* @param x 原点からのX軸距離
	* @param y 原点からのY軸距離
	* @param color 色
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void CreateSquareVertex(CUSTOMVERTEX* vertex, float x, float y, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	/**
	* @brief CUSTOMVERTEXに中心情報を左上としたパラメータの設定
	* @param central 中心座標情報
	* @param vertex 値を入れる配列
	* @param color 色
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void CreateSquareVertex(CENTRAL_STATE central, CUSTOMVERTEX* vertex, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	/**
	* @brief CUSTOMVERTEXから中心情報を作成する
	* @param central [out]
	* @param vertex [in]
	* @details 関連するCUSTOMVERTEX作成関数の逆動作をする
	*/
	void TranslateCentral_State(CENTRAL_STATE* central, CUSTOMVERTEX* vertex);
	void TranslateCentral_State(CUSTOMVERTEX* vertex, CENTRAL_STATE* central);

	/**
	* @brief CUSTOMVERTEXにUVパラメータを入れる
	* @param vertex 値を入れる配列
	* @param tu 切り取り画像の左端
	* @param tv 切り取り画像の上端
	* @param scaleTu 切り取り画像の右端
	* @param scaleTv 切り取り画像の下端
	*/
	void SetVertexUV(CUSTOMVERTEX* vertex, float Tu, float Tv, float scaleTu, float scaleTv);


	virtual void Reverse(Object* mapChip) {};

	//! 度数から弧度への変換
	float DegToRad(float deg) {
		return deg * (D3DX_PI / 180);
	}
	//! 弧度から度数への変換
	float RadToDeg(float rad) {
		return rad * ( 180/ D3DX_PI);
	}


	bool ContactSpecifyObject(CENTRAL_STATE* object);


	int GetRow() {
		return m_MapSizeX;
	}
	int GetColunm() {
		return m_MapSizeY;
	}

	int GetMapChipData(int height, int width) {
		if (height >= m_MapSizeY) {
			height = m_MapSizeY - 1;
		}
		if (height < 0) {
			height = 0;
		}
		if (width >= m_MapSizeX) {
			width = m_MapSizeX - 1;
		}
		if (width < 0) {
			width = 0;
		}
		int buf = -1;
		buf = MapData[height][width];
		return buf;
	}

	virtual MapDataState GetMapDataState() {
		MapDataState State = STATE_FALSE;
		return State;
	}

protected:
	DirectX* m_pDirectX = NULL;
	SoundOperater* m_pSoundOperater = NULL;
	CENTRAL_STATE m_Central;
	std::vector< std::vector<int> > MapData;
	
	//!行
	int m_MapSizeX = 0;
	//!列
	int m_MapSizeY = 0;

	static const int ARRAY_LONG = 64;
	
	//! Z軸回転
	void RevolveZ(CUSTOMVERTEX* vertex, float rad, CENTRAL_STATE central, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	
	//! 回転する中心を指定してZ軸回転
	void RevolveZ(CUSTOMVERTEX* vertex, float rad, CENTRAL_STATE central, float revolvingShaftX, float 	revolvingShaftY, DWORD  color = 0xffffffff, float tu = 0, float tv = 0, float scaleTu = 1, float scaleTv = 1);
	
	/**
	* @brief 貼り付ける画像の回転
	* @param vertex 回転させる頂点情報
	* @param deg 90度右に何回転させるか
	*/
	void RevolveTexture(CUSTOMVERTEX * vertex, int deg);

	static std::vector<BlockInfo> m_ReversePoint;
	static std::vector<MapScroll> m_ReverseBuffer;
	static int m_ReverseCount;
	static bool m_isReversing;
	static float m_Rad;
};
