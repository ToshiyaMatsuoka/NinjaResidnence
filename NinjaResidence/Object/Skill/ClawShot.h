/**
* @file ClawShot.h
* @brief 鉤爪ロープクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "Object/Skill/SkillBase.h"
#include "Object/MapChip.h"

class SkillBase;

class ClawShot :public SkillBase
{
public:
	/*
	* @brief キー入力時の内部処理関数
	*/
	void KeyOperation(KeyDirection vec);



	bool Update();
	void Render();
	void Reverse(MapChip* MapChip);


	ClawShot(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* MapChip, GameChara* GameChara);
	~ClawShot();
	bool GetActive() { return m_isActive; };
private:
	float RopeBatteryPosX = 0;
	float RopeBatteryPosY = 0;

	//! 位置の初期化
	void InitPosition();
	/*
	* @brief 投擲起動処理
	* @return 投げられたらtrue
	* @details 投擲前に角度指定フェイズを挟む
	*/
	bool PermitActive();
	int m_ropeX;
	int m_ropeY;
};

