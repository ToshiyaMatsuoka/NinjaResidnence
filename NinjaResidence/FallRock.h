/**
* @file FallRock.h
* @brief FallRockクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "DirectX.h"
#include "BaseGimmick.h"
#include "TargetandGimmickType.h"


class FallRock : public BaseGimmick
{
public:
	void Activate();
	void Update();
	void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse);
	FallRock(BlockInfo Gimmick, DirectX* pDirectX, MapChip* pMapChip, SoundOperater* pSoundOperater);
	virtual ~FallRock();
private:
	MapChip * m_pMapChip = NULL;
};
