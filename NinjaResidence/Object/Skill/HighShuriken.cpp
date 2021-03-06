﻿/**
* @file HighShuriken.cpp
* @brief 高等手裏剣クラス
* @author Toshiya Matsuoka
*/
#include "HighShuriken.h"
#include "Device/XinputDevice.h"



using namespace PlayerAnimation;

HighShuriken::HighShuriken(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* mapChip, GameChara* gameChara, XinputDevice* pXinputDevice) :SkillBase(pDirectX, pSoundOperater, mapChip, gameChara)
{
	m_Central = { 500,0,CELL_SIZE*0.5f,CELL_SIZE*0.5f };
	m_pMapChip = mapChip;
	m_pGameChara = gameChara;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();
	m_pXinputDevice = pXinputDevice;
	m_SkillType = HIGH_SHURIKEN_ART;
}



HighShuriken::~HighShuriken()
{

}



void HighShuriken::KeyOperation(KeyDirection vec)
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

	case BIT_X_UP:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = InputRightStickDeg();
		break;
	case BIT_X_DOWN:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = InputRightStickDeg();
		break;
	case BIT_X_LEFT:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = InputRightStickDeg();
		break;
	case BIT_X_RIGHT:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = InputRightStickDeg();
		break;
	case BIT_D_UP:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = 90.f;
		break;
	case BIT_D_DOWN:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = 270.f;
		break;
	case BIT_D_LEFT:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = 180.f;
		break;
	case BIT_D_RIGHT:
		m_Direction = FACING_RIGHT;
		m_DirectionDeg = 0.f;
		break;
	case END_ART:
		InitPosition();
		break;
	}
}

bool HighShuriken::PermitActive() {
	if (!m_isChoseDeg && !m_isActive) {
		m_isChoseDeg = true;
		m_DirectionDeg = 0;
		m_Direction = static_cast<float>(m_pGameChara->GetFacing());
		if (m_Direction == FACING_RIGHT) {
			m_DirectionBias = ZERO;
		}
		else m_DirectionBias = ONE;
		InitPosition();
		return false;
	}
	if (m_isChoseDeg && !m_isActive) {
		m_Central.x = m_pGameChara->GetPositionX() + (m_Direction * m_Central.scaleX);
		m_Central.y = m_pGameChara->GetPositionY();
		m_PrevScroll = MapChip::Scroll();
		isOperation = false;
		m_isChoseDeg = false;
		return true;
	}
	if (m_isActive) {
		return true;
	}
	return false;
}


void HighShuriken::InitPosition() {
	m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scaleX;
	m_Central.y = m_pGameChara->GetPositionY();
}

bool HighShuriken::Update()
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

	if (m_Central.x < 0 || m_Central.x > DISPLAY_WIDTH || m_MapPositionX >= m_MapSizeX - 1) {
		DeActive();
	}
	if (m_MapPositionY == 0 || m_Central.y < 0 || m_Central.y > DISPLAY_HEIGHT || m_MapPositionY >= m_MapSizeY - 1) {
		DeActive();
	}
	int buf = m_pMapChip->GetMapData(m_MapPositionY, m_MapPositionX);
	if (buf > NOMAL_BLOCK_MAX  && buf<200)
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


void HighShuriken::Render()
{
#ifdef _DEBUG
	RECT testName = { 0, 100, 1250, 720 };
	char TestName[ARRAY_LONG];
	sprintf_s(TestName, ARRAY_LONG, "\nDirectionDeg::%.2f", m_DirectionDeg);
	m_pDirectX->DrawWord(testName, TestName, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
#endif

	static int rad = 0;
	if (!m_isChoseDeg && !m_isActive) {
		return;
	}
	if (m_isChoseDeg) {
		CUSTOMVERTEX DirectionArrowVertex[4];
		RevolveZ(DirectionArrowVertex, DegToRad(m_DirectionDeg), m_DirectionArrow, m_Central.x , m_Central.y, DEFFALT_COLOR, m_DirectionBias*(BLOCK_INTEGRATION_WIDTH*1.5f), BLOCK_INTEGRATION_HEIGHT * 9.65f, (BLOCK_INTEGRATION_WIDTH*1.5f)*m_Direction, BLOCK_INTEGRATION_HEIGHT*0.5f);
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

void HighShuriken::Reverse(MapChip* mapChip) {
	m_pMapChip = mapChip;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();
	InitPosition();
}

float HighShuriken::InputRightStickDeg() {
	int XaxisValue = m_pXinputDevice->GetAnalogRValue(ANALOG_X);
	int YaxisValue = m_pXinputDevice->GetAnalogRValue(ANALOG_Y);
	if (XaxisValue == 0 && YaxisValue == 0) { 
		return 0.f;
	}
	float rad = static_cast<float>(std::atan2(YaxisValue, XaxisValue));
	static float deg = 0;
	return deg = RadToDeg(rad);
}
