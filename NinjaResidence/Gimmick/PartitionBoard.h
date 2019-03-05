/**
* @file PartitionBoard.h
* @brief PartitionBoardクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BaseGimmick.h"

class PartitionBoard : public BaseGimmick
{
public:
	void Activate();
	void Update();
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState);
	PartitionBoard(BlockInfo gimmick, DirectX* pDirectX, MapChip* pMapChip, SoundOperater* pSoundOperater);
	virtual ~PartitionBoard();
private:
	MapChip * m_pMapChip = NULL;
};
