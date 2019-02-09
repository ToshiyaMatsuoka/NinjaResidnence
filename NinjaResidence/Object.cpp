/**
* @file Object.cpp
* @brief 色々な物体の基底クラス
* @author Toshiya Matsuoka
*/
#include "Object.h"

int Object::m_MapScrollX = 0;
int Object::m_MapScrollY = 0;
std::vector<BlockInfo> Object::m_ReversePoint;
std::vector<MapScrollBuffer> Object::m_ReverseBuffer;
int Object::m_ReverseCount = 0;

Object::Object(DirectX* pDirectX, SoundOperater* pSoundOperater):m_pDirectX(pDirectX), m_pSoundOperater(pSoundOperater)
{
}



Object::~Object()
{
}



void Object::KeyOperation()
{
}


bool Object::Update()
{
	return true;
}


bool Object::ContactSpecifyObject(CENTRAL_STATE* object)
{
	if ((m_Central.x <= object->x + object->scale_x) && (m_Central.x >= object->x- object->scale_x)
		&& (m_Central.y <= object->y + object->scale_y) && (m_Central.y >= object->y- object->scale_y)) {
		return true;
	}
	return false;
}

void Object::TextureRender(std::string TextureKey, CUSTOMVERTEX* TextureSize)
{
	m_pDirectX->DrawTexture(TextureKey, TextureSize);
}


void Object::CreateSquareVertex(CUSTOMVERTEX* Vertex, CENTRAL_STATE Central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	Vertex[0] = { Central.x - Central.scale_x, Central.y - Central.scale_y, 1.f, 1.f, color, tu, tv };
	Vertex[1] = { Central.x + Central.scale_x, Central.y - Central.scale_y, 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { Central.x + Central.scale_x, Central.y + Central.scale_y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { Central.x - Central.scale_x, Central.y + Central.scale_y, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Object::CreateSquareVertex(CUSTOMVERTEX* Vertex, float x, float y, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	Vertex[0] = { 0,  0, 1.f, 1.f, color, tu, tv };
	Vertex[1] = { x, 0, 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { x, y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { 0,  y, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Object::CreateSquareVertex( CENTRAL_STATE Central,CUSTOMVERTEX* Vertex, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	Vertex[0] = { Central.x , Central.y , 1.f, 1.f, color, tu, tv };
	Vertex[1] = { Central.x + Central.scale_x, Central.y , 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { Central.x + Central.scale_x, Central.y + Central.scale_y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { Central.x , Central.y + Central.scale_y, 1.f, 1.f, color, tu, tv + scaleTv };
}

void Object::TranslateCentral_State(CENTRAL_STATE* Central, CUSTOMVERTEX* Vertex) {
	Central->x = Vertex[0].x;
	Central->y = Vertex[0].y;
	Central->scale_x = Vertex[1].x - Vertex[0].x;
	Central->scale_y = Vertex[3].y - Vertex[0].y;
}
void Object::TranslateCentral_State(CUSTOMVERTEX* Vertex, CENTRAL_STATE* Central) {
	if (!Vertex) {
		return;
	}
	Central->scale_x = (Vertex[1].x - Vertex[0].x)*0.5f;
	Central->scale_y = (Vertex[3].y - Vertex[0].y)*0.5f;
	Central->x = Vertex[0].x + Central->scale_x;
	Central->y = Vertex[0].y + Central->scale_y;
}
void Object::SetVertexUV(CUSTOMVERTEX* Vertex, float Tu, float Tv, float scaleTu, float scaleTv) {
	Vertex[0].tu = Tu;
	Vertex[0].tv = Tv;

	Vertex[1].tu = Tu + scaleTu;
	Vertex[1].tv = Tv;

	Vertex[2].tu = Tu + scaleTu;
	Vertex[2].tv = Tv + scaleTv;

	Vertex[3].tu = Tu;
	Vertex[3].tv = Tv + scaleTv;

}
void Object::RevolveZ(CUSTOMVERTEX* Vertex, float Rad, CENTRAL_STATE Central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

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

	for (int i = 0; i < 4; i++) {

		CharVertexX[i] -= Central.x;
		CharVertexY[i] -= Central.y;

		float KEEPER = CharVertexX[i];

		CharVertexX[i] = (CharVertexX[i] * cos(-Rad)) - (CharVertexY[i] * sin(-Rad));
		CharVertexY[i] = (CharVertexY[i] * cos(-Rad)) + (KEEPER * sin(-Rad));

		CharVertexX[i] += Central.x;
		CharVertexY[i] += Central.y;

	}

	Vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	Vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

void Object::RevolveZEX(CUSTOMVERTEX* Vertex, float Rad, CENTRAL_STATE Central, float RevolvingShaftX, float RevolvingShaftY, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

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

	for (int i = 0; i < 4; i++) {

		CharVertexX[i] -= RevolvingShaftX;
		CharVertexY[i] -= RevolvingShaftY;

		float KEEPER = CharVertexX[i];

		CharVertexX[i] = (CharVertexX[i] * cos(-Rad)) - (CharVertexY[i] * sin(-Rad));
		CharVertexY[i] = (CharVertexY[i] * cos(-Rad)) + (KEEPER * sin(-Rad));

		CharVertexX[i] += RevolvingShaftX;
		CharVertexY[i] += RevolvingShaftY;

	}

	Vertex[0] = { CharVertexX[0], CharVertexY[0], 1.f, 1.f, color, tu, tv };
	Vertex[1] = { CharVertexX[1], CharVertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	Vertex[2] = { CharVertexX[2], CharVertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	Vertex[3] = { CharVertexX[3], CharVertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

void Object::RevolveTexture(CUSTOMVERTEX* Vertex, int Deg) {

	float VertexX;
	float VertexY;
	//TUTV入れ替えて画像だけ指定回数回す
	for (int i = 0; i < Deg; ++i) {
		VertexX = Vertex[0].tu;
		Vertex[0].tu = Vertex[1].tu;
		Vertex[1].tu = Vertex[2].tu;
		Vertex[2].tu = Vertex[3].tu;
		Vertex[3].tu = VertexX;
		VertexY = Vertex[0].tv;
		Vertex[0].tv = Vertex[1].tv;
		Vertex[1].tv = Vertex[2].tv;
		Vertex[2].tv = Vertex[3].tv;
		Vertex[3].tv = VertexY;
	}
}


void Object::WriteLog(std::string Text)
{
	const char* fileName = "test.txt";
	std::ofstream ofs(fileName, std::ios::out);

	if (!ofs)
	{
		std::cout << "ファイルが開けませんでした。" << std::endl;
		std::cin.get();
		return;
	}

	ofs << Text << std::endl;
	std::cout << fileName << "に書き込みました。" << std::endl;

	std::cin.get();
}

