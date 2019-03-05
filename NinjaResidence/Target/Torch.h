/**
* @file Torch.h
* @brief Torchクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BaseTarget.h"
#include "../Object/Object.h"

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
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState);
	Torch(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	~Torch();
};
