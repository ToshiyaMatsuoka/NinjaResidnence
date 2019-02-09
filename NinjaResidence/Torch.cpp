/**
* @file Torch.cpp
* @brief Torchクラス
* @author Kojiro Kawahara
*/
#include "Torch.h"
#include "DirectX.h"
#include "TargetandGimmickType.h"

Torch::Torch(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(Target, Gimmick, pDirectX,pSoundOperater)
{

}

Torch::~Torch()
{

}


void Torch::ActivateTarget()
{
	m_pBaseGimmick->Activate();
	m_isActive = true;
	m_pSoundOperater->Start("FIRE", true);

}

void Torch::Update()
{
	m_pBaseGimmick->Update();
	if (!m_isActive) {
		if (SoundLib::Playing == m_pSoundOperater->GetStatus("FIRE")) {
			m_pSoundOperater->Stop("FIRE");
		}
	}
}


void Torch::Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse)
{
	if (m_GimmickInfo.GimmickType == BT_PARTITIONBOARD)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
	if (m_GimmickInfo.GimmickType == BT_FALLROCK)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
	if (m_GimmickInfo.GimmickType == BT_WATER)
	{
		m_pBaseGimmick->Render(MapScrollY, MapScrollX, MapDataReverse);
	}
	if (MapDataReverse != m_TargetInfo.MapDataState)
	{
		return;
	}

	float ScrollY = static_cast<float>(MapScrollY);
	float ScrollX = static_cast<float>(MapScrollX);

	m_TargetPosX = static_cast<float>(m_TargetInfo.PositionX);
	m_TargetPosY = static_cast<float>(m_TargetInfo.PositionY);

	m_TargetVertex[0].x = (CELL_SIZE * m_TargetPosX) + ScrollX;
	m_TargetVertex[0].y = (CELL_SIZE * m_TargetPosY) + ScrollY;
	m_TargetVertex[1].x = (CELL_SIZE * m_TargetPosX) + ScrollX + CELL_SIZE;
	m_TargetVertex[1].y = (CELL_SIZE * m_TargetPosY) + ScrollY;
	m_TargetVertex[2].x = (CELL_SIZE * m_TargetPosX) + ScrollX + CELL_SIZE;
	m_TargetVertex[2].y = (CELL_SIZE * m_TargetPosY) + ScrollY + CELL_SIZE * 3.f;
	m_TargetVertex[3].x = (CELL_SIZE * m_TargetPosX) + ScrollX;
	m_TargetVertex[3].y = (CELL_SIZE * m_TargetPosY) + ScrollY + CELL_SIZE * 3.f;

	m_TargetVertex[0].tu = BLOCK_INTEGRATION_WIDTH * 3.f;
	m_TargetVertex[1].tu = BLOCK_INTEGRATION_WIDTH * 4.f;
	m_TargetVertex[2].tu = BLOCK_INTEGRATION_WIDTH * 4.f;
	m_TargetVertex[3].tu = BLOCK_INTEGRATION_WIDTH * 3.f;

	m_TargetVertex[0].tv = BLOCK_INTEGRATION_HEIGHT * 3.f;
	m_TargetVertex[1].tv = BLOCK_INTEGRATION_HEIGHT * 3.f;
	m_TargetVertex[2].tv = BLOCK_INTEGRATION_HEIGHT * 5.f;
	m_TargetVertex[3].tv = BLOCK_INTEGRATION_HEIGHT * 5.f;

	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
	if (m_isActive) {
		float HarfCellSize = CELL_SIZE * 0.5;
		m_TargetVertex[0].x = (CELL_SIZE * m_TargetPosX) + ScrollX;
		m_TargetVertex[0].y = (CELL_SIZE * m_TargetPosY) + ScrollY - HarfCellSize;
		m_TargetVertex[1].x = (CELL_SIZE * m_TargetPosX) + ScrollX + CELL_SIZE;
		m_TargetVertex[1].y = (CELL_SIZE * m_TargetPosY) + ScrollY - HarfCellSize;
		m_TargetVertex[2].x = (CELL_SIZE * m_TargetPosX) + ScrollX + CELL_SIZE;
		m_TargetVertex[2].y = (CELL_SIZE * m_TargetPosY) + ScrollY + HarfCellSize;
		m_TargetVertex[3].x = (CELL_SIZE * m_TargetPosX) + ScrollX;
		m_TargetVertex[3].y = (CELL_SIZE * m_TargetPosY) + ScrollY + HarfCellSize;

		m_TargetVertex[0].tu = BLOCK_INTEGRATION_WIDTH * 4.f;
		m_TargetVertex[1].tu = BLOCK_INTEGRATION_WIDTH * 5.f;
		m_TargetVertex[2].tu = BLOCK_INTEGRATION_WIDTH * 5.f;
		m_TargetVertex[3].tu = BLOCK_INTEGRATION_WIDTH * 4.f;

		m_TargetVertex[0].tv = BLOCK_INTEGRATION_HEIGHT * 5.f;
		m_TargetVertex[1].tv = BLOCK_INTEGRATION_HEIGHT * 5.f;
		m_TargetVertex[2].tv = BLOCK_INTEGRATION_HEIGHT * 6.f;
		m_TargetVertex[3].tv = BLOCK_INTEGRATION_HEIGHT * 6.f;
		for (int i = 0; i < 4; ++i) {
			m_TargetVertex[i].color = 0xCCFFFFFF;
		}
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
	}

}
