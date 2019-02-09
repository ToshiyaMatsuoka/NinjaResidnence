/**
* @file SkillSelect.cpp
* @brief 忍術選択UI
* @author Toshiya Matsuoka
*/

#include "SkillSelect.h"

SkillSelect::SkillSelect(DirectX* pDirectX, SoundOperater* pSoundOperater, bool* m_EnableSkill) :Object(pDirectX, pSoundOperater), m_EnableSkill(m_EnableSkill)
{

}

SkillSelect::~SkillSelect()
{

}

bool SkillSelect::Update() {
	switch (m_SkillNum) {
	case SHURIKEN:
		m_SkillTexKey = "BLOCK_INTEGRATION_A_TEX";
		CreateSquareVertex(m_SkillVertex, m_SkillUI, m_Color, 0.f, BLOCK_INTEGRATION_HEIGHT * 2.965f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		break;
	case CLAWSHOT:
		m_SkillTexKey = "BLOCK_INTEGRATION_A_TEX";
		CreateSquareVertex(m_SkillVertex, m_SkillUI, m_Color, 0.f, BLOCK_INTEGRATION_HEIGHT * 4.95f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		break;
	case FIRE_ART:
		m_SkillTexKey = "BLOCK_INTEGRATION_A_TEX";
		CreateSquareVertex(m_SkillVertex, m_SkillUI, m_Color, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT * 5.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
		break;
	case HIGH_SHURIKEN_ART:
		m_SkillTexKey = "BLOCK_INTEGRATION_A_TEX";
		CreateSquareVertex(m_SkillVertex, m_SkillUI, m_Color, BLOCK_INTEGRATION_WIDTH*2.f, BLOCK_INTEGRATION_HEIGHT * 5.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);

		break;
	}
	return true;
}
void SkillSelect::Render()
{
	CUSTOMVERTEX UIVertex[4];
	CreateSquareVertex(UIVertex, m_SelectUI, 0xFFFFFFFF, 0.f, 481.f / 1024.f ,120.f/512.f, 120.f / 1024.f);
	TextureRender("BLOCK_INTEGRATION_A_TEX", UIVertex);

	TextureRender(m_SkillTexKey, m_SkillVertex);
	
}

void SkillSelect::KeyOperation(KeyDirection vec) {
	if (vec == LEFT) {
		SkillTransUp();
			return;
	}
	SkillTransDown();
}

void SkillSelect::SkillTransUp() {
	switch (m_SkillNum) {
	case SHURIKEN:
		if (m_EnableSkill[0]) {
			m_SkillNum = CLAWSHOT;
			break;
		}
	case CLAWSHOT:
		if (m_EnableSkill[1]) {
			m_SkillNum = FIRE_ART;
			break;
		}
	case FIRE_ART:
		if (m_EnableSkill[2]) {
			m_SkillNum = HIGH_SHURIKEN_ART;
			break;
		}
	case HIGH_SHURIKEN_ART:
		m_SkillNum = SHURIKEN;
		break;
	}
}

void SkillSelect::SkillTransDown() {
	switch (m_SkillNum) {
	case HIGH_SHURIKEN_ART:
		if (m_EnableSkill[1]) {
			m_SkillNum = FIRE_ART;
			break;
		}
	case FIRE_ART:
		if (m_EnableSkill[0]) {
			m_SkillNum = CLAWSHOT;
			break;
		}
	case CLAWSHOT:
		m_SkillNum = SHURIKEN;
		break;
	case SHURIKEN:
		if (m_EnableSkill[2]) {
			m_SkillNum = HIGH_SHURIKEN_ART;
			break;
		}
		if (m_EnableSkill[1]) {
			m_SkillNum = FIRE_ART;
			break;
		}
		if (m_EnableSkill[0]) {
			m_SkillNum = CLAWSHOT;
			break;
		}

	}
}

