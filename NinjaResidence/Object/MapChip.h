/**
* @file MapChip.h
* @brief MapChipクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "Scene/GameScene.h"
#include "Object/Object.h"

class BaseTarget;
class GameScene;

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define FIELD_LEFT 0.f
#define FIELD_TOP  0.f



class MapChip :public Object
{
public:
	MapChip(DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual ~MapChip();
	/**
	* @brief マップチップの生成
	* @param filename CSVファイルの相対パス
	* @param mapState 表か裏かの指定
	* @sa enum MapDataState
	*/
	void Create(std::string filename, MapDataState mapState);
	//! ギミックの起動
	void Activate(int X, int Y);
	//! 指定箇所にマップに900を詰める
	void MapDataVectorHitSet(int mapY, int mapX, int gimmickY, int gimmickX);
	//! 指定箇所にマップに0を詰める
	void MapDataVectorZeroSet(int mapY, int mapX, int gimmickY, int gimmickX);
	//! マップデータの確認
	int GimmickMapDataCheck(int y, int x);
	//! ギミックペアの作成
	void CreateGimmick();
	void Render();
	bool Update();
	MapDataState GetMapDataState() {
		return m_MapDataState;
	}
	//! 0に近い指定のブロックのマップ座標の取得
	int SearchBlockX(MapBlock::BLOCKTYPE block);
	//! 0に近い指定のブロックのマップ座標の取得
	int SearchBlockY(MapBlock::BLOCKTYPE block);
	CUSTOMVERTEX* GetTargetPosition(int targetType);
	//! ギミックの座標の取得
	float GetGimmickPosition(bool isAxisX, int mapYPos,int mapXPos);
	//! ギミックの起動状態の取得
	bool GetGimmckActive(int mapXPos);

	static MapScroll GetScroll() {
		return m_MapScroll;
	}

	static void SetScroll(MapScroll scroll) {
		m_MapScroll = scroll;
	}

private:

	void RevolveY(CUSTOMVERTEX* Vertex, float Rad);

	int m_MapSelected = 0;
	int m_MapSelectedWIDTH = 0;
	int m_MapSelectedHEIGHT = 0;
	CUSTOMVERTEX CELL[4];
	//! CELLの初期化
	void CellInit();
	static int m_TargetCount;
	static int m_GimmickCount;

	static MapScroll m_MapScroll;


	static std::vector<BaseTarget*> pBaseTarget;
	static std::vector<BlockInfo> GimmickVector;
	static std::vector<BlockInfo> TargetVector;

	MapDataState m_MapDataState;
};
