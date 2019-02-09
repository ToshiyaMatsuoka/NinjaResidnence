/**
* @file TargetandGimmickType.h
* @brief enum TargetandGimmickTypeの宣言
* @author Kojiro Kawahara
*/
#pragma once

/**
* @enum TargetandGimmickType
* ターゲット、ギミックの判別
*/
enum TargetandGimmickType
{
	BT_NONE,
	//的
	BT_TARGET,
	BT_ROPE,
	BT_SWITCH,
	BT_TORCH,
	//ギミック
	BT_PARTITIONBOARD,
	BT_FALLROCK,
	BT_WATER,
	BT_ADHEREBOARD,
};
