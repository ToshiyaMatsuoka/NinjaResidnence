/**
* @file BaseGimmick.h
* @brief BaseGimmickクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BlockInfo.h"
#include "Object.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)


class BaseGimmick 
{
public:
	BlockInfo m_GimmickInfo;
	virtual void Activate() = 0;
	virtual void Update() = 0;
	virtual void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse) = 0;
	BaseGimmick(BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual float GetGimmickPosition(bool isAxisX, MapDataState MapDataReverse) { return 0; };
	bool GetActive() { return m_isActive; };
	virtual ~BaseGimmick();
protected:
	DirectX* m_pDirectX = NULL;
	SoundOperater* m_pSoundOperater = NULL;

	float m_QuantityOfMovement = 0.f;

	int m_GimmickPosX;
	int m_GimmickPosY;

	float m_WorldPosLeft;
	float m_WorldPosRight;
	float m_WorldPosTop;
	float m_WorldPosBottom;
	CUSTOMVERTEX m_GimmickVertex[4];

	bool m_isFirstTime = false;
	bool m_isActive = false;
};
