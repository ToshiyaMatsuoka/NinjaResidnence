/**
* @file Target.cpp
* @brief Targetクラス
* @author Kojiro Kawahara
*/
#include "Target.h"
#include "Device/DirectX.h"
#include "TargetandGimmickType.h"

float DegToRad(float deg) {
	return deg * (D3DX_PI / 180);
}

Target::Target(BlockInfo target, BlockInfo gimmick, DirectX* pDirectX, SoundOperater* pSoundOperater) :BaseTarget(target, gimmick, pDirectX,pSoundOperater)
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


void Target::Render(int mapScrollY, int mapScrollX, MapDataState mapReverseState)
{
	DWORD Color = 0xffffffff;
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
	if (m_isActive) {
		m_MotionBias = 1;
		Color = 0xFFCCCCCC;
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
	if (mapReverseState == m_TargetInfo.MapDataState)
	{
		VertexSetUp::SetVertex(m_TargetVertex, 
			(CELL_SIZE * (m_TargetPosY)) + mapScrollY - CELL_SIZE * 0.5f, 
			(CELL_SIZE * m_TargetPosX) + mapScrollX , 
			CELL_SIZE * 1.5f, 
			CELL_SIZE * 1.5f);
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

		RevolveZ(m_TargetVertex, DegToRad(deg), Color, BLOCK_INTEGRATION_WIDTH * m_MotionBias, BLOCK_INTEGRATION_HEIGHT * 2.0f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", m_TargetVertex);
	}
}

void Target::RevolveZ(CUSTOMVERTEX* vertex, float rad,DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float CharVertexX[4];
	float CharVertexY[4];
	float bufX = ((vertex[1].x - vertex[0].x) / 2.f) + vertex[0].x;
	float bufY = ((vertex[3].y - vertex[0].y) / 2.f) + vertex[0].y;

	for (int i = 0; i < 4; i++) {

		CharVertexX[i] = vertex[i].x;
		CharVertexY[i] = vertex[i].y;

		CharVertexX[i] -= bufX;
		CharVertexY[i] -= bufY;

		float KEEPER = CharVertexX[i];

		CharVertexX[i] = (CharVertexX[i] * cos(-rad)) - (CharVertexY[i] * sin(-rad));
		CharVertexY[i] = (CharVertexY[i] * cos(-rad)) + (KEEPER * sin(-rad));

		CharVertexX[i] += bufX;
		CharVertexY[i] += bufY;

	}

	vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

