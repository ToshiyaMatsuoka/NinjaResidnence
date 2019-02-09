/**
* @file Switch.h
* @brief Switchクラス
* @author Kojiro Kawahara
*/
#pragma once
#include "BaseTarget.h"
class Switch :
	public BaseTarget
{
public:
	Switch(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Switch();
	void ActivateTarget();
	void Update();
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse);

};

