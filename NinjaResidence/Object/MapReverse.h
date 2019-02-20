/**
* @file MapReverse.h
* @brief MapReverseクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "Object.h"
#include "GameChara.h"


class GameScene;
class GameChara;
class MapChip;

class MapReverse :public Object
{
public:
	void GoMapReverse(Object** m_pBusyMapChip, Object** m_pIdleMapChip);
	MapReverse(DirectX* pDirectX, SoundOperater* pSoundOperater, GameChara * GameChara);
	~MapReverse();
	bool collisonReversePoint(int x, int y, Object* pBusyMapChip);
	bool getMapDataReverseState()
	{
		return MapDataReverseState;
	}
	
private:
	CENTRAL_STATE m_Player = { 400,10,(CELL_SIZE * 2),(CELL_SIZE * 4) };
	GameChara* m_pGameChara = NULL;
	static bool MapDataReverseState;
	int m_ReversePair = 0;
	int MapScrollXBuf = 0;
	int MapScrollYBuf = 0;
	int m_ActiveReversePointNum = -1;

	bool CollLeft = false;
	bool CollRight = false;
	bool CollCenter = false;
};
