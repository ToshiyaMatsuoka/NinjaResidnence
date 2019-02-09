/**
* @file SkillBase.h
* @brief SkillBaseクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "GameChara.h"
#include "SkillSelect.h"



class SkillBase :	public Object
{
public:
	/*
	* @brief キー入力時の内部処理関数
	*/
	virtual void KeyOperation(KeyDirection vec) {};
	virtual bool Update() { return true; };
	virtual void Render() {};
	virtual void Reverse(Object* MapChip) {};


	SkillBase(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara) ;
	virtual ~SkillBase();
	bool GetActive() { return m_isActive; };
protected:
	Object* m_pMapChip = NULL;
	GameChara* m_pGameChara = NULL;
	bool m_isActive = false;
	bool m_isChoseDeg = false;

	int m_MapPositionX;
	int m_MapPositionY;
	int PrevMapScrollX = 0;
	int PrevMapScrollY = 0;

	float m_DirectionDeg = 0;
	//! キャラの向きの判別の為
	float m_Direction = PlayerAnimation::FACING_NOTHING;
	PlayerAnimation::DIRECTION_BIAS m_DirectionBias = PlayerAnimation::ZERO;

	int m_targetX;
	int m_targetY;

	int m_ropeX;
	int m_ropeY;
	bool CollisionRope();

	bool CollisionTarget();

	void InitPosition() {};
	SkillType m_SkillType;
};

