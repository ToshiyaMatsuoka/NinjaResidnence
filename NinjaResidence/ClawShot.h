/**
* @file ClawShot.h
* @brief 鉤爪ロープクラス
* @author Toshiya Matsuoka
*/
#pragma once
#include "SkillBase.h"
#include "SkillBase.h"
#include "MapChip.h"

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
	void Reverse(Object* MapChip);


	ClawShot(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara);
	~ClawShot();
	bool GetActive() { return m_isActive; };
private:
	CENTRAL_STATE m_DirectionArrow = { 500,0,100,20 };
	CUSTOMVERTEX m_DisplayCoordinate[4];
	CUSTOMVERTEX m_WorldCoordinate[4];
	float RopeBatteryPosX = 0;
	float RopeBatteryPosY = 0;


	const float MoveSpeed = 10.f;
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

