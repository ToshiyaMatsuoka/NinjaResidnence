/**
* @file Torch.h
* @brief Torchクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BaseTarget.h"
#include "BlockInfo.h"
#include "Object.h"

class MapReverse;

class Torch :public BaseTarget
{
private:

public:
	void ActivateTarget();
	void Update();
	/**
	* @author Toshiya Matsuoka
	*/
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse);
	Torch(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Torch();
};
