/**
* @file Rope.cpp
* @brief Ropeクラス
* @author Toshiya Matsuoka
*/
#include "Rope.h"



Rope::Rope(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(target, gimmick, pDirectX,pSoundOperater)
{
}


Rope::~Rope()
{
}

void Rope::ActivateTarget()
{
	m_pBaseGimmick->Activate();
	m_pSoundOperater->Start("CUT_OFF", false);

	m_isActive = true;
}

void Rope::Update()
{
	m_pBaseGimmick->Update();
	if (m_isActive) {
		m_Movement += 1.5f;
		m_CellSize = CELL_SIZE *0.5f;
	}
}


void Rope::Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState)
{
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

	if (mapReverseState != m_TargetInfo.MapDataState)
	{
		return;
	}
	float scrollY = static_cast<float>(mapScrollY);
	float scrollX = static_cast<float>(mapScrollX);

	m_TargetPosX = static_cast<float>(m_TargetInfo.PositionX);
	m_TargetPosY = static_cast<float>(m_TargetInfo.PositionY);
	
	VertexSetUp::SetVertexUV(m_TargetVertex, 0.f, BLOCK_INTEGRATION_WIDTH * 6.f, 0.5f);

	
	m_TargetVertex[0].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f;
	m_TargetVertex[0].y = (CELL_SIZE * (m_TargetPosY - 1)) + scrollY;
	m_TargetVertex[1].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f + CELL_SIZE;
	m_TargetVertex[1].y = (CELL_SIZE * (m_TargetPosY - 1)) + scrollY;
	m_TargetVertex[2].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f + CELL_SIZE;
	m_TargetVertex[2].y = (CELL_SIZE * m_TargetPosY) + scrollY + m_CellSize;
	m_TargetVertex[3].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f;
	m_TargetVertex[3].y = (CELL_SIZE * m_TargetPosY) + scrollY + m_CellSize;


	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_B_TEX", m_TargetVertex);
	if (m_isActive) {
		CUSTOMVERTEX divededRope[4];
		divededRope[0].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f;
		divededRope[0].y = (CELL_SIZE * (m_TargetPosY - 1)) + scrollY + m_Movement;
		divededRope[1].x = (CELL_SIZE * m_TargetPosX) + scrollX + CELL_SIZE + 5.f;
		divededRope[1].y = (CELL_SIZE * (m_TargetPosY - 1)) + scrollY + m_Movement;
		divededRope[2].x = (CELL_SIZE * m_TargetPosX) + scrollX + CELL_SIZE + 5.f;
		divededRope[2].y = (CELL_SIZE * m_TargetPosY) + scrollY + m_CellSize + m_Movement;
		divededRope[3].x = (CELL_SIZE * m_TargetPosX) + scrollX + 5.f;
		divededRope[3].y = (CELL_SIZE * m_TargetPosY) + scrollY + m_CellSize + m_Movement;

		VertexSetUp::SetVertexUV(divededRope, 0.f, BLOCK_INTEGRATION_WIDTH * 6.f, 0.5f);

		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_B_TEX", divededRope);
	}
}

