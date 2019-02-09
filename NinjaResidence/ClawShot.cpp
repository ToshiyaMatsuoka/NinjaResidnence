/**
* @file ClawShot.cpp
* @brief 鉤爪ロープクラス
* @author Toshiya Matsuoka
*/
#include "ClawShot.h"
using namespace PlayerAnimation;

ClawShot::ClawShot(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara) :SkillBase(pDirectX, pSoundOperater, MapChip, GameChara)
{
	m_Central = { 500,0,25,25 };
	m_row = m_pMapChip->GetRow();
	m_colunm = m_pMapChip->GetColunm();

	m_SkillType = CLAWSHOT;
}



ClawShot::~ClawShot()
{

}



void ClawShot::KeyOperation(KeyDirection vec)
{
	if (m_DirectionDeg < 0) {
		m_DirectionDeg *= -1;
	}
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
		m_DirectionDeg += 1.0f;
		if (m_DirectionDeg > 90) {
			m_DirectionDeg = 90;
		}
		if (m_DirectionDeg < 0) {
			m_DirectionDeg = 0;
		}
		break;
	case DOWN:
		if (m_isActive || !m_isChoseDeg) {
			return;
		}
		m_DirectionDeg -= 1.0f;
		if (m_DirectionDeg > 90) {
			m_DirectionDeg = 90;
		}
		if (m_DirectionDeg < 0) {
			m_DirectionDeg = 0;
		}
		break;
	case END_ART:
		InitPosition();
		break;
	}
	m_DirectionDeg *= m_Direction;
}


bool ClawShot::PermitActive() {
	if (!m_isChoseDeg && !m_isActive) {
		m_isChoseDeg = true;
		m_Direction = static_cast<float>(m_pGameChara->GetFacing());
		if (m_Direction == FACING_RIGHT) {
			m_DirectionBias = ZERO;
		}
		else m_DirectionBias = ONE;
		return false;
	}
	if (m_isChoseDeg && !m_isActive) {
		RopeBatteryPosX = m_Central.x = m_pGameChara->GetPositionX() + (m_Direction * m_Central.scale_x);
		RopeBatteryPosY = m_Central.y = m_pGameChara->GetPositionY();
		PrevMapScrollX = m_MapScrollX;
		PrevMapScrollY = m_MapScrollY;

		m_isChoseDeg = false;
		return true;
	}
	if (m_isActive) {
		return true;
	}
	return false;
}

//bool ClawShot::CollisionRope()
//{
//	CENTRAL_STATE RopeCentral = { 0 };
//	TranslateCentral_State(m_pMapChip->GetTargetPosition(BT_ROPE), &RopeCentral);
//
//	m_ropeX = static_cast<int>((RopeCentral.x - m_MapScrollX) / CELL_SIZE);
//	m_ropeY = static_cast<int>((RopeCentral.y - m_MapScrollY) / CELL_SIZE);
//
//	return ContactSpecifyObject(&RopeCentral);
//}
//
void ClawShot::InitPosition() {
	m_isActive = false;
	RopeBatteryPosX = m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scale_x;
	RopeBatteryPosY = m_Central.y = m_pGameChara->GetPositionY();
	m_DirectionDeg = 0;
}

bool ClawShot::Update()
{
	if (m_isChoseDeg) {
		m_DirectionArrow.x = m_pGameChara->GetPositionX() + m_Direction * CELL_SIZE * 2;
		m_DirectionArrow.y = m_pGameChara->GetPositionY();
		RopeBatteryPosX = m_Central.x = m_pGameChara->GetPositionX() + m_Direction * m_Central.scale_x;
		RopeBatteryPosY = m_Central.y = m_pGameChara->GetPositionY();
	}

	if (!m_isActive) {
		return true;
	}
	PrevMapScrollX -= m_MapScrollX;
	PrevMapScrollY -= m_MapScrollY;
	m_Central.x += (MoveSpeed * m_Direction) * std::cos(DegToRad(m_DirectionDeg));
	m_Central.y -= (MoveSpeed * m_Direction) * std::sin(DegToRad(m_DirectionDeg));
	m_MapPositionX = static_cast<int>((m_Central.x - m_MapScrollX) / CELL_SIZE);
	m_MapPositionY = static_cast<int>((m_Central.y - m_MapScrollY) / CELL_SIZE);
	if (m_Direction == FACING_RIGHT) {
		m_DirectionBias = ZERO;
	}
	else m_DirectionBias = ONE;
	if (m_Central.x < 0 || m_Central.x > DISPLAY_WIDTH || m_MapPositionX >= m_row - 1) {
		InitPosition();
	}
	if (m_MapPositionY == 0 || m_Central.y < 0 || m_Central.y > DISPLAY_HEIGHT || m_MapPositionY >= m_colunm - 1) {
		InitPosition();
	}
	int buf = m_pMapChip->GetMapChipData(m_MapPositionY, m_MapPositionX);
	if (buf > 100 && buf<200)
	{
		m_pMapChip->Activate(m_MapPositionX, m_MapPositionY);
		m_pSoundOperater->Start("CLAWSHOT", false);
		InitPosition();
	}
	else if (buf < 100 && buf > MapBlock::NONE && buf != MapBlock::START_ZONE&&buf != MapBlock::DESCRIPTION_BOARD)
	{
		m_pSoundOperater->Start("CLAWSHOT", false);
		InitPosition();
	}
	//if (CollisionTarget()) {
	//	m_pMapChip->Activate(m_targetX, m_targetY);
	//	m_pSoundOperater->Start("CLAWSHOT", false);

	//	InitPosition();
	//}


	//if (CollisionRope()) {
	//	m_pMapChip->Activate(m_ropeX, m_ropeY);

	//	InitPosition();
	//}
	PrevMapScrollX = m_MapScrollX;
	PrevMapScrollY = m_MapScrollY;

	return true;
}


void ClawShot::Render()
{
	if (!m_isChoseDeg && !m_isActive) {
		return;
	}
	if (m_isChoseDeg) {
		CUSTOMVERTEX DirectionArrowVertex[4];
		RevolveZEX(DirectionArrowVertex, DegToRad(m_DirectionDeg), m_DirectionArrow, m_DirectionArrow.x - (m_DirectionArrow.scale_x * m_Direction), m_DirectionArrow.y, 0xFFFFFFFF, m_DirectionBias*(BLOCK_INTEGRATION_WIDTH*1.5f), BLOCK_INTEGRATION_HEIGHT * 9.65f, (BLOCK_INTEGRATION_WIDTH*1.5f)*m_Direction, BLOCK_INTEGRATION_HEIGHT*0.5f);
		TextureRender("BLOCK_INTEGRATION_A_TEX", DirectionArrowVertex);
		return;
	}
	if (m_isActive) {
		///////////////////////////////////////////////
		//ロープの描画計算
		CUSTOMVERTEX RopeVertex[4];
		CENTRAL_STATE m_RopeCentral;
		float Xpos = m_Central.x - RopeBatteryPosX ;
		float Ypos = RopeBatteryPosY - m_Central.y;
		//キャラから爪までの斜辺の長さを算出
		float BehindLength = std::sqrt(Xpos*Xpos + Ypos * Ypos);
		//長さから画像の中心を割り出す
		m_RopeCentral.x = (BehindLength * 0.5f * m_Direction + RopeBatteryPosX );
		m_RopeCentral.y = RopeBatteryPosY-1.f;
		m_RopeCentral.scale_x = BehindLength * 0.5f;
		m_RopeCentral.scale_y = 3.5f;
		RevolveZEX(RopeVertex, DegToRad(m_DirectionDeg), m_RopeCentral, RopeBatteryPosX, RopeBatteryPosY, 0xFFFFFFFF, BLOCK_INTEGRATION_WIDTH * 6.0f, 0,13.f/512.f);
		RevolveTexture(RopeVertex, 1);
		///////////////////////////////////////////////
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_B_TEX", RopeVertex);

		CUSTOMVERTEX ClawVertex[4];
		RevolveZ(ClawVertex, DegToRad(m_DirectionDeg), m_Central,0xFFFFFFFF, (m_DirectionBias + 3) * BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT * 5, BLOCK_INTEGRATION_WIDTH * m_Direction, BLOCK_INTEGRATION_HEIGHT);
		m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", ClawVertex);

	}
}

void ClawShot::Reverse(Object* MapChip) {
	m_pMapChip = MapChip;
	m_row = m_pMapChip->GetRow();
	m_colunm = m_pMapChip->GetColunm();

	InitPosition();
}
