/**
* @file FallRock.cpp
* @brief FallRockクラス
* @author Kojiro Kawahara
*/
#include "FallRock.h"
#include "../Object/MapChip.h"
#include <string>


FallRock::FallRock(BlockInfo Gimmick, DirectX* pDirectX, MapChip* pMapChip, SoundOperater* pSoundOperater) :BaseGimmick(Gimmick, pDirectX,pSoundOperater)
{
	m_pDirectX = pDirectX;
	m_pMapChip = pMapChip;
	m_GimmickPosX = static_cast<int>(CELL_SIZE) * m_GimmickInfo.PositionX;
	m_GimmickPosY = static_cast<int>(CELL_SIZE) * m_GimmickInfo.PositionY;
	m_pMapChip->MapDataVectorHitSet(m_GimmickInfo.PositionY, m_GimmickInfo.PositionX, 2, 2);
}

FallRock::~FallRock()
{

}

void FallRock::Activate()
{
	m_isActive = true;
}

void FallRock::Update()
{
	if (!m_isActive) return;

	//落ちる岩の動き
	int CheckNumber = m_pMapChip->GimmickMapDataCheck((m_GimmickPosY + static_cast<int>(CELL_SIZE) * 2 + static_cast<int>(m_QuantityOfMovement)) / static_cast<int>(CELL_SIZE), m_GimmickPosX / static_cast<int>(CELL_SIZE));
	if (CheckNumber == 0)
	{
		m_pMapChip->MapDataVectorZeroSet((m_GimmickPosY + static_cast<int>(m_QuantityOfMovement)) / static_cast<int>(CELL_SIZE), m_GimmickInfo.PositionX, 2, 2);
		m_QuantityOfMovement += 5.f;
		m_pMapChip->MapDataVectorHitSet((m_GimmickPosY + static_cast<int>(m_QuantityOfMovement)) / static_cast<int>(CELL_SIZE), m_GimmickInfo.PositionX, 2, 2);
	}
}
void FallRock::Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse)
{
	float ScrollY = static_cast<float>(MapScrollY);
	float ScrollX = static_cast<float>(MapScrollX);

	if (MapDataReverse != m_GimmickInfo.MapDataState)
	{
		return;
	}
	VertexSetUp::SetVertex(m_GimmickVertex, m_GimmickPosY + ScrollY + m_QuantityOfMovement, m_GimmickPosX + ScrollX, CELL_SIZE * 2.f, CELL_SIZE * 2.f);

	VertexSetUp::SetVertexUV(m_GimmickVertex, BLOCK_INTEGRATION_WIDTH * 4.f, BLOCK_INTEGRATION_HEIGHT * 3.f, BLOCK_INTEGRATION_WIDTH * 2.f, BLOCK_INTEGRATION_HEIGHT * 2.f);

	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_GimmickVertex);
}
