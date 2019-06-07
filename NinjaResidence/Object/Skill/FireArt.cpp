/**
* @file FireArt.cpp
* @brief FireArtクラス
* @author Toshiya Matsuoka
*/
#include "FireArt.h"


using namespace PlayerAnimation;

FireArt::FireArt(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* mapChip, GameChara* gameChara) :SkillBase(pDirectX, pSoundOperater, mapChip,gameChara)
{
	m_Central = { 500,0,CELL_SIZE*2.f,CELL_SIZE*2.f };
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();
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
		m_Central.x = m_pGameChara->GetPositionX() + (m_Direction * m_Central.scaleX);
		m_Central.y = m_pGameChara->GetPositionY();
		m_isChoseDeg = false;
		return true;
	}
	if (m_isActive) {
		return true;
	}

	return false;
}

void FireArt::InitPosition() {
	m_isActive = false;
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * (m_Central.scaleX + 5.f);
	m_Central.y = m_pGameChara->GetPositionY();
	m_Animation = 0.f;
}

bool FireArt::Update()
{

	if (!m_isActive) {
		return true;
	}
	static int animeCount = 0;
	++animeCount;
	if (animeCount > 10) {
		m_Animation += 1.f;
		animeCount = 0;
	}
	if (m_Direction == FACING_RIGHT) {
		m_DirectionBias = ZERO;
	}
	else m_DirectionBias = ONE;
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scaleX;
	m_Central.y = m_pGameChara->GetPositionY()-10.f;
	m_MapPositionX = static_cast<int>((m_Central.x - MapChip::Scroll().X) / CELL_SIZE);
	m_MapPositionY = static_cast<int>((m_Central.y - MapChip::Scroll().Y) / CELL_SIZE);
	//if (m_Central.x < 0 || m_Central.x > DISPLAY_WIDTH) {
	//	InitPosition();
	//}
	//if (m_MapPositionY == 0 || m_Central.y < 0 || m_Central.y > DISPLAY_HEIGHT) {
	//	InitPosition();
	//}
	int buf = 0;
	for (int i = -1; i <7; ++i) {
		int mapPosX = m_MapPositionX + i * static_cast<int>(m_Direction);
		if (mapPosX < 0) {
			mapPosX = m_MapPositionX;
		}
		if (mapPosX > m_MapSizeX-1) {
			mapPosX = m_MapSizeX - 1;
		}

		if (m_pMapChip->GetMapChipData(m_MapPositionY, mapPosX) > 100)
		{
			m_pMapChip->Activate(mapPosX, m_MapPositionY);
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
		CUSTOMVERTEX vertex[4];
		CreateSquareVertex(vertex, m_Central, DEFFALT_COLOR, m_DirectionBias * m_CharTu, m_CharTv * 8, m_CharTu * m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", vertex);
		m_Central.x += CELL_SIZE * 2.125f * m_Direction;
		CreateSquareVertex(vertex, m_Central, DEFFALT_COLOR,(m_DirectionBias + 1)* m_CharTu, m_CharTv * 8, m_CharTu*m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", vertex);
		m_Central.x += CELL_SIZE * 2.125f * m_Direction;
		CreateSquareVertex(vertex, m_Central, DEFFALT_COLOR, (m_DirectionBias +2 ) * m_CharTu, m_CharTv * 8, m_CharTu*m_Direction, m_CharTv);
		m_pDirectX->DrawTexture("CHARA_TEX", vertex);

	}
}

void FireArt::Reverse(MapChip* mapChip) {
	m_pMapChip = mapChip;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();
}
