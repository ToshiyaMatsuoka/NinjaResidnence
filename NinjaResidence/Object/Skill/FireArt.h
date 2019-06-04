﻿/**
* @file FireArt.h
* @brief FireArtクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "SkillBase.h"
#include "Object/MapChip.h"

class SkillBase;

class FireArt :public SkillBase
{
public:
	/*
	* @brief キー入力時の内部処理関数
	*/
	void KeyOperation(KeyDirection vec);



	bool Update();
	void Render();
	void Reverse(MapChip* MapChip);


	FireArt(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* MapChip, GameChara* GameChara);
	~FireArt();
	bool GetActive() { return m_isActive; };
private:
	
	/*
	* @brief 起動処理
	* @return 起動したらtrue
	*/
	bool PermitActive();

	bool m_isActive = false;
	bool m_isChoseDeg = false;

	int m_MapPositionX;
	int m_MapPositionY;

	float m_DirectionDeg = 0;
	//! キャラの向きの判別の為
	float m_Direction = PlayerAnimation::FACING_NOTHING;
	PlayerAnimation::DIRECTION m_Facing = PlayerAnimation::FACING_RIGHT;

	void InitPosition();
	float m_Animation = 0.f;
	const float CHARA_TEXTURE_WIDTH = 4096.f;
	const float m_CharTu = 320 / CHARA_TEXTURE_WIDTH;
	const float m_CharTv = 320 / CHARA_TEXTURE_WIDTH;

};
