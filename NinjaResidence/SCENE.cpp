/**
* @file SCENE.cpp
* @brief SCENEクラス
* @author Kojiro Kawahara
*/
#include "SCENE.h"
#include "BlockInfo.h"

int Scene::m_StageNum = 8;
bool Scene::m_SoundSetting = false;
Scene :: Scene(DirectX* pDirectX, SoundOperater* pSoundOperater):m_pDirectX(pDirectX), m_pSoundOperater(pSoundOperater)
{
	m_pDirectX = pDirectX;
	m_pXinputDevice = new XinputDevice;
	m_pSoundOperater = pSoundOperater;
	
	m_pDirectX->LoadTexture("texture/object_a.png", "BLOCK_INTEGRATION_A_TEX");
	m_pDirectX->LoadTexture("texture/Load_BG.jpg", "LOAD_BG_TEX");

	m_pDirectX->LoadTexture("Texture/GyoushoNumber.png", "NUMBER_TEX");
	m_pDirectX->LoadTexture("Texture/SoundVolumeMenu.png", "SV_MENU_TEX");
	m_pDirectX->LoadTexture("Texture/SoundSelectCursor.png", "SV_CURSOR_TEX");
	m_pDirectX->LoadTexture("texture/Setting_BG.jpg", "VOLUME_BG_TEX");

	m_pDirectX->SetFont(75, 75, "LOAD_FONT");
}

Scene::~Scene()
{
	delete m_pXinputDevice;
	m_pXinputDevice = NULL;
}


void Scene::CreateSquareVertex(CUSTOMVERTEX* Vertex, CENTRAL_STATE Central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	Vertex[0] = { Central.x - Central.scale_x, Central.y - Central.scale_y, 1.f, 1.f, color, tu, tv };
	Vertex[1] = { Central.x + Central.scale_x, Central.y - Central.scale_y, 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { Central.x + Central.scale_x, Central.y + Central.scale_y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { Central.x - Central.scale_x, Central.y + Central.scale_y, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Scene::CreateSquareVertex(CUSTOMVERTEX* Vertex, float x,float y, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	Vertex[0] = { 0,  0, 1.f, 1.f, color, tu, tv };
	Vertex[1] = { x, 0, 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { x, y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { 0,  y, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Scene::GoToOptionScene() {

}

void Scene::RunOptionScene(int m_BGMvolume,int m_SEvolume) {

	
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


void Scene::RevolveZ(CUSTOMVERTEX* Vertex, float Rad, CENTRAL_STATE Central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float CharVertexX[4];
	float CharVertexY[4];

	CharVertexX[0] = Central.x - Central.scale_x;
	CharVertexX[1] = Central.x + Central.scale_x;
	CharVertexX[2] = Central.x + Central.scale_x;
	CharVertexX[3] = Central.x - Central.scale_x;

	CharVertexY[0] = Central.y - Central.scale_y;
	CharVertexY[1] = Central.y - Central.scale_y;
	CharVertexY[2] = Central.y + Central.scale_y;
	CharVertexY[3] = Central.y + Central.scale_y;

	for (int RoteCnt = 0; RoteCnt < 4; RoteCnt++) {

		CharVertexX[RoteCnt] -= Central.x;
		CharVertexY[RoteCnt] -= Central.y;

		float KEEPER = CharVertexX[RoteCnt];

		CharVertexX[RoteCnt] = (CharVertexX[RoteCnt] * cos(-Rad)) - (CharVertexY[RoteCnt] * sin(-Rad));
		CharVertexY[RoteCnt] = (CharVertexY[RoteCnt] * cos(-Rad)) + (KEEPER * sin(-Rad));

		CharVertexX[RoteCnt] += Central.x;
		CharVertexY[RoteCnt] += Central.y;

	}

	Vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	Vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}


void Scene::WriteLog(std::string Text)
{
	const char* fileName = "test.txt";
	std::ofstream ofs(fileName, std::ios::out);

	if (!ofs)
	{
		std::cout << "ファイルが開けませんでした。" << std::endl;
		std::cin.get();
		return ;
	}

	ofs << Text << std::endl;
	std::cout << fileName << "に書き込みました。" << std::endl;

	std::cin.get();
}
