/**
* @file Rope.h
* @brief Ropeクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "BaseTarget.h"

class Rope :public BaseTarget
{
public:
	void ActivateTarget();
	void Update();
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse);
	Rope(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Rope();
private:
	float m_Movement = 0;
	float m_CellSize = CELL_SIZE;
};

