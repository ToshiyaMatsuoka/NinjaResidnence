/**
* @file BlockInfo.h
* @brief enum BlockInfo及びMapDataStateの宣言とブロックに関する定数
* @author Kojiro Kawahara
*/

#pragma once
#define CELL_SIZE  40.f
#define BLOCK_INTEGRATION_WIDTH 80.2f / 512.0f
#define BLOCK_INTEGRATION_HEIGHT 81.0f / 1024.0f

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

namespace MapBlock
{
	/**
	* @enum BLOCKTYPE
	* ブロック種別
	*/
	enum BLOCKTYPE//CSVのブロックの番号
	{
		NONE,
		WOOD_BLOCK,
		ROCK_BLOCK,
		WOOD_TRACT,
		ROCK_TRACT,
		SPEAR,
		STAGE_DROP_ZONE,
		DESCRIPTION_BOARD = 7,
		DESCRIPTION_BOARD2 = 8,
		START_ZONE = 9,
		GOAL_ZONE = 10,
		WOOD_REVERSE_ZONE = 11,
		ROCK_REVERSE_ZONE = 12,

	};
}


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
