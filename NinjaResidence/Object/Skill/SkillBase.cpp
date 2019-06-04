/**
* @file SkillBase.cpp
* @brief SkillBaseクラス
* @author Toshiya Matsuoka
*/
#include "SkillBase.h"



SkillBase::SkillBase(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* MapChip, GameChara* GameChara) :Object(pDirectX, pSoundOperater)
{
	m_pMapChip = MapChip;
	m_pGameChara = GameChara;
}


SkillBase::~SkillBase()
{
}


bool SkillBase::CollisionRope()
{
	CENTRAL_STATE central = { 0,0,0,0 };
	TranslateCentral_State(m_pMapChip->GetTargetPosition(BT_ROPE), &central);

	m_ropeX = static_cast<int>((central.x - MapChip::GetScroll().X) / CELL_SIZE);
	m_ropeY = static_cast<int>((central.y - MapChip::GetScroll().Y) / CELL_SIZE);

	return ContactSpecifyObject(&central);
}

bool SkillBase::CollisionTarget()
{
	CENTRAL_STATE central = { 0 };
	TranslateCentral_State(m_pMapChip->GetTargetPosition(BT_TARGET), &central);
	bool coll=ContactSpecifyObject(&central);
	m_targetX = static_cast<int>((central.x - MapChip::GetScroll().X) / CELL_SIZE);
	m_targetY = static_cast<int>((central.y - MapChip::GetScroll().Y) / CELL_SIZE);
	return coll;
}

