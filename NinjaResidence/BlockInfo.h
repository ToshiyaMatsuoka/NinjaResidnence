/**
* @file BlockInfo.h
* @brief enum BlockInfo及びMapDataStateの宣言とブロックに関する定数
* @author Kojiro Kawahara
*/

#pragma once
//継承関係で面倒なため仮置き、いい置き方あれば教えてほしい
const float CELL_SIZE = 40.f;
const float BLOCK_INTEGRATION_WIDTH = 80.2f / 512.0f;
const float BLOCK_INTEGRATION_HEIGHT = 81.0f / 1024.0f;

class MapChip;

/**
* @enum MapDataState
* 表か裏か判別
*/
enum MapDataState
{
	SURFACE,//表
	REVERSE,//裏
	STATE_FALSE,
};

/**
* @struct BlockInfo
* ギミックのブロック情報
*/
struct BlockInfo
{
	int PositionX;//X座標
	int PositionY;//Y座標
	int PairNumber;//下2桁
	int GimmickType;//Targetなら1.2.3、Gimmickなら4.5.6
	MapDataState MapDataState;
	MapChip* pMapChip;
};
