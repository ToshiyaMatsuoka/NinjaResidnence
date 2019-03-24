/**
* @file HighShuriken.h
* @brief 高等手裏剣クラス
* @author Toshiya Matsuoka
*/

#pragma once
#include "Object/Skill/SkillBase.h"
#include "Object/MapChip.h"

class SkillBase;
class XinputDevice;

class HighShuriken :public SkillBase
{
public:
	/*
	* @brief キー入力時の内部処理関数
	*/
	void KeyOperation(KeyDirection vec);



	bool Update();
	void Render();
	void Reverse(Object* mapChip);

	float InputRightStickDeg();


	HighShuriken(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* mapChip, GameChara* gameChara, XinputDevice* pXinputDevice);
	~HighShuriken();
	bool GetActive() { return m_isActive; };
private:
	XinputDevice* m_pXinputDevice = NULL;
	void InitPosition();
	/*
	* @brief 投擲起動処理
	* @return 投げられたらtrue
	* @details 投擲前に角度指定フェイズを挟む
	*/
	bool PermitActive();
	bool isOperation = false;
};

