/**
* @file BaseTarget.h
* @brief BaseTargetクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "BaseGimmick.h"

#include "PartitionBoard.h"
#include "Water.h"
#include "FallRock.h"

#include "DirectX.h"


class BaseTarget
{
public:
	BaseTarget(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater);
	virtual ~BaseTarget();

	BlockInfo* GetTargetInfo() { return &m_TargetInfo; }
	void Activate();
	virtual void ActivateTarget() = 0;
	virtual void Update() = 0;
	/**
	* @brief ギミックの起動
	* @author Toshiya Matsuoka
	*/
	void ActivateGimmick();
	virtual void Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse) = 0;
	virtual float GetGimmickPosition(bool isAxisX, MapDataState MapDataReverse) { return m_pBaseGimmick->GetGimmickPosition(isAxisX, MapDataReverse); };
	virtual CUSTOMVERTEX* GetTargetPosition() { return m_TargetVertex; };
	/**
	* @brief ギミックのマップ上のY座標取得
	* @author Toshiya Matsuoka
	*/
	int GetGimmickYPos() { return m_GimmickInfo.PositionY; };
	
	/**
	* @brief ギミックのマップ上のX座標取得
	* @author Toshiya Matsuoka
	*/
	int GetGimmickXPos() { return m_GimmickInfo.PositionX; };
	
	/**
	* @brief ギミックが起動しているか取得
	* @return 起動していればtrue
	* @author Toshiya Matsuoka
	*/
	bool GetGimmickActive(int PosX);
protected:
	BaseGimmick * m_pBaseGimmick = NULL;
	DirectX* m_pDirectX = NULL;
	SoundOperater* m_pSoundOperater = NULL;

	BlockInfo m_TargetInfo;
	BlockInfo m_GimmickInfo;
	CUSTOMVERTEX m_TargetVertex[4];
	bool m_isActive = false;
	float m_TargetPosX;
	float m_TargetPosY;
	bool m_isSwitchedOn = false;

};
