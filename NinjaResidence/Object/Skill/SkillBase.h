/**
* @file SkillBase.h
* @brief SkillBaseクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "../GameChara.h"
#include "../SkillSelect.h"



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

	CENTRAL_STATE m_DirectionArrow = { 500,0,CELL_SIZE*2.5f,CELL_SIZE*0.5f };

	const DWORD DEFFALT_COLOR = 0xFFFFFFFF;

	int m_targetX;
	int m_targetY;

	int m_ropeX;
	int m_ropeY;
	//! ロープとの当たり判定
	bool CollisionRope();
	//! 的との当たり判定
	bool CollisionTarget();
	//! 座標の初期化
	void InitPosition() {};
	SkillType m_SkillType;
};

