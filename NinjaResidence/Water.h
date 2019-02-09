/**
* @file Water.h
* @brief Waterクラス
* @author Toshiya Matsuoka
*/
#pragma once

#include "DirectX.h"
#include "BaseGimmick.h"

class Water : public BaseGimmick
{
public:
	void Activate();
	void Update();
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse );
	float GetGimmickPosition(bool isAxisX, MapDataState MapDataReverse);
	Water(BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Water();
private:
	float m_TopPosition = 0;
	float m_PrevMovement = 0;
};
