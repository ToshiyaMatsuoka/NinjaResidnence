/**
* @file BaseGimmick.h
* @brief BaseGimmickクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "../BlockInfo.h"
#include "../TargetandGimmickType.h"
#include "../Object/Object.h"


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

namespace VertexSetUp {

	inline void SetVertex(CUSTOMVERTEX* Vertex,float top,float left,float Yscale,float Xscale) {
		Vertex[0].x = left;
		Vertex[0].y = top;
		Vertex[1].x = left + Xscale;
		Vertex[1].y = top;
		Vertex[2].x = left + Xscale;
		Vertex[2].y = top + Yscale;
		Vertex[3].x = left;
		Vertex[3].y = top + Yscale;
	}
	inline void SetVertexUV(CUSTOMVERTEX* Vertex, float Tu, float Tv, float scaleTu = 1.f, float scaleTv = 1.f) {
		Vertex[0].tu = Tu;
		Vertex[0].tv = Tv;

		Vertex[1].tu = Tu + scaleTu;
		Vertex[1].tv = Tv;

		Vertex[2].tu = Tu + scaleTu;
		Vertex[2].tv = Tv + scaleTv;

		Vertex[3].tu = Tu;
		Vertex[3].tv = Tv + scaleTv;

	}

};