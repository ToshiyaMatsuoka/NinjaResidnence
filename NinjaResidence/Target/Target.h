/**
* @file Target.h
* @brief Targetクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "Target/BaseTarget.h"
#include "BlockInfo.h"
#include "Object/Object.h"

class MapReverse;

class Target :public BaseTarget
{
public:
	void ActivateTarget();
	void Update();
	void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState);
	void RevolveZ(CUSTOMVERTEX * vertex, float rad, DWORD color, float tu, float tv, float scaleTu, float scaleTv);
	Target(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual ~Target();
private:
	int m_MotionBias = 0;
};
