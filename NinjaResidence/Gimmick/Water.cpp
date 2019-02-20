/**
* @file Water.cpp
* @brief Waterクラス
* @author Toshiya Matsuoka
*/
#include "Water.h"
#include <string>


Water::Water(BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseGimmick(Gimmick, pDirectX,pSoundOperater)
{
	m_pDirectX = pDirectX;
}

Water::~Water()
{

}

void Water::Activate()
{
	//水の動き
	m_isActive = true;
}

void Water::Update()
{
	if (!m_isActive) return;
	if (m_QuantityOfMovement > -(CELL_SIZE*8.f)) {
		m_QuantityOfMovement -= 5.f;
		if (SoundLib::Playing != m_pSoundOperater->GetStatus("DRAINAGE")) {
			m_pSoundOperater->Start("DRAINAGE", false);
		}
		return;
	}
	if (SoundLib::Playing == m_pSoundOperater->GetStatus("DRAINAGE")) {
		m_pSoundOperater->Stop("DRAINAGE");
	}
}

void Water::Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse)
{
	if (MapDataReverse != m_GimmickInfo.MapDataState)
	{
		return;
	}

	m_GimmickPosX = m_GimmickInfo.PositionX;
	m_GimmickPosY = m_GimmickInfo.PositionY;
	float ScrollY = static_cast<float>(MapScrollY);
	float ScrollX = static_cast<float>(MapScrollX);

	if (m_isFirstTime == false)
	{
		m_WorldPosLeft = (CELL_SIZE * m_GimmickPosX);
		m_WorldPosTop = (CELL_SIZE * m_GimmickPosY);
		m_WorldPosRight = (CELL_SIZE * (m_GimmickPosX + 1));
		m_WorldPosBottom = (CELL_SIZE * (m_GimmickPosY + 1));
		m_isFirstTime = true;
	}
	m_TopPosition = m_WorldPosTop + ScrollY + m_QuantityOfMovement;
	VertexSetUp::SetVertex(m_GimmickVertex, m_TopPosition, m_WorldPosLeft + ScrollX, CELL_SIZE, CELL_SIZE );

	VertexSetUp::SetVertexUV(m_GimmickVertex, BLOCK_INTEGRATION_WIDTH* 2.f, BLOCK_INTEGRATION_HEIGHT, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);

	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_GimmickVertex);
	//VertexSetUp::SetVertex(m_GimmickVertex, m_WorldPosTop + ScrollY + m_QuantityOfMovement, m_WorldPosLeft + ScrollX, CELL_SIZE, CELL_SIZE);

	VertexSetUp::SetVertexUV(m_GimmickVertex, BLOCK_INTEGRATION_WIDTH* 3.f, BLOCK_INTEGRATION_HEIGHT, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);

	m_GimmickVertex[0].y = m_GimmickVertex[2].y;
	m_GimmickVertex[1].y = m_GimmickVertex[3].y;
	m_GimmickVertex[2].y = m_WorldPosBottom + ScrollY;
	m_GimmickVertex[3].y = m_WorldPosBottom + ScrollY;
	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_GimmickVertex);

}

float Water::GetGimmickPosition(bool isAxisX, MapDataState MapDataReverse)
{
	if (MapDataReverse != m_GimmickInfo.MapDataState)
	{
		return 0;
	}

	if (isAxisX) {
		return m_GimmickVertex[0].x;
	}
	return m_TopPosition;
}
