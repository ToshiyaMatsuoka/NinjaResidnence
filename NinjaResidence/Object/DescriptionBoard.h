/**
* @file DescriptionBoard.h
* @brief DescriptionBoardクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "Scene/GameScene.h"
#include "Object/MapChip.h"

class GameChara;

class DescriptionBoard :public Object
{
public:
	CUSTOMVERTEX DescriptionBoardSIZE[4];
	enum DescriptionNumber
	{
		NONE,
		Number1,
		Number2,
	};
	DescriptionNumber DescriptionNumberdecision;
	void GoDescriptionBoard(MapChip* m_pBusyMapChip);
	DescriptionBoard(DirectX* pDirectX,SoundOperater* pSoundOperater, GameChara * gameChara, MapChip* m_pBusyMapChip);
	virtual ~DescriptionBoard();
private:
	GameChara* m_pGameChara = NULL;
	MapChip* m_pMapChip = NULL;
};
