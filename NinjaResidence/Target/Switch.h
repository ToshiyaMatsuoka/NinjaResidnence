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
	Switch(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Switch();
	void ActivateTarget();
	void Update();
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState);

};

