/**
* @file SkillBase.cpp
* @brief SkillBaseクラス
* @author Toshiya Matsuoka
*/
#include "SkillBase.h"



SkillBase::SkillBase(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara) :Object(pDirectX, pSoundOperater)
{
	m_pMapChip = MapChip;
	m_pGameChara = GameChara;
}


SkillBase::~SkillBase()
{
}


bool SkillBase::CollisionRope()
{
	CENTRAL_STATE Central = { 0,0,0,0 };
	TranslateCentral_State(m_pMapChip->GetTargetPosition(BT_ROPE), &Central);

	m_ropeX = static_cast<int>((Central.x - m_MapScrollX) / CELL_SIZE);
	m_ropeY = static_cast<int>((Central.y - m_MapScrollY) / CELL_SIZE);

	return ContactSpecifyObject(&Central);
}

bool SkillBase::CollisionTarget()
{
	CENTRAL_STATE Central = { 0 };
	TranslateCentral_State(m_pMapChip->GetTargetPosition(BT_TARGET), &Central);
	bool coll=ContactSpecifyObject(&Central);
	m_targetX = static_cast<int>((Central.x - m_MapScrollX) / CELL_SIZE);
	m_targetY = static_cast<int>((Central.y - m_MapScrollY) / CELL_SIZE);
	return coll;
}

