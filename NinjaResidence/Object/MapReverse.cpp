/**
* @file MapReverse.cpp
* @brief MapReverseクラス
* @author Kojiro Kawahara
*/
#include "MapReverse.h"

using namespace MapBlock;
bool MapReverse::MapDataReverseState = true;


MapReverse::MapReverse(DirectX* pDirectX, SoundOperater* pSoundOperater, GameChara* gameChara) :Object(pDirectX, pSoundOperater)
{
	m_pGameChara = gameChara;
}


MapReverse::~MapReverse()
{

}

bool MapReverse::CollisonReversePoint(int x, int y, Object* pBusyMapChip) {
	MapDataState buf = STATE_FALSE;
	for (int i = 0; i < m_ReverseCount; ++i) {
		if (m_ReversePoint[i].PositionX == x &&
			m_ReversePoint[i].PositionY == y &&
			m_ReversePoint[i].MapDataState == (buf = (*pBusyMapChip).GetMapDataState())) {

			m_ReversePair = m_ReversePoint[i].PairNumber;
			m_ActiveReversePointNum = i;
			return true;
		}
	}
	return false;
}

void MapReverse::GoMapReverse(Object** pBusyMapChip, Object** pIdleMapChip)
{

	int mapPosiinonX = m_pGameChara->GetMapLeftDirectionPosition() + 1;
	int mapPosiinonY = m_pGameChara->GetMapPositionY();

	CollLeft = CollisonReversePoint(mapPosiinonX-1, mapPosiinonY, *pBusyMapChip);
	CollRight = CollisonReversePoint(mapPosiinonX + 1, mapPosiinonY, *pBusyMapChip);
	CollCenter = CollisonReversePoint(mapPosiinonX + 0, mapPosiinonY, *pBusyMapChip);
	if ((CollCenter)&& m_ReversePair)
	{
		m_pSoundOperater->Start("REVERSE",false);
		MapScrollBuffer msBuf = { m_MapScrollX ,m_MapScrollY };
		for (int i = 0; i < m_ReverseCount; ++i) {
			if (m_ReversePoint[i].PairNumber == m_ReversePair &&
				m_ReversePoint[i].MapDataState == (*pBusyMapChip)->GetMapDataState()) {
				m_ReverseBuffer[i].ScrollX = msBuf.ScrollX;
				m_ReverseBuffer[i].ScrollY = msBuf.ScrollY;
			}
			if (m_ReversePoint[i].PairNumber== m_ReversePair&&
				m_ReversePoint[i].MapDataState != (*pBusyMapChip)->GetMapDataState()) {
				if (i == m_ActiveReversePointNum) {
					continue;
				}
				m_MapScrollX = m_ReverseBuffer[i].ScrollX;
				m_MapScrollY = m_ReverseBuffer[i].ScrollY;
			}
		}

		Object* mapbuf;
		mapbuf = (*pBusyMapChip);
		*pBusyMapChip = *pIdleMapChip;
		*pIdleMapChip = mapbuf;
		m_isReversing = true;
		m_pGameChara->Reverce(*pBusyMapChip, m_ReversePair);
	}
	CollLeft = CollRight = CollCenter = false;
}
