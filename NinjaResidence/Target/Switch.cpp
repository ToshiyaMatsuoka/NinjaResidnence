/**
* @file Switch.cpp
* @brief Switchクラス
* @author Kojiro Kawahara
*/
#include "Switch.h"



Switch::Switch(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(Target, Gimmick, pDirectX,pSoundOperater)
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


void Switch::Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse)
{
	if (MapDataReverse == m_TargetInfo.MapDataState)
	{
		m_TargetPosX = static_cast<float>(m_TargetInfo.PositionX);
		m_TargetPosY = static_cast<float>(m_TargetInfo.PositionY);
		//TODO:的のオンオフ切り替え
		if (MapDataReverse == m_TargetInfo.MapDataState)
		{
			VertexSetUp::SetVertex(m_TargetVertex, (CELL_SIZE * (m_TargetPosY)) + MapScrollY, (CELL_SIZE * m_TargetPosX) + MapScrollX, CELL_SIZE, CELL_SIZE);

			VertexSetUp::SetVertexUV(m_TargetVertex, BLOCK_INTEGRATION_WIDTH * 4.f, 0.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);

			m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
		}
	}

	if (m_GimmickInfo.GimmickType == BT_WATER)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
	if (m_GimmickInfo.GimmickType == BT_PARTITIONBOARD)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
	if (m_GimmickInfo.GimmickType == BT_FALLROCK)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
}

