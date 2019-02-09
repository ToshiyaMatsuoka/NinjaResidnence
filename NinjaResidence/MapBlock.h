/**
* @file MapBlock.h
* @brief enum BLOCKTYPEの宣言
* @author Kojiro Kawahara
*/
#pragma once

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

