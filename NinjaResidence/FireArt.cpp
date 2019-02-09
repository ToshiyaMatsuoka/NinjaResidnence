/**
* @file FireArt.cpp
* @brief FireArtクラス
* @author Toshiya Matsuoka
*/
#include "FireArt.h"


using namespace PlayerAnimation;

FireArt::FireArt(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara) :SkillBase(pDirectX, pSoundOperater, MapChip,GameChara)
{
	m_Central = { 500,0,80,80 };
	m_row = m_pMapChip->GetRow();
	m_colunm = m_pMapChip->GetColunm();
	m_SkillType = FIRE_ART;
}



FireArt::~FireArt()
{

}



void FireArt::KeyOperation(KeyDirection vec)
{
	//Key操作での処理
	switch (vec)
	{
	case FIRE:
		m_isActive = PermitActive();
		m_pSoundOperater->Start("BURNING", false);
		break;
	case END_ART:
		InitPosition();
		if (SoundLib::Playing == m_pSoundOperater->GetStatus("BURNING")) {
			m_pSoundOperater->Stop("BURNING");
		}
		break;
	}
}


bool FireArt::PermitActive() {
	if (!m_isActive) {
		m_Direction = static_cast<float>(m_pGameChara->GetFacing());
		m_Central.x = m_pGameChara->GetPositionX() + (m_Direction * m_Central.scale_x);
		m_Central.y = m_pGameChara->GetPositionY();
		m_isChoseDeg = false;
		return true;
	}
	if (m_isActive) {
		return true;
	}
	//else return false;


	return false;
}

void FireArt::InitPosition() {
	m_isActive = false;
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * (m_Central.scale_x + 5.f);
	m_Central.y = m_pGameChara->GetPositionY();
	m_Animation = 0.f;
}

bool FireArt::Update()
{

	if (!m_isActive) {
		return true;
	}
	static int AnimeCount = 0;
	++AnimeCount;
	if (AnimeCount > 10) {
		m_Animation += 1.f;
		AnimeCount = 0;
	}
	if (m_Direction == FACING_RIGHT) {
		m_DirectionBias = ZERO;
	}
	else m_DirectionBias = ONE;
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scale_x;
	m_Central.y = m_pGameChara->GetPositionY()-10.f;
	m_MapPositionX = static_cast<int>((m_Central.x - m_MapScrollX) / CELL_SIZE);
	m_MapPositionY = static_cast<int>((m_Central.y - m_MapScrollY) / CELL_SIZE);
	if (m_Central.x < 0 || m_Central.x > DISPLAY_WIDTH) {
		//InitPosition();
	}
	if (m_MapPositionY == 0 || m_Central.y < 0 || m_Central.y > DISPLAY_HEIGHT) {
		//InitPosition();
	}
	int buf = 0;
	for (int i = -1; i <7; ++i) {
		int MapPosX = m_MapPositionX + i * static_cast<int>(m_Direction);
		if (MapPosX < 0) {
			MapPosX = m_MapPositionX;
		}
		if (MapPosX > m_row-1) {
			MapPosX = m_row - 1;
		}

		if (m_pMapChip->GetMapChipData(m_MapPositionY, MapPosX) > 100)
		{
			m_pMapChip->Activate(MapPosX, m_MapPositionY);
		}
		if (CollisionRope()) {
			m_pMapChip->Activate(m_ropeX, m_ropeY);
		}

	}
	return true;
}


void FireArt::Render()
{
	static int rad = 0;
	if (!m_isActive) {
		return;
	}
	if (m_isActive) {
		CUSTOMVERTEX Vertex[4];
		CreateSquareVertex(Vertex, m_Central, 0xFFFFFFFF, m_DirectionBias * m_CharTu, m_CharTv * 8, m_CharTu * m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", Vertex);
		m_Central.x += 85 * m_Direction;
		CreateSquareVertex(Vertex, m_Central, 0xFFFFFFFF,(m_DirectionBias + 1)* m_CharTu, m_CharTv * 8, m_CharTu*m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", Vertex);
		m_Central.x += 85 * m_Direction;
		CreateSquareVertex(Vertex, m_Central, 0xFFFFFFFF, (m_DirectionBias +2 ) * m_CharTu, m_CharTv * 8, m_CharTu*m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", Vertex);

	}
}

void FireArt::Reverse(Object* MapChip) {
	m_pMapChip = MapChip;
	m_row = m_pMapChip->GetRow();
	m_colunm = m_pMapChip->GetColunm();
}
