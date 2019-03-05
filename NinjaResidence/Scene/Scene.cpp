/**
* @file Scene.cpp
* @brief Sceneクラス
* @author Kojiro Kawahara
*/
#include "Scene.h"
#include "../BlockInfo.h"

int Scene::m_StageNum = 8;
bool Scene::m_SoundSetting = false;
Scene :: Scene(DirectX* pDirectX, SoundOperater* pSoundOperater):m_pDirectX(pDirectX), m_pSoundOperater(pSoundOperater)
{
	m_pDirectX = pDirectX;
	m_pXinputDevice = new XinputDevice;
	m_pSoundOperater = pSoundOperater;
	
	m_pDirectX->LoadTexture("Resource/Texture/object_a.png", "BLOCK_INTEGRATION_A_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/Load_BG.jpg", "LOAD_BG_TEX");

	m_pDirectX->LoadTexture("Resource/Texture/GyoushoNumber.png", "NUMBER_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/SoundVolumeMenu.png", "SV_MENU_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/SoundSelectCursor.png", "SV_CURSOR_TEX");
	m_pDirectX->LoadTexture("Resource/Texture/Setting_BG.jpg", "VOLUME_BG_TEX");

	m_pDirectX->SetFont(75, 75, "LOAD_FONT");
}

Scene::~Scene()
{
	delete m_pXinputDevice;
	m_pXinputDevice = NULL;
}


void Scene::CreateSquareVertex(CUSTOMVERTEX* vertex, CENTRAL_STATE central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	vertex[0] = { central.x - central.scaleX, central.y - central.scaleY, 1.f, 1.f, color, tu, tv };
	vertex[1] = { central.x + central.scaleX, central.y - central.scaleY, 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { central.x + central.scaleX, central.y + central.scaleY, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { central.x - central.scaleX, central.y + central.scaleY, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Scene::CreateSquareVertex(CUSTOMVERTEX* vertex, float x,float y, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	vertex[0] = { 0,  0, 1.f, 1.f, color, tu, tv };
	vertex[1] = { x, 0, 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { x, y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { 0,  y, 1.f, 1.f, color, tu, tv + scaleTv };
}

void Scene::LoadAnimation() {
	CUSTOMVERTEX LoadBg[4];
	CreateSquareVertex(LoadBg, DISPLAY_WIDTH, DISPLAY_HEIGHT - 20);
	m_pDirectX->DrawTexture("LOAD_BG_TEX", LoadBg);

	static int rad = 0;
	rad += static_cast<int>(D3DX_PI) * 3;
	CUSTOMVERTEX cross[4];
	CENTRAL_STATE CROSS = { 1200,630,50,50 };
	RevolveZ(cross, static_cast<float>(rad), CROSS,0xFFFFFFFF, 0.f, BLOCK_INTEGRATION_HEIGHT * 2.96f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
	m_pDirectX->DrawTexture("BLOCK_INTEGRATION_A_TEX", cross);
}


void Scene::RevolveZ(CUSTOMVERTEX* vertex, float rad, CENTRAL_STATE central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float CharVertexX[4];
	float CharVertexY[4];

	CharVertexX[0] = central.x - central.scaleX;
	CharVertexX[1] = central.x + central.scaleX;
	CharVertexX[2] = central.x + central.scaleX;
	CharVertexX[3] = central.x - central.scaleX;

	CharVertexY[0] = central.y - central.scaleY;
	CharVertexY[1] = central.y - central.scaleY;
	CharVertexY[2] = central.y + central.scaleY;
	CharVertexY[3] = central.y + central.scaleY;

	for (int RoteCnt = 0; RoteCnt < 4; RoteCnt++) {

		CharVertexX[RoteCnt] -= central.x;
		CharVertexY[RoteCnt] -= central.y;

		float KEEPER = CharVertexX[RoteCnt];

		CharVertexX[RoteCnt] = (CharVertexX[RoteCnt] * cos(-rad)) - (CharVertexY[RoteCnt] * sin(-rad));
		CharVertexY[RoteCnt] = (CharVertexY[RoteCnt] * cos(-rad)) + (KEEPER * sin(-rad));

		CharVertexX[RoteCnt] += central.x;
		CharVertexY[RoteCnt] += central.y;

	}

	vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}
