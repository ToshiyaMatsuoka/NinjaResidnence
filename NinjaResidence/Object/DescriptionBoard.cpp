/**
* @file DescriptionBoard.cpp
* @brief DescriptionBoardクラス
* @author Kojiro Kawahara
*/
#include "DescriptionBoard.h"

using namespace MapBlock;

DescriptionBoard::DescriptionBoard(DirectX* pDirectX,SoundOperater* pSoundOperater,GameChara * GameChara, Object * pBusyMapChip) :Object(pDirectX, pSoundOperater)
{
	m_pGameChara = GameChara;
	m_pMapChip = pBusyMapChip;
	DescriptionNumberdecision = NONE;
	DescriptionBoardSIZE[0] = { 300,110,1.0f, 1.0f,0xFFFFFFFF, 0.f, 0.f };
	DescriptionBoardSIZE[1] = { 980,110,1.0f, 1.0f,0xFFFFFFFF, 1.0f, 0.f };
	DescriptionBoardSIZE[2] = { 980,500,1.0f, 1.0f,0xFFFFFFFF, 1.0f, 1.0f };
	DescriptionBoardSIZE[3] = { 300,500,1.0f, 1.0f,0xFFFFFFFF, 0.f, 1.0f };
}

DescriptionBoard::~DescriptionBoard()
{

}

void DescriptionBoard::GoDescriptionBoard(Object* pBusyMapChip)
{
	m_pMapChip = pBusyMapChip;
	int MapPosiinonX = m_pGameChara->GetMapLeftDirectionPosition();
	int MapPosiinonY = m_pGameChara->GetMapPositionY();
	if ((m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX + 1) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX + 2) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX - 1) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX + 1) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX + 2) == DESCRIPTION_BOARD) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX - 1) == DESCRIPTION_BOARD))
	{
		DescriptionNumberdecision = Number1;
	}
	if ((m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX + 1) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX + 2) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 5, MapPosiinonX - 1) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX + 1) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX + 2) == DESCRIPTION_BOARD2) ||
		(m_pMapChip->GetMapChipData(MapPosiinonY - 4, MapPosiinonX - 1) == DESCRIPTION_BOARD2))
	{
		DescriptionNumberdecision = Number2;
	}
}
