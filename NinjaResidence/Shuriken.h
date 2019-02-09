/**
* @file Shuriken.h
* @brief 手裏剣クラス
* @author Toshiya Matsuoka
*/

#pragma once
#include "SkillBase.h"
#include "MapChip.h"

class SkillBase;

class Shuriken :public SkillBase
{
public:
	/*
	* @brief キー入力時の内部処理関数
	*/
	void KeyOperation(KeyDirection vec);



	bool Update();
	void Render();
	void Reverse(Object* MapChip);


	Shuriken(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip, GameChara* GameChara);
	~Shuriken();
	bool GetActive() { return m_isActive; };
private:
	CENTRAL_STATE m_DirectionArrow = { 500,0,100,20 };
	CUSTOMVERTEX m_DisplayCoordinate[4];
	CUSTOMVERTEX m_WorldCoordinate[4];

	const float MoveSpeed = 10.f;
	void InitPosition();
	/*
	* @brief 投擲起動処理
	* @return 投げられたらtrue
	* @details 投擲前に角度指定フェイズを挟む
	*/
	bool PermitActive();
};
