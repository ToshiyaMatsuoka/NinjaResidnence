/**
* @file BaseTarget.cpp
* @brief BaseTargetクラス
* @author Kojiro Kawahara
*/
#include "BaseTarget.h"
#include "../TargetandGimmickType.h"

BaseTarget::BaseTarget(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater)
	:m_TargetInfo(target), m_GimmickInfo(gimmick), m_pDirectX(pDirectX)
{
	m_pSoundOperater = pSoundOperater;
	for (int i = 0; i < 4; ++i) {
		m_TargetVertex[i].color = 0xFFFFFFFF;
		m_TargetVertex[i].rhw = 1;
	}

	switch (m_GimmickInfo.GimmickType)
	{
	case BT_PARTITIONBOARD:
		m_pBaseGimmick = new PartitionBoard(gimmick, pDirectX, m_GimmickInfo.pMapChip, m_pSoundOperater);
		break;
	case BT_FALLROCK:
		m_pBaseGimmick = new FallRock(gimmick, pDirectX, m_GimmickInfo.pMapChip, m_pSoundOperater);
		break;
	case BT_WATER:
		m_pBaseGimmick = new Water(gimmick, pDirectX, m_pSoundOperater);
		break;
	case BT_ADHEREBOARD:
		//m_pBaseGimmick = new AdhereBoard(gimmick, pDirectX);
		break;
	}
}

BaseTarget::~BaseTarget()
{
	delete m_pBaseGimmick;
	m_pBaseGimmick = NULL;
}

bool BaseTarget::GetGimmickActive(int PosX)
{
	if (PosX != m_GimmickInfo.PositionX) {
		return false;
	}
	return m_pBaseGimmick->GetActive();
}
void BaseTarget::Activate()
{
	ActivateTarget();
}

void BaseTarget::ActivateGimmick()
{
	m_pBaseGimmick->Activate();
}
