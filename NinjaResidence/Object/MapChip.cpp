/**
* @file MapChip.cpp
* @brief MapChipクラス
* @author Kojiro Kawahara
*/
#include "MapChip.h"
#include "TargetandGimmickType.h"

using std::vector;
using namespace MapBlock;

MapScroll MapChip::m_MapScroll;
int MapChip::m_TargetCount = 0;
int MapChip::m_GimmickCount = 0;
vector<BaseTarget*> MapChip::pBaseTarget;
vector<BlockInfo> MapChip::GimmickVector;
vector<BlockInfo> MapChip::TargetVector;
float MapChip::m_Rad = 0;

MapChip::MapChip(DirectX* pDirectX, SoundOperater* pSoundOperater) :Object(pDirectX,pSoundOperater)
{
	m_TargetCount = 0;
	m_GimmickCount = 0;
	CellInit();
}

MapChip::~MapChip()
{
	m_MapScroll = { 0, 0 };
	m_ReverseCount = 0;
	for (int i = 0; i < m_MapSizeY; i++)
	{
		MapData[i].clear();
		vector<int>().swap(MapData[i]);
	}
	MapData.clear();
	vector<vector<int>>().swap(MapData);
	GimmickVector.clear();
	TargetVector.clear();
	m_ReversePoint.clear();

	for (int i = (static_cast<int>(pBaseTarget.size()) - 1); i >= 0; --i)
	{
		delete pBaseTarget[i];
		pBaseTarget[i] = NULL;
	}
	pBaseTarget.clear();
}

void MapChip::Create(std::string filename, MapDataState mapState)
{
	m_MapDataState = mapState;
	int y = 0;

	std::ifstream ifs(filename);
	std::string str;

	getline(ifs, str);
	replace(str.begin(), str.end(), ',', ' ');
	std::stringstream MapSizeStream(str);
	MapSizeStream >> m_MapSizeX >> m_MapSizeY;

	MapData.resize(m_MapSizeY);

	for (int j = 0; j<m_MapSizeY; j++)
	{
		MapData[j].resize(m_MapSizeX);
	}

	while (getline(ifs, str))
	{
		replace(str.begin(), str.end(), ',', ' ');
		std::stringstream MapNumsStream(str);
		for (int x = 0; x < static_cast<int>(MapData[y].size());++x) {
			MapNumsStream >> MapData[y][x];
			BlockInfo block;
			int blocktype = MapData[y][x] / 100;
			if (blocktype)
			{
				int PairNum = 0;
				if (blocktype < BT_PARTITIONBOARD)
				{
					PairNum = MapData[y][x] % 100;
					block = { x,y,PairNum,blocktype,mapState,this };
					TargetVector.push_back(block);
					m_TargetCount++;
				}
				else if (blocktype > BT_TORCH && blocktype < WOOD_REVERSE_ZONE)
				{
					PairNum = MapData[y][x] % 100;
					block = { x,y,PairNum,blocktype,mapState,this };
					GimmickVector.push_back(block);
					m_GimmickCount++;
				}
				else  if (blocktype == WOOD_REVERSE_ZONE)
				{
					PairNum = MapData[y][x] % 100;
					block = { x,y,PairNum,blocktype,mapState,this };
					m_ReversePoint.push_back(block);
					m_ReverseCount++;
				}
				else if (blocktype == ROCK_REVERSE_ZONE)
				{
					PairNum = MapData[y][x] % 100;
					block = { x,y,PairNum,blocktype,mapState,this };
					m_ReversePoint.push_back(block);
					m_ReverseCount++;
				}

			}
		}
	y++;
	if (y > static_cast<int>(MapData.size())) break;
	}

	if (mapState == REVERSE)
	{
		CreateGimmick();
		m_ReverseBuffer.resize(m_ReverseCount);
	}
}

void MapChip::MapDataVectorHitSet(int mapY, int mapX, int gimmickY, int gimmickX)
{
	for (int j = 0;j < gimmickY;j++)
	{
		for (int i = 0;i < gimmickX;i++)
		{
			SetMapData(mapY + j,mapX + i, 900);
		}
	}
}

void MapChip::MapDataVectorZeroSet(int mapY, int mapX, int gimmickY, int gimmickX)
{
	for (int j = -1;j < gimmickY;j++)
	{
		for (int i = 0;i < gimmickX;i++)
		{
			SetMapData(mapY + j, mapX + i, 0);
		}
	}
}

void MapChip::CreateGimmick()
{
	BaseTarget* pBuf = NULL;

	for (int i = 0; i < m_TargetCount; i++)
	{
		for (int j = 0; j < m_GimmickCount; j++)
		{
			if (TargetVector[i].PairNumber % 100 != GimmickVector[j].PairNumber % 100) continue;
			switch (TargetVector[i].GimmickType)
			{
			case BT_TARGET://的
				pBuf = new Target(TargetVector[i], GimmickVector[j], m_pDirectX, m_pSoundOperater);
				break;
			case BT_ROPE://ロープ
				pBuf = new Rope(TargetVector[i], GimmickVector[j], m_pDirectX, m_pSoundOperater);
				break;
			case BT_SWITCH://スイッチ
				pBuf = new Switch(TargetVector[i], GimmickVector[j], m_pDirectX, m_pSoundOperater);
				break;
			case BT_TORCH://たいまつ
				pBuf = new Torch(TargetVector[i], GimmickVector[j], m_pDirectX, m_pSoundOperater);
				break;
			}
			pBaseTarget.push_back(pBuf);
		}
	}
}

void MapChip::Activate(int X, int Y)
{
	for (auto& ite : pBaseTarget)
	{
		if (X == ite->GetTargetInfo()->PositionX && Y == ite->GetTargetInfo()->PositionY)
		{
			ite->ActivateTarget();
		}
	}
}

int MapChip::GimmickMapDataCheck(int y, int x)
{ 
	if (MapData[y][x] == 0)
	{
		return 0;
	}

	if (MapData[y][x] > 300)
	{
		return 3;
	}
	return 1;
}

void MapChip::Render()
{
	int TopCellPos = (m_MapScroll.Y * -1)/ static_cast<int>(CELL_SIZE);
	int LeftCellPos = (m_MapScroll.X * -1) / static_cast<int>(CELL_SIZE);
	if (LeftCellPos<0) {
		LeftCellPos = 0;
	}
	int BottomCellPos = ((m_MapScroll.Y * -1) + DISPLAY_HEIGHT) / static_cast<int>(CELL_SIZE) + 1;
	int RightCellPos = ((m_MapScroll.X * -1) + DISPLAY_WIDTH) / static_cast<int>(CELL_SIZE) + 1;
	if (BottomCellPos > m_MapSizeY) {
		BottomCellPos = m_MapSizeY;
	}
	if (RightCellPos > m_MapSizeX) {
		RightCellPos = m_MapSizeX;
	}

	for (int j = TopCellPos; j < BottomCellPos;j++)
	{
		for (int i = LeftCellPos;i < RightCellPos;i++)
		{
			if (MapData[j][i] == 0)
			{
				continue;
			}
			CellInit();
			float top = FIELD_TOP + (CELL_SIZE * j) + static_cast<float>(m_MapScroll.Y);
			float left = FIELD_LEFT + (CELL_SIZE * i) + static_cast<float>(m_MapScroll.X);

			m_Cell[0].x = left;
			m_Cell[0].y = top;
			m_Cell[1].x = (left + CELL_SIZE);
			m_Cell[1].y = top;
			m_Cell[2].x = (left + CELL_SIZE);
			m_Cell[2].y = (top + CELL_SIZE);
			m_Cell[3].x = left;
			m_Cell[3].y = (top + CELL_SIZE);

			m_MapSelected = MapData[j][i];
			if (m_MapSelected < 100)
			{
				switch (m_MapSelected)
				{
				case WOOD_BLOCK:
				case ROCK_BLOCK:
				case WOOD_TRACT:
				case ROCK_TRACT:
					SetVertexUV(m_Cell, BLOCK_INTEGRATION_WIDTH * (m_MapSelected - 1.f), 0, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
					break;
				case GOAL_ZONE:
					m_Cell[0].x -= CELL_SIZE * 0.5f;
					m_Cell[0].y -= CELL_SIZE * 0.5f;
					m_Cell[1].x += CELL_SIZE * 0.5f;
					m_Cell[1].y -= CELL_SIZE * 0.5f;
					m_Cell[2].x += CELL_SIZE * 0.5f;
					m_Cell[3].x -= CELL_SIZE * 0.5f;
					SetVertexUV(m_Cell, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT * 3.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
					break;
				case SPEAR:
					SetVertexUV(m_Cell, BLOCK_INTEGRATION_WIDTH * 2.f, BLOCK_INTEGRATION_HEIGHT * 3.f, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT * 2.f);
					break;
				case DESCRIPTION_BOARD:
				case DESCRIPTION_BOARD2:
					SetVertexUV(m_Cell, 0.f, BLOCK_INTEGRATION_HEIGHT, 0.24f, BLOCK_INTEGRATION_HEIGHT*0.98f);
					m_Cell[1].x += CELL_SIZE * 3.f;
					m_Cell[2].x += CELL_SIZE * 3.f;
					m_Cell[3].y += CELL_SIZE * 2.f;
					m_Cell[2].y += CELL_SIZE * 2.f;
					break;
				default:
					continue;
				}
			}
			else {
				switch (m_MapSelected/100)
				{
				case WOOD_REVERSE_ZONE:
					SetVertexUV(m_Cell, BLOCK_INTEGRATION_WIDTH * 4.f, 0, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
					break;
				case ROCK_REVERSE_ZONE:
					SetVertexUV(m_Cell, BLOCK_INTEGRATION_WIDTH * 5.f, 0, BLOCK_INTEGRATION_WIDTH, BLOCK_INTEGRATION_HEIGHT);
					break;
#ifdef _DEBUG
				case 9: {
					CUSTOMVERTEX test[4]{ m_Cell[0],m_Cell[1],m_Cell[2],m_Cell[3] };
					for (int i = 0; i < 4; ++i) {
						test[i].color = 0xCCDDDDDD;
					}
					TextureRender("TEST_TEX", test); 
				}
#endif
				default:
					continue;
				}
			}
			if (m_isReversing) {
				m_Rad += 0.001f;
				if ((i + j) == 0) {
					RevolveY(m_Cell, m_Rad);
				}
				if (((i + j) % 2) == 0) {
					RevolveY(m_Cell, -m_Rad * 2);
				}
				else {
					RevolveY(m_Cell, m_Rad);
				}
				if (m_Rad > 5.f) {
					m_Rad = 0;
					m_isReversing = false;
				}
			}
			TextureRender("BLOCK_INTEGRATION_A_TEX", m_Cell);

		}
	}
	for (BaseTarget* pi : pBaseTarget)
	{
		pi->Render(m_MapScroll.Y, m_MapScroll.X, m_MapDataState);
	}
	
#ifdef _DEBUG
	RECT test = { 0,500,1250,700 };
	char TestText[ARRAY_LONG];
	sprintf_s(TestText, ARRAY_LONG, "MapScroll::X:%d,Y:%d", m_MapScroll.X, m_MapScroll.Y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
#endif
}

bool MapChip::Update() {
	if (m_MapScroll.Y > 0) {
		m_MapScroll.Y = 0;
	}
	for (auto ite : pBaseTarget)
	{
		ite->Update();
	}
	return true;
}

void MapChip::CellInit() {
	for (int i = 0; i < 4; i++) {
		m_Cell[i].z = 1.0f;
		m_Cell[i].rhw = 1.0f;
		m_Cell[i].color = 0xFFFFFFFF;
	}
	m_Cell[0].tv = 0.f;
	m_Cell[1].tv = 0.f;
	m_Cell[2].tv = BLOCK_INTEGRATION_WIDTH;
	m_Cell[3].tv = BLOCK_INTEGRATION_WIDTH;

}

int MapChip::SearchBlockX(BLOCKTYPE block) {
	for (int j = 0; j < m_MapSizeY; j++)
	{
		for (int i = 0; i < m_MapSizeX; i++)
		{
			if (MapData[j][i] == block)
			{
				return i;
			}
		}
	}
	return 2;
}

int MapChip::SearchBlockY(BLOCKTYPE block) {
	for (int j = 0; j < m_MapSizeY; j++)
	{
		for (int i = 0; i < m_MapSizeX; i++)
		{
			if (MapData[j][i] == block)
			{
				return j;
			}
		}
	}
	return 5;
}

CUSTOMVERTEX* MapChip::GetTargetPosition(int targetType)
{
	CUSTOMVERTEX* buf = NULL;
	for (auto ite : pBaseTarget)
	{
		BlockInfo* InfoBuf = ite->GetTargetInfo();
		if (InfoBuf->GimmickType!= targetType) {
			continue;
		}
		buf = ite->GetTargetPosition();
	}
	return buf;
}

float MapChip::GetGimmickPosition(bool isAxisX, int mapYPos, int mapXPos)
{
	float buf = 0;
	for (auto ite : pBaseTarget)
	{
		if (!ite->GetGimmickActive(mapXPos)) {
			continue;
		}
		buf = ite->GetGimmickPosition(isAxisX, m_MapDataState);
		if (buf != 0) break;
	}
	return buf;
}

bool MapChip::GetGimmckActive(int mapXPos)
{
	for (auto ite : pBaseTarget)
	{
		return ite->GetGimmickActive(mapXPos);
	}
	return false;
};

void MapChip::RevolveY(CUSTOMVERTEX* Vertex, float Rad) {

	float CharVertexX[4];

	CharVertexX[0] = Vertex[0].x;
	CharVertexX[1] = Vertex[1].x;
	CharVertexX[2] = Vertex[2].x;
	CharVertexX[3] = Vertex[3].x;

	float centralX = (Vertex[1].x - Vertex[0].x) * 0.5f + Vertex[0].x;

	for (int RoteCnt = 0; RoteCnt < 4; RoteCnt++) {

		CharVertexX[RoteCnt] -= centralX;
		Vertex[RoteCnt].z -= 1;

		float KEEPER = CharVertexX[RoteCnt];

		CharVertexX[RoteCnt] = (CharVertexX[RoteCnt] * cos(-Rad)) - (Vertex[RoteCnt].z * sin(-Rad));
		Vertex[RoteCnt].z = (Vertex[RoteCnt].z * cos(-Rad)) + (KEEPER * sin(-Rad));

		CharVertexX[RoteCnt] += centralX;
		Vertex[RoteCnt].z += 1;

	}

	Vertex[0] = { CharVertexX[0], Vertex[0].y, Vertex[0].z, 1.f, Vertex[0].color, Vertex[0].tu, Vertex[0].tv };
	Vertex[1] = { CharVertexX[1], Vertex[1].y, Vertex[1].z, 1.f, Vertex[1].color, Vertex[1].tu, Vertex[1].tv };
	Vertex[2] = { CharVertexX[2], Vertex[2].y, Vertex[2].z, 1.f, Vertex[2].color, Vertex[2].tu, Vertex[2].tv };
	Vertex[3] = { CharVertexX[3], Vertex[3].y, Vertex[3].z, 1.f, Vertex[3].color, Vertex[3].tu, Vertex[3].tv };

}
