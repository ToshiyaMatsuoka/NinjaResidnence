/**
* @file PartitionBoard.h
* @brief PartitionBoardクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "DirectX.h"
#include "BaseGimmick.h"

class PartitionBoard : public BaseGimmick
{
public:
	void Activate();
	void Update();
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse);
	PartitionBoard(BlockInfo Gimmick, DirectX* pDirectX, MapChip* pMapChip, SoundOperater* pSoundOperater);
	virtual ~PartitionBoard();
private:
	MapChip * m_pMapChip = NULL;
};
