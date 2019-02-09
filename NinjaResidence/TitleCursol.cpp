/**
* @file TitleCursol.cpp
* @brief タイトルのカーソル処理
* @author Toshiya Matsuoka
*/
#include "TITLESCENE.h"
#include "TitleCursol.h"

using namespace Cursol;

TitleCursol::TitleCursol(DirectX* pDirectX, SoundOperater* pSoundOperater) :Object(pDirectX, pSoundOperater)
{

}

TitleCursol::~TitleCursol()
{

}

bool TitleCursol::Update() {
	//数値は仮
	switch (m_CursolPos) {
	case START:
		m_Cursol.y = 430;
		break;
	case OPTION:
		m_Cursol.y = 500;
		break;
	case END:
		m_Cursol.y = 570;
		break;
	}
	return true;
}
void TitleCursol::Render()
{
	CUSTOMVERTEX CursolVertex[4];
	RevolveZ(CursolVertex, DegToRad(180),m_Cursol);
	TextureRender("CURSOL_TEX", CursolVertex);
}

void TitleCursol::KeyOperation(KeyDirection vec) {

	switch (vec)
	{
	case UP:
		MoveUp();
		break;
	case DOWN:
		MoveDown();
		break;
	};
}
void TitleCursol::MoveUp() {
	switch (m_CursolPos) {
	case START:
		m_CursolPos = END;
		break;
	case OPTION:
		m_CursolPos = START;
		break;
	case END:
		m_CursolPos = OPTION;
		break;
	}
}
void TitleCursol::MoveDown() {
	switch (m_CursolPos) {
	case Cursol::START:
		m_CursolPos = OPTION;
		break;
	case Cursol::OPTION:
		m_CursolPos = END;
		break;
	case Cursol::END:
		m_CursolPos = START;
		break;
	}
}
CursolPosition TitleCursol::GetCursolPosition() {
	return m_CursolPos;
}
