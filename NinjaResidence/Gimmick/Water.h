/**
* @file Water.h
* @brief Waterクラス
* @author Toshiya Matsuoka
*/
#pragma once

#include "BaseGimmick.h"

class Water : public BaseGimmick
{
public:
	void Activate();
	void Update();
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState );
	float GetGimmickPosition(bool isAxisX, MapDataState mapReverseState);
	Water(BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Water();
private:
	float m_TopPosition = 0;
	float m_PrevMovement = 0;
};
