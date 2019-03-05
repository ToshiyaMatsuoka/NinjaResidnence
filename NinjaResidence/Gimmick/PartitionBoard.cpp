/**
* @file PartitionBoard.cpp
* @brief PartitionBoardクラス
* @author Kojiro Kawahara
*/
#include "PartitionBoard.h"
#include "../Object/MapChip.h"
#include <string>

PartitionBoard::PartitionBoard(BlockInfo gimmick, DirectX* pDirectX, MapChip* pMapChip, SoundOperater* pSoundOperater) :BaseGimmick(gimmick, pDirectX,pSoundOperater)
{
	m_pDirectX = pDirectX;
	m_pMapChip = pMapChip;
	m_pMapChip->MapDataVectorHitSet(m_GimmickInfo.PositionY, m_GimmickInfo.PositionX, 13, 3);
}

PartitionBoard::~PartitionBoard()
{

}


void PartitionBoard::Activate()
{
	m_isActive = true;
	m_pSoundOperater->Start("DOOR",false);
}


void PartitionBoard::Update()
{
	if (!m_isActive) return;
	//仕切り板の動き
	while (m_WorldPosBottom >= m_WorldPosTop)
	{
		m_WorldPosBottom -= 5.f;
		break;
	}
	if (m_WorldPosBottom <= m_WorldPosTop)
	{
		m_pSoundOperater->Stop("DOOR");
		m_pMapChip->MapDataVectorZeroSet(m_GimmickInfo.PositionY, m_GimmickInfo.PositionX, 13, 3);
		m_isActive = false;
	}
}


void PartitionBoard::Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState)
{
	if (mapReverseState != m_GimmickInfo.MapDataState)
	{
		return;
	}
	m_GimmickPosX = m_GimmickInfo.PositionX;
	m_GimmickPosY = m_GimmickInfo.PositionY;
	if (m_isFirstTime == false)
	{
		m_WorldPosLeft = (CELL_SIZE * m_GimmickPosX);
		m_WorldPosTop = (CELL_SIZE * m_GimmickPosY);
		m_WorldPosRight = (CELL_SIZE * (m_GimmickPosX + 3));
		m_WorldPosBottom = (CELL_SIZE * (m_GimmickPosY + 15));
		m_isFirstTime = true;
	}
	float BoradHeight = m_WorldPosBottom - m_WorldPosTop;
	VertexSetUp::SetVertex(m_GimmickVertex, m_WorldPosTop + mapScrollY, m_WorldPosLeft + mapScrollX, BoradHeight, CELL_SIZE * 3.f);

	VertexSetUp::SetVertexUV(m_GimmickVertex, BLOCK_INTEGRATION_WIDTH* 3.f,0.f, BLOCK_INTEGRATION_WIDTH * 2.f, BLOCK_INTEGRATION_HEIGHT * 5.f);


	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_B_TEX", m_GimmickVertex);

}
