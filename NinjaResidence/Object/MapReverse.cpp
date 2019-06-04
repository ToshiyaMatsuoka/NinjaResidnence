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

bool MapReverse::CollisonReversePoint(int x, int y, MapChip* pBusyMapChip) {
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

void MapReverse::GoMapReverse(MapChip** pBusyMapChip, MapChip** pIdleMapChip)
{

	int mapPosiinonX = m_pGameChara->GetMapLeftDirectionPosition() + 1;
	int mapPosiinonY = m_pGameChara->GetMapPositionY();

	CollLeft = CollisonReversePoint(mapPosiinonX-1, mapPosiinonY, *pBusyMapChip);
	CollRight = CollisonReversePoint(mapPosiinonX + 1, mapPosiinonY, *pBusyMapChip);
	CollCenter = CollisonReversePoint(mapPosiinonX + 0, mapPosiinonY, *pBusyMapChip);
	if ((CollCenter)&& m_ReversePair)
	{
		m_pSoundOperater->Start("REVERSE",false);
		MapScroll msBuf = MapChip::GetScroll();
		for (int i = 0; i < m_ReverseCount; ++i) {
			if (m_ReversePoint[i].PairNumber == m_ReversePair &&
				m_ReversePoint[i].MapDataState == (*pBusyMapChip)->GetMapDataState()) {
				m_ReverseBuffer[i].X = msBuf.X;
				m_ReverseBuffer[i].Y = msBuf.Y;
			}
			if (m_ReversePoint[i].PairNumber== m_ReversePair&&
				m_ReversePoint[i].MapDataState != (*pBusyMapChip)->GetMapDataState()) {
				if (i == m_ActiveReversePointNum) {
					continue;
				}
				MapChip::SetScroll(m_ReverseBuffer[i]);
			}
		}

		MapChip* mapbuf;
		mapbuf = (*pBusyMapChip);
		*pBusyMapChip = *pIdleMapChip;
		*pIdleMapChip = mapbuf;
		m_isReversing = true;
		m_pGameChara->Reverce(*pBusyMapChip, m_ReversePair);
	}
	CollLeft = CollRight = CollCenter = false;
}
