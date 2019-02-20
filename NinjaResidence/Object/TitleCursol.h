/**
* @file TitleCursol.h
* @brief タイトルのカーソル処理
* @author Toshiya Matsuoka
*/
#pragma once

#include "Object.h"

namespace Cursol {
	enum CursolPosition {
		START,
		OPTION,
		END
	};
};

class TitleCursol :public Object
{
public:
	void KeyOperation(KeyDirection vec);
	bool Update();
	void Render();
	Cursol::CursolPosition GetCursolPosition();
	TitleCursol(DirectX* pDirectX, SoundOperater* pSoundOperater);
	~TitleCursol();
private:
	Cursol::CursolPosition m_CursolPos = Cursol::START;
	CENTRAL_STATE m_Cursol = { 200,0,40,30 };
	void MoveUp();
	void MoveDown();
};
