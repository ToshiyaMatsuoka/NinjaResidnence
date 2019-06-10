/**
* @file Shuriken.cpp
* @brief 手裏剣クラス
* @author Toshiya Matsuoka
*/
#include "Shuriken.h"
using namespace PlayerAnimation;

Shuriken::Shuriken(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* MapChip, GameChara* GameChara) :SkillBase(pDirectX, pSoundOperater,MapChip,GameChara)
{
	m_Central = { 500,0,CELL_SIZE*0.5f,CELL_SIZE*0.5f };
	m_pMapChip = MapChip;
	m_pGameChara = GameChara;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();

	m_SkillType = SHURIKEN;
}



Shuriken::~Shuriken()
{

}



void Shuriken::KeyOperation(KeyDirection vec)
{
	//Key操作での処理
	switch (vec)
	{
	case THROW:
		m_isActive = PermitActive();
		break;
	case UP:
		if (m_isActive || !m_isChoseDeg) {
			return;
		}
		m_DirectionDeg += m_Direction;
		if (m_DirectionDeg > MAX_INCIDENCE_ANGLE) {
			m_DirectionDeg = MAX_INCIDENCE_ANGLE;
		}
		if (m_DirectionDeg < -MAX_INCIDENCE_ANGLE) {
			m_DirectionDeg = -MAX_INCIDENCE_ANGLE;
		}
		break;
	case DOWN:
		if (m_isActive || !m_isChoseDeg) {
			return;
		}
		m_DirectionDeg -= m_Direction;
		if (m_DirectionDeg > MAX_INCIDENCE_ANGLE) {
			m_DirectionDeg = MAX_INCIDENCE_ANGLE;
		}
		if (m_DirectionDeg < -MAX_INCIDENCE_ANGLE) {
			m_DirectionDeg = -MAX_INCIDENCE_ANGLE;
		}
		break;
	case END_ART:
		m_DirectionDeg = 0;

		break;
	}
}


bool Shuriken::PermitActive() {
	if (!m_isChoseDeg && !m_isActive) {
		m_isChoseDeg = true;
		m_Direction = static_cast<float>(m_pGameChara->GetFacing());
		if (m_Direction == FACING_RIGHT) {
			m_DirectionBias = ZERO;
		}
		else m_DirectionBias = ONE;
		InitPosition();
		return false;
	}
	if (m_isChoseDeg && !m_isActive) {
		m_Central.x = m_pGameChara->GetPositionX() +( m_Direction * m_Central.scaleX);
		m_Central.y = m_pGameChara->GetPositionY();
		m_PrevScroll = MapChip::Scroll();

		m_isChoseDeg = false;
		return true;
	}
	if (m_isActive) {
		return true;
	}
	//else return false;


	return false;
}
void Shuriken::InitPosition() {
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scaleX;
	m_Central.y = m_pGameChara->GetPositionY();
}

bool Shuriken::Update()
{
	if (m_isChoseDeg) {
		InitPosition();

		m_DirectionArrow.x = m_pGameChara->GetPositionX() + m_Direction * CELL_SIZE * 2;
		m_DirectionArrow.y = m_pGameChara->GetPositionY();
	}

	if (!m_isActive) {
		return true;
	}
	m_PrevScroll.X -= MapChip::Scroll().X;
	m_PrevScroll.Y -= MapChip::Scroll().Y;
	m_Central.x += (MoveSpeed * m_Direction) * std::cos(DegToRad(m_DirectionDeg)) - m_PrevScroll.X;
	m_Central.y -= (MoveSpeed * m_Direction) * std::sin(DegToRad(m_DirectionDeg)) + m_PrevScroll.Y;
	m_MapPositionX = static_cast<int>((m_Central.x - MapChip::Scroll().X) / CELL_SIZE);
	m_MapPositionY = static_cast<int>((m_Central.y - MapChip::Scroll().Y) / CELL_SIZE);
	//終了判定
	if (m_Central.x < 0 || m_Central.x > DISPLAY_WIDTH || m_MapPositionX >= m_MapSizeX-1) {
		DeActive();
	}
	if (m_MapPositionY == 0 || m_Central.y < 0 || m_Central.y > DISPLAY_HEIGHT || m_MapPositionY >= m_MapSizeY-1) {
		DeActive();
	}
	int buf = m_pMapChip->GetMapData(m_MapPositionY, m_MapPositionX);
	if (buf > NOMAL_BLOCK_MAX && buf<200)
	{
		m_pMapChip->Activate(m_MapPositionX, m_MapPositionY);
		DeActive();
	}
	else if (buf < NOMAL_BLOCK_MAX && buf > MapBlock::NONE && buf != MapBlock::START_ZONE&&buf != MapBlock::DESCRIPTION_BOARD)
	{
		m_pSoundOperater->Start("CLAWSHOT", false);
		DeActive();
	}

	if (CollisionRope()) {
		m_pMapChip->Activate(m_ropeX, m_ropeY);
		DeActive();
	}
	m_PrevScroll = MapChip::Scroll();

	return true;
}


void Shuriken::Render()
{
	static int rad = 0;
	if (!m_isChoseDeg&&!m_isActive) {
		return;
	}
	if (m_isChoseDeg) {
		CUSTOMVERTEX DirectionArrowVertex[4];
		RevolveZ(DirectionArrowVertex, DegToRad(m_DirectionDeg), m_DirectionArrow, m_Central.x, m_Central.y, DEFFALT_COLOR, m_DirectionBias*(BLOCK_INTEGRATION_WIDTH*1.5f), BLOCK_INTEGRATION_HEIGHT * 9.65f, (BLOCK_INTEGRATION_WIDTH*1.5f)*m_Direction, BLOCK_INTEGRATION_HEIGHT*0.5f);
		TextureRender("BLOCK_INTEGRATION_A_TEX", DirectionArrowVertex);
		rad = 0;
		return;
	}
	if (m_isActive) {
		CUSTOMVERTEX ShurikenVertex[4];
		static float rad = 0.f;
		rad += 10.f;
		RevolveZ(ShurikenVertex, static_cast<float>(rad), m_Central, DEFFALT_COLOR, 0.f, BLOCK_INTEGRATION_HEIGHT * 2.965f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", ShurikenVertex);
	}
}

void Shuriken::Reverse(MapChip* MapChip) {
	m_pMapChip = MapChip;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();
	InitPosition();
}
