/**
* @file Object.cpp
* @brief 色々な物体の基底クラス
* @author Toshiya Matsuoka
*/
#include "Object.h"

std::vector<BlockInfo> Object::m_ReversePoint;
std::vector<MapScroll> Object::m_ReverseBuffer;
int Object::m_ReverseCount = 0;
bool Object::m_isReversing = false;
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
	if ((m_Central.x <= object->x + object->scaleX) && (m_Central.x >= object->x- object->scaleX)
		&& (m_Central.y <= object->y + object->scaleY) && (m_Central.y >= object->y- object->scaleY)) {
		return true;
	}
	return false;
}

void Object::TextureRender(std::string textureKey, CUSTOMVERTEX* textureSize)
{
	m_pDirectX->DrawTexture(textureKey, textureSize);
}


void Object::CreateSquareVertex(CUSTOMVERTEX* vertex, CENTRAL_STATE central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	vertex[0] = { central.x - central.scaleX, central.y - central.scaleY, 1.f, 1.f, color, tu, tv };
	vertex[1] = { central.x + central.scaleX, central.y - central.scaleY, 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { central.x + central.scaleX, central.y + central.scaleY, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { central.x - central.scaleX, central.y + central.scaleY, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Object::CreateSquareVertex(CUSTOMVERTEX* vertex, float x, float y, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	vertex[0] = { 0,  0, 1.f, 1.f, color, tu, tv };
	vertex[1] = { x, 0, 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { x, y, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { 0,  y, 1.f, 1.f, color, tu, tv + scaleTv };
}
void Object::CreateSquareVertex( CENTRAL_STATE central,CUSTOMVERTEX* vertex, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {
	vertex[0] = { central.x , central.y , 1.f, 1.f, color, tu, tv };
	vertex[1] = { central.x + central.scaleX, central.y , 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { central.x + central.scaleX, central.y + central.scaleY, 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { central.x , central.y + central.scaleY, 1.f, 1.f, color, tu, tv + scaleTv };
}

void Object::TranslateCentral_State(CENTRAL_STATE* central, CUSTOMVERTEX* vertex) {
	central->x = vertex[0].x;
	central->y = vertex[0].y;
	central->scaleX = vertex[1].x - vertex[0].x;
	central->scaleY = vertex[3].y - vertex[0].y;
}
void Object::TranslateCentral_State(CUSTOMVERTEX* vertex, CENTRAL_STATE* central) {
	if (!vertex) {
		return;
	}
	central->scaleX = (vertex[1].x - vertex[0].x)*0.5f;
	central->scaleY = (vertex[3].y - vertex[0].y)*0.5f;
	central->x = vertex[0].x + central->scaleX;
	central->y = vertex[0].y + central->scaleY;
}
void Object::SetVertexUV(CUSTOMVERTEX* vertex, float Tu, float Tv, float scaleTu, float scaleTv) {
	vertex[0].tu = Tu;
	vertex[0].tv = Tv;

	vertex[1].tu = Tu + scaleTu;
	vertex[1].tv = Tv;

	vertex[2].tu = Tu + scaleTu;
	vertex[2].tv = Tv + scaleTv;

	vertex[3].tu = Tu;
	vertex[3].tv = Tv + scaleTv;

}
void Object::RevolveZ(CUSTOMVERTEX* vertex, float rad, CENTRAL_STATE central, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float vertexX[4];
	float vertexY[4];

	vertexX[0] = central.x - central.scaleX;
	vertexX[1] = central.x + central.scaleX;
	vertexX[2] = central.x + central.scaleX;
	vertexX[3] = central.x - central.scaleX;

	vertexY[0] = central.y - central.scaleY;
	vertexY[1] = central.y - central.scaleY;
	vertexY[2] = central.y + central.scaleY;
	vertexY[3] = central.y + central.scaleY;

	for (int i = 0; i < 4; i++) {

		vertexX[i] -= central.x;
		vertexY[i] -= central.y;

		float buf = vertexX[i];

		vertexX[i] = (vertexX[i] * cos(-rad)) - (vertexY[i] * sin(-rad));
		vertexY[i] = (vertexY[i] * cos(-rad)) + (buf * sin(-rad));

		vertexX[i] += central.x;
		vertexY[i] += central.y;

	}

	vertex[0] = { vertexX[0], vertexY[0], 1.f, 1.f, color, tu, tv };
	vertex[1] = { vertexX[1], vertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { vertexX[2], vertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { vertexX[3], vertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

void Object::RevolveZ(CUSTOMVERTEX* vertex, float rad, CENTRAL_STATE central, float revolvingShaftX, float revolvingShaftY, DWORD  color, float tu, float tv, float scaleTu, float scaleTv) {

	float vertexX[4];
	float vertexY[4];

	vertexX[0] = central.x - central.scaleX;
	vertexX[1] = central.x + central.scaleX;
	vertexX[2] = central.x + central.scaleX;
	vertexX[3] = central.x - central.scaleX;

	vertexY[0] = central.y - central.scaleY;
	vertexY[1] = central.y - central.scaleY;
	vertexY[2] = central.y + central.scaleY;
	vertexY[3] = central.y + central.scaleY;

	for (int i = 0; i < 4; i++) {

		vertexX[i] -= revolvingShaftX;
		vertexY[i] -= revolvingShaftY;

		float buf = vertexX[i];

		vertexX[i] = (vertexX[i] * cos(-rad)) - (vertexY[i] * sin(-rad));
		vertexY[i] = (vertexY[i] * cos(-rad)) + (buf * sin(-rad));

		vertexX[i] += revolvingShaftX;
		vertexY[i] += revolvingShaftY;

	}

	vertex[0] = { vertexX[0], vertexY[0], 1.f, 1.f, color, tu, tv };
	vertex[1] = { vertexX[1], vertexY[1], 1.f, 1.f, color, tu + scaleTu, tv };
	vertex[2] = { vertexX[2], vertexY[2], 1.f, 1.f, color, tu + scaleTu, tv + scaleTv };
	vertex[3] = { vertexX[3], vertexY[3], 1.f, 1.f, color, tu, tv + scaleTv };

}

void Object::RevolveTexture(CUSTOMVERTEX* vertex, int deg) {

	float vertexX;
	float vertexY;
	//TUTV入れ替えて画像だけ指定回数回す
	for (int i = 0; i < deg; ++i) {
		vertexX = vertex[0].tu;
		vertex[0].tu = vertex[1].tu;
		vertex[1].tu = vertex[2].tu;
		vertex[2].tu = vertex[3].tu;
		vertex[3].tu = vertexX;
		vertexY = vertex[0].tv;
		vertex[0].tv = vertex[1].tv;
		vertex[1].tv = vertex[2].tv;
		vertex[2].tv = vertex[3].tv;
		vertex[3].tv = vertexY;
	}
}


