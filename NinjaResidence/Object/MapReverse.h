/**
* @file MapReverse.h
* @brief MapReverseクラス
* @author Kojiro Kawahara
*/
#pragma once

#include "Object/Object.h"
#include "Object/GameChara.h"


class GameScene;
class GameChara;
class MapChip;

class MapReverse :public Object
{
public:
	void GoMapReverse(MapChip** m_pBusyMapChip, MapChip** m_pIdleMapChip);
	MapReverse(DirectX* pDirectX, SoundOperater* pSoundOperater, GameChara * gameChara);
	~MapReverse();
	bool CollisonReversePoint(int x, int y, MapChip* pBusyMapChip);
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
