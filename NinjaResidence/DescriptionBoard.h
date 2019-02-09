/**
* @file DescriptionBoard.h
* @brief DescriptionBoardクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "GAMESCENE.h"
#include "MapChip.h"

class GameChara;
class MapChip;

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
	void GoDescriptionBoard(Object * m_pBusyMapChip);
	DescriptionBoard(DirectX* pDirectX,SoundOperater* pSoundOperater, GameChara * GameChara, Object * m_pBusyMapChip);
	virtual ~DescriptionBoard();
private:
	GameChara* m_pGameChara = NULL;
	Object* m_pMapChip = NULL;
};
