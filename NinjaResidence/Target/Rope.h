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
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState);
	Rope(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Rope();
private:
	float m_Movement = 0;
	float m_CellSize = CELL_SIZE;
};

