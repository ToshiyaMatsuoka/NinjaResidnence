/**
* @file Target.cpp
* @brief Targetクラス
* @author Kojiro Kawahara
*/
#include "Target.h"
#include "DirectX.h"
#include "TargetandGimmickType.h"

float DegToRad(float deg) {
	return deg * (D3DX_PI / 180);
}

Target::Target(BlockInfo Target, BlockInfo Gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(Target, Gimmick, pDirectX,pSoundOperater)
{

}

Target::~Target()
{
	delete m_pBaseGimmick;
	m_pBaseGimmick = NULL;
}


void Target::ActivateTarget()
{
	m_pBaseGimmick->Activate();
	m_pSoundOperater->Start("TARGET_ACTIVE");
	m_isActive = true;

}

void Target::Update()
{
	m_pBaseGimmick->Update();
}


void Target::Render(int MapScrollY, int MapScrollX, MapDataState MapDataReverse)
{
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

	if (MapDataReverse != m_TargetInfo.MapDataState)
	{
		return;
	}
	if (m_isActive) {
		m_MotionBias = 1;
	}
	else m_MotionBias = 0;
	if (m_TargetInfo.PairNumber > 30) {
		m_MotionBias += 2;
	}
	if (m_TargetInfo.PairNumber > 60) {
		m_MotionBias += 2;
	}
	m_TargetPosX = static_cast<float>(m_TargetInfo.PositionX);
	m_TargetPosY = static_cast<float>(m_TargetInfo.PositionY);
	if (MapDataReverse == m_TargetInfo.MapDataState)
	{
		m_TargetVertex[0].x = (CELL_SIZE * m_TargetPosX) + MapScrollX + 5;
		m_TargetVertex[0].y = (CELL_SIZE * (m_TargetPosY - 0.5f)) + MapScrollY;
		m_TargetVertex[1].x = (CELL_SIZE * m_TargetPosX) + MapScrollX + CELL_SIZE + 5;
		m_TargetVertex[1].y = (CELL_SIZE * (m_TargetPosY - 0.5f)) + MapScrollY;
		m_TargetVertex[2].x = (CELL_SIZE * m_TargetPosX) + MapScrollX + CELL_SIZE + 5;
		m_TargetVertex[2].y = (CELL_SIZE * (m_TargetPosY + 0.5f)) + MapScrollY + CELL_SIZE;
		m_TargetVertex[3].x = (CELL_SIZE * m_TargetPosX) + MapScrollX + 5;
		m_TargetVertex[3].y = (CELL_SIZE * (m_TargetPosY + 0.5f)) + MapScrollY + CELL_SIZE;

		//m_TargetVertex[0].tu = BLOCK_INTEGRATION_WIDTH * m_MotionBias;
		//m_TargetVertex[1].tu = BLOCK_INTEGRATION_WIDTH * (m_MotionBias + 1);
		//m_TargetVertex[2].tu = BLOCK_INTEGRATION_WIDTH * (m_MotionBias + 1);
		//m_TargetVertex[3].tu = BLOCK_INTEGRATION_WIDTH * m_MotionBias;

		//m_TargetVertex[0].tv = BLOCK_INTEGRATION_HEIGHT * 2;
		//m_TargetVertex[1].tv = BLOCK_INTEGRATION_HEIGHT * 2;
		//m_TargetVertex[2].tv = BLOCK_INTEGRATION_HEIGHT * 3;
		//m_TargetVertex[3].tv = BLOCK_INTEGRATION_HEIGHT * 3;
		float deg = 0;
		switch (m_TargetInfo.PairNumber % 10) {
			//上向き
		case 4:
		case 5:
		case 6:
			deg = 90.f;
			for (int i = 0; i < 4; ++i) {
				m_TargetVertex[i].y -= CELL_SIZE * 0.5f;
			}

			break;
			//右向き
		case 7:
		case 8:
			deg = 180.f;
			break;
			//下向き
		case 9:
		case 0:
			deg = 270.f;
			for (int i = 0; i < 4; ++i) {
				m_TargetVertex[i].y += CELL_SIZE * 0.5f;
			}
			break;
		}

		RevolveZ(m_TargetVertex, DegToRad(deg), 0xffffffff, BLOCK_INTEGRATION_WIDTH * m_MotionBias, BLOCK_INTEGRATION_HEIGHT * 2, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
	}
}

void Target::RevolveZ(CUSTOMVERTEX* Vertex, float Rad,DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float CharVertexX[4];
	float CharVertexY[4];
	float bufX = ((Vertex[1].x - Vertex[0].x) / 2.f) + Vertex[0].x;
	float bufY = ((Vertex[3].y - Vertex[0].y) / 2.f) + Vertex[0].y;

	for (int i = 0; i < 4; i++) {

		CharVertexX[i] = Vertex[i].x;
		CharVertexY[i] = Vertex[i].y;

		CharVertexX[i] -= bufX;
		CharVertexY[i] -= bufY;

		float KEEPER = CharVertexX[i];

		CharVertexX[i] = (CharVertexX[i] * cos(-Rad)) - (CharVertexY[i] * sin(-Rad));
		CharVertexY[i] = (CharVertexY[i] * cos(-Rad)) + (KEEPER * sin(-Rad));

		CharVertexX[i] += bufX;
		CharVertexY[i] += bufY;

	}

	Vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	Vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

