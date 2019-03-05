/**
* @file Switch.cpp
* @brief Switchクラス
* @author Kojiro Kawahara
*/
#include "Switch.h"



Switch::Switch(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(target, gimmick, pDirectX,pSoundOperater)
{
}


Switch::~Switch()
{
}

void Switch::ActivateTarget()
{
	m_pBaseGimmick->Activate();
}

void Switch::Update()
{
	m_pBaseGimmick->Update();
}


void Switch::Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState)
{
	if (mapReverseState == m_TargetInfo.MapDataState)
	{
		m_TargetPosX = static_cast<float>(m_TargetInfo.PositionX);
		m_TargetPosY = static_cast<float>(m_TargetInfo.PositionY);
		//TODO:的のオンオフ切り替え
		if (mapReverseState == m_TargetInfo.MapDataState)
		{
			VertexSetUp::SetVertex(m_TargetVertex, (CELL_SIZE * (m_TargetPosY)) + mapScrollY, (CELL_SIZE * m_TargetPosX) + mapScrollX, CELL_SIZE, CELL_SIZE);

			VertexSetUp::SetVertexUV(m_TargetVertex, BLOCK_INTEGRATION_WIDTH * 4.f, 0.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);

			m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
		}
	}

	if (m_GimmickInfo.GimmickType == BT_WATER)
	{
		m_pBaseGimmick->Render(mapScrollY, mapScrollX, mapReverseState);
	}
	if (m_GimmickInfo.GimmickType == BT_PARTITIONBOARD)
	{
		m_pBaseGimmick->Render(mapScrollY, mapScrollX, mapReverseState);
	}
	if (m_GimmickInfo.GimmickType == BT_FALLROCK)
	{
		m_pBaseGimmick->Render(mapScrollY, mapScrollX, mapReverseState);
	}
}

