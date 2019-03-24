/**
* @file BaseGimmick.h
* @brief BaseGimmickクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BlockInfo.h"
#include "TargetandGimmickType.h"
#include "Object/Object.h"


class BaseGimmick 
{
public:
	BlockInfo m_GimmickInfo;
	virtual void Activate() = 0;
	virtual void Update() = 0;
	virtual void Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState) = 0;
	BaseGimmick(BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual float GetGimmickPosition(bool isAxisX, MapDataState mapReverseState) { return 0; };
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

namespace VertexSetUp {

	inline void SetVertex(CUSTOMVERTEX* vertex,float top,float left,float yScale,float xScale) {
		vertex[0].x = left;
		vertex[0].y = top;
		vertex[1].x = left + xScale;
		vertex[1].y = top;
		vertex[2].x = left + xScale;
		vertex[2].y = top + yScale;
		vertex[3].x = left;
		vertex[3].y = top + yScale;
	}
	inline void SetVertexUV(CUSTOMVERTEX* vertex, float Tu, float Tv, float scaleTu = 1.f, float scaleTv = 1.f) {
		vertex[0].tu = Tu;
		vertex[0].tv = Tv;

		vertex[1].tu = Tu + scaleTu;
		vertex[1].tv = Tv;

		vertex[2].tu = Tu + scaleTu;
		vertex[2].tv = Tv + scaleTv;

		vertex[3].tu = Tu;
		vertex[3].tv = Tv + scaleTv;

	}

};