/**
* @file BaseGimmick.cpp
* @brief BaseGimmickクラス
* @author Kojiro Kawahara
*/
#include "BaseGimmick.h"


BaseGimmick::BaseGimmick(BlockInfo gimmick, DirectX* pDirectX,SoundOperater* pSoundOperater) :m_GimmickInfo(gimmick)
{
	m_pSoundOperater = pSoundOperater;
	for (int i = 0; i < 4; ++i) {
		m_GimmickVertex[i].color = 0xFFFFFFFF;
		m_GimmickVertex[i].rhw = 1;
	}
}

BaseGimmick::~BaseGimmick()
{

}

