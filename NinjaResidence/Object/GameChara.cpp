/**
* @file GameChara.cpp
* @brief GameCharaクラス
* @author Kojiro Kawahara
*/
#include "GameChara.h"

using  namespace MapBlock;
using namespace PlayerAnimation;

GameChara::GameChara(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* mapChip) :Object(pDirectX, pSoundOperater)
{
	m_Central = { 0.f,0.f,(CELL_SIZE * 2.f),(CELL_SIZE * 4.f) };

	//mapChipの情報を取得するために必要
	m_pMapChip = mapChip;
	m_MapSizeX = m_pMapChip->GetRow();
	m_MapSizeY = m_pMapChip->GetColunm();

	m_Central.x = static_cast<float>(m_pMapChip->SearchBlockX(START_ZONE))*CELL_SIZE;
	m_Central.y = static_cast<float>(m_pMapChip->SearchBlockY(START_ZONE))*CELL_SIZE;

	CreateSquareVertex(m_Central, m_WorldCoordinate, 0xFFFFFFFF, 0, 0, m_CollisionTu, m_CollisionTv);
	CreateSquareVertex(m_Central, m_DisplayCoordinate, 0xFFFFFFFF, 0, 0, m_CollisionTu, m_CollisionTv);

	m_MapScroll = MapChip::Scroll();

	int scrollXBuf = 0;
	int scrollYBuf = 0;
	int scrollBehindX = 0;
	int scrollBehindY = 0;

	do {
		scrollXBuf = m_MapScroll.X;
		scrollYBuf = m_MapScroll.Y;

		MapScrool();
		scrollBehindX = (m_MapScroll.X - scrollXBuf);
		scrollBehindY = (m_MapScroll.Y - scrollYBuf);

	} while (0 != scrollBehindX || 0 != scrollBehindY);
	MapChip::Scroll(m_MapScroll);
}

GameChara::~GameChara()
{
}

void GameChara::InitJumpParam() {
	m_isJump = false;
	m_isJumpRight = false;
	m_isJumpLeft = false;
	m_Acceleration.Y = INITIAL_ACCELERATION;
	m_RiseFlameTime = 0;
}

void GameChara::AccelarationControl() {
	m_Acceleration.Y -= CELL_SIZE / 20.f;

}

bool GameChara::PermitJumping() {
	//壁ジャンプ判定の為
	UpdateMapPos();

	m_MapPosition.Left -= 1;
	m_MapPosition.Right += 1;
	m_HeldOntoWallLeft = !LeftCollisionBlock(NONE) && (LeftCollisionBlock(START_ZONE) || !LeftCollisionBlock(START_ZONE));
	m_HeldOntoWallRight = !RightCollisionBlock(NONE) && (RightCollisionBlock(START_ZONE) || !RightCollisionBlock(START_ZONE));
	m_MapPosition.Left += 1;
	m_MapPosition.Right -= 1;

	if (!m_isInTheAir) {
		m_isInertiaMoving = false;
		m_pSoundOperater->Start("JUMP", false);
		return true;
	}
	else if (m_HeldOntoWallLeft || m_HeldOntoWallRight) {
		m_isInertiaMoving = false;
		m_pSoundOperater->Start("JUMP", false);
		return true;
	}
	return false;
}

void GameChara::Jump()
{
	if (!m_isJump) {
		return;
	}
	m_Central.y -= m_Acceleration.Y;
	TopCollision();
	SideCollision();

	if (m_Central.y - m_Central.scaleY < 0) {
		m_Central.y = m_Central.scaleY;
		m_isInertiaMoving = true;
		InitJumpParam();
		return;
	}
	if (!m_isUsingArt) {
		m_ChangeAnimation = JUMPING;
	}
	m_TurnAnimation = 0.f;
	static int animeCount = 0;
	++animeCount;
	if (animeCount > 3) {
		m_TurnAnimation = 1.f;
	}
	if (m_RiseFlameTime < 15) {
		JumpingLateralMotion();
		m_RiseFlameTime++;
		AccelarationControl();
		UpdateMapPos();

		return;
	}
	animeCount = 0;
	AddGravity();
	InitJumpParam();
}

void GameChara::JumpingLateralMotion() {
	if (!m_isInTheAir) {
		m_HeldOntoWallLeft = m_HeldOntoWallRight = false;
		return;
	}
	if (m_isDash) return;

	if (m_HeldOntoWallLeft) {
		m_isJumpRight = true;
	}
	if (m_HeldOntoWallRight) {
		m_isJumpLeft = true;
	}
	if (m_isJumpRight) {
		//右に移動
		m_Central.x += m_Acceleration.X;
		m_Facing = FACING_RIGHT;
		m_DirectionBias = ZERO;

		SideCollision();
		TopCollision();
	}
	if (m_isJumpLeft) {
		//左に移動
		m_Central.x -= m_Acceleration.X;
		m_Facing = FACING_LEFT;
		m_DirectionBias = ONE;

		SideCollision();
		TopCollision();

	}
}

void GameChara::MoveOperation(KeyDirection vec)
{
	switch (vec)
	{
		//上に移動
	case JUMP:

		m_isJump = PermitJumping();
		break;
	case MOVE_RIGHT:
		//右に移動
		m_Facing = FACING_RIGHT;
		m_DirectionBias = ZERO;
		Dash();
		break;
	case MOVE_LEFT:
		//左に移動
		m_Facing = FACING_LEFT;
		m_DirectionBias = ONE;
		Dash();
		break;
	case INERTIA:
		m_isInertiaMoving = true;
		break;
	}
}

void GameChara::Dash()
{
	static int animeCount = 0;
	m_isDash = true;
	m_Central.x += MOVE_SPEED * static_cast<float>(m_Facing);
	SideCollision();
	if (m_isInTheAir) return;
	if (m_ChangeAnimation == WATER_ART) {
		m_TurnAnimation = 3.f;
		return;
	}
	if (SoundLib::Playing != m_pSoundOperater->GetStatus("DASH")) {
		m_pSoundOperater->Start("DASH");
	}
	m_ChangeAnimation = DASH;
	++animeCount;
	if (animeCount > ANIMATION_INTERVAL) {
		TurnTheAnimation(8);
		animeCount = 0;
	}

}

void GameChara::PrevSaveMapPos()
{
	m_PrevPosition.Left = m_Central.x;
	m_PrevPosition.Right = m_Central.x+ m_Central.scaleX;
	m_PrevPosition.Y = m_Central.y;
}

void GameChara::KeyOperation(KeyDirection vec)
{
	if (m_isReversing) return;
	m_isFire = false;
	m_isUsingArt = false;
	m_isDash = false;
	//Key操作での処理
	switch (vec)
	{
	case JUMP:
		MoveOperation(JUMP);
		break;
	case DOWN:
		//今のところ無し
		break;
	case MOVE_RIGHT:
		//右に移動
		MoveOperation(MOVE_RIGHT);
		break;
	case MOVE_LEFT:
		//左に移動
		MoveOperation(MOVE_LEFT);
		break;
	case PUSH_NONE:
		NoOperation();
		break;
	case THROW:
		m_ChangeAnimation = THROWING;
		m_isUsingArt = true;
		break;
	case FIRE:
		m_ChangeAnimation = FIREART;
		m_isUsingArt = true;
		m_isFire = true;
		break;
	case INERTIA:
		m_isInertiaMoving = true;
		break;
	case MAP_RIGHT:
		if (m_Central.x + m_MapScroll.X > SCROLL_LEFT_RANGE)
		{
			m_MapScroll.X -= 5;
		}
		MapChip::Scroll(m_MapScroll);
		break;
	case MAP_LEFT:
		if (m_MapScroll.X >= 0) {
			m_MapScroll.X = 0;
			break;
		}
		if (m_Central.x + m_Central.scaleX + m_MapScroll.X < SCROLL_RIGHT_RANGE)
		{
			m_MapScroll.X += 5;
		}
		MapChip::Scroll(m_MapScroll);
		break;
	case MAP_DOWN:
		DownScrollOparation();
		MapChip::Scroll(m_MapScroll);
		break;
	case MAP_UP:
		UpScrollOparation();
		MapChip::Scroll(m_MapScroll);
		break;
	}
}

void GameChara::DownScrollOparation()
{
	if (m_Central.y+m_MapScroll.Y <= SCROLL_DOWN_RANGE)
	{
		m_MapScroll.Y -= 5;
		if (m_MapScroll.Y <= static_cast<int>((CELL_SIZE * -m_MapSizeY))) {
			m_MapScroll.Y = static_cast<int>((CELL_SIZE*-m_MapSizeY));
		}
		if (m_Central.y - m_Central.scaleY + m_MapScroll.Y > DISPLAY_HEIGHT)
		{
			return;
		}
		//スクロールの制限
		if (m_Central.y - m_Central.scaleY + m_MapScroll.Y < static_cast<float>(SCROLL_UP_RANGE) + VERTICAL_SCROLLING_LEVEL)
		{
			m_MapScroll.Y += 5;
		}
	}
}

void GameChara::UpScrollOparation() {
	if (m_Central.y + m_MapScroll.Y >= SCROLL_UP_RANGE)
	{
		m_MapScroll.Y += 5;
		if (m_MapScroll.Y >= 0) {
			m_MapScroll.Y = 0;
		}
		if (m_Central.y + m_MapScroll.Y < 0)
		{
			return;
		}	
		//スクロールの制限
		if (m_Central.y + m_MapScroll.Y > static_cast<float>(SCROLL_DOWN_RANGE) - VERTICAL_SCROLLING_LEVEL)
		{
			m_MapScroll.Y -= 5;
		}
	}
}

void GameChara::NoOperation() {
	if (DownCollisionAnything()) {
		m_ChangeAnimation = STAND;
		static int animeCount = 0;
		++animeCount;
		if (animeCount > 10) {
			TurnTheAnimation(3);
			animeCount = 0;
		}

	}

}

void GameChara::MapReversePointSearch(int pairNumber, MapDataState mapState)
{
	int BlockY = 0;
	int BlockX = 0;
	MapScroll ScrollBuf = {};
	MapScroll ScrollBehind = {};
	for (unsigned int i = 0; i < m_ReversePoint.size(); ++i) {
		bool isSameMapState = m_ReversePoint[i].MapDataState == mapState;
		bool isSamePair = pairNumber == m_ReversePoint[i].PairNumber;
		if (isSameMapState && isSamePair) {
			BlockY = m_ReversePoint[i].PositionY;
			BlockX = m_ReversePoint[i].PositionX;
			break;
		}
	}

	m_Central = { (BlockX * CELL_SIZE), (BlockY * CELL_SIZE) ,m_Central.scaleX,m_Central.scaleY };
	UpdateMapPos();
	m_MapScroll = MapChip::Scroll();
	do {
		ScrollBuf = m_MapScroll;

		MapScrool();
		SetGround();
		ScrollBehind = { (m_MapScroll.X - ScrollBuf.X), (m_MapScroll.Y - ScrollBuf.Y) };

	} while (0 != ScrollBehind.X || 0 != ScrollBehind.Y);
	MapChip::Scroll(m_MapScroll);
}

void GameChara::Reverce(MapChip* mapChip, int pairNumber)
{
	m_pMapChip = mapChip;
	m_MapSizeY = m_pMapChip->GetColunm();
	m_MapSizeX = m_pMapChip->GetRow();
	MapReversePointSearch(pairNumber,m_pMapChip->GetMapDataState());
}

void GameChara::UpdateMapPos()
{
	m_DisplayCoordinate[0].y = m_Central.y + m_MapScroll.Y - m_Central.scaleY;
	m_DisplayCoordinate[0].x = m_Central.x + m_MapScroll.X;
	m_DisplayCoordinate[1].y = m_Central.y + m_MapScroll.Y - m_Central.scaleY;
	m_DisplayCoordinate[1].x = m_Central.x + m_MapScroll.X + m_Central.scaleX;
	m_DisplayCoordinate[2].y = m_Central.y + m_MapScroll.Y;
	m_DisplayCoordinate[2].x = m_Central.x + m_MapScroll.X + m_Central.scaleX;
	m_DisplayCoordinate[3].y = m_Central.y + m_MapScroll.Y;
	m_DisplayCoordinate[3].x = m_Central.x + m_MapScroll.X;

	m_MapPosition.Left = static_cast<int>(m_Central.x / CELL_SIZE);
	m_MapPosition.Right = static_cast<int>((m_Central.x + m_Central.scaleX*0.5f) / CELL_SIZE);
	m_MapPosition.Y = static_cast<int>((m_Central.y + 10.f) / CELL_SIZE);
	if (m_MapPosition.Y >= m_MapSizeY) {
		m_MapPosition.Y = m_MapSizeY - 1;
	}
}

void GameChara::MapScrool()
{
	//下にスクロール移動
	if (m_Central.y + m_MapScroll.Y > static_cast<float>(SCROLL_DOWN_RANGE))
	{
		if (m_Central.y > m_MapSizeY*CELL_SIZE) {
			return;
		}
		m_MapScroll.Y -= VERTICAL_SCROLLING_LEVEL;
		UpdateMapPos();
		m_isScrollingDown = true;
	}
	else m_isScrollingDown = false;
	//上にスクロール移動
	if (m_Central.y + m_MapScroll.Y < static_cast<float>(SCROLL_UP_RANGE))
	{
		if (m_Central.y >= static_cast<float>(SCROLL_UP_RANGE))
		{
			m_MapScroll.Y += VERTICAL_SCROLLING_LEVEL;
		}
	}

	//右にスクロール移動
	if (m_Central.x + m_Central.scaleX + m_MapScroll.X > static_cast<float>(SCROLL_RIGHT_RANGE))
	{
		if ((m_Central.x + m_Central.scaleX <= (((m_MapSizeX-1) * CELL_SIZE) - static_cast<float>(SCROLL_VARTICAL_RANGE))) ||
			(m_Central.x + m_Central.scaleX > DISPLAY_WIDTH))
		{
			m_MapScroll.X -= SCROLL_SPEED;
		}
	}
	//左にスクロール移動
	if (m_Central.x + m_MapScroll.X < static_cast<float>(SCROLL_LEFT_RANGE))
	{
		if (m_Central.x >= static_cast<float>(SCROLL_VARTICAL_RANGE) || (m_Central.x<0))
		{
			m_MapScroll.X += SCROLL_SPEED;
		}
	}
}

void GameChara::TurnTheAnimation(int AnimationPage)
{
	if (m_TurnAnimation < AnimationPage - 1) {
		m_TurnAnimation += 1.f;
	}
	else m_TurnAnimation = 0;
}

void GameChara::ThrowAnime() {
	static bool AnimeOn = false;
	static int AnimationCount = 0;

	if (m_ChangeAnimation == THROWING) {

		AnimeOn = true;
	}
	if (AnimeOn) {
		AnimationCount++;
		static int animeCount = 0;
		++animeCount;
		if (animeCount > ANIMATION_INTERVAL) {
			TurnTheAnimation(6);
			animeCount = 0;
		}
	}
	else return;

	if (m_TurnAnimation!=0) {
		m_ChangeAnimation = THROWING;
	}
	else {
		m_pSoundOperater->Start("SHURIKEN", false);
		AnimationCount = 0;
		AnimeOn = false;
		m_ChangeAnimation = STAND;
	}
}

void GameChara::FireArtAnime() {
	static bool AnimeOn = false;

	if (m_isFire && m_ChangeAnimation == FIREART) {
		static int animeCount = 0;
		++animeCount;
		if (animeCount <= ANIMATION_INTERVAL) return;
		animeCount = 0;
		if (m_TurnAnimation <= ANIMATION_INTERVAL) return;
		static bool anime = true;
		m_TurnAnimation += ((anime) ? 0 : -1);
		anime = !anime;
	}
	else m_TurnAnimation++;
}

bool GameChara::Update()
{
	m_MapScroll = MapChip::Scroll();
	ThrowAnime();
	UpdateMapPos();
	AddGravity();
	MapScrool();
	SideCollision();
	if (!(m_isInTheAir = !SetGround()) && TopCollision()) {
		SideCollision();
		SetGround();
	}
	if (m_isInertiaMoving) {
		MoveInertia();
	}
	if (!m_CollisionHead) {
		Jump();
		MapScrool();
		SideCollision();
		TopCollision();
		SetGround();
	}
	else {
		m_isInertiaMoving = true;
		InitJumpParam();
	}
	MapChip::Scroll(m_MapScroll);
	return CollisionIventBlock();
}

bool GameChara::CollisionIventBlock() {
	m_MapPosition.Left -= 1;
	m_MapPosition.Right += 1;
	if (LeftCollisionBlock(GOAL_ZONE) || TopCollisionBlock(GOAL_ZONE) ||
		DownCollisionBlock(GOAL_ZONE) || RightCollisionBlock(GOAL_ZONE)) {
		m_MapPosition.Left += 1;
		m_MapPosition.Right -= 1;
		return true;
	}
	if (FailureGame()) {
		m_GameFailure = true;
	}
	m_MapPosition.Left += 1;
	m_MapPosition.Right -= 1;

	return false;
}

bool GameChara::DownCollisionAnything() {
	int D_Buf = 0;

	if (m_MapPosition.Y < 0 && m_MapPosition.Left < 0) {
		return false;
	}

	if (DownCollisionBlock(START_ZONE)) {
		return false;
	}

	if (DownCollisionBlock(DESCRIPTION_BOARD)) {
		return false;
	}

	//松明
	bool TorchMin = ((D_Buf = m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) >= 400) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) >= 400) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) >= 400));
	bool TorchMax = ((m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) < 500) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) < 500) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) < 500));
	if (TorchMax && TorchMin&&D_Buf > NOMAL_BLOCK_MAX) {
		return false;
	}

	//的
	bool TargetMin = ((m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) >= NOMAL_BLOCK_MAX) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) >= NOMAL_BLOCK_MAX) ||
		((D_Buf = m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right)) >= NOMAL_BLOCK_MAX));
	bool TargetMax = ((m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) < 300) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) < 300) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) < 300));
	if (TargetMax && TargetMin&&D_Buf > NOMAL_BLOCK_MAX && D_Buf <1100) {
		return false;
	}

	//どんでん返しゾーン
	bool ReverseCollLeft = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) < 1300) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) >= 1100);
	bool ReverseCollCenter = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) < 1300) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) >= 1100);
	bool ReverseCollRight = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) < 1300) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) >= 1100);
	
	//基本ブロック
	bool CollLeft = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) < NOMAL_BLOCK_MAX)&& (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) > 0);
	bool CollCenter = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left+1) < NOMAL_BLOCK_MAX)&& (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left+1) > 0);
	bool CollRight =(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) < NOMAL_BLOCK_MAX)&& (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) > 0);
	if ((CollLeft || CollRight || CollCenter)|| (ReverseCollLeft || ReverseCollRight || ReverseCollCenter)){
		return true;
	}

	//水面
	CollLeft = ((m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) < 800) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) >= 700));
	CollRight = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) < 800) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) >= 700);
	CollCenter = (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) < 800) && (m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) >= 700);
	if (CollLeft || CollRight || CollCenter) {
		m_ChangeAnimation = WATER_ART;
		if (m_isInTheAir) {
			m_pSoundOperater->Start("SET_DOWN_WATER", false);
		}
		return true;
	}

	if (DownCollisionBlock(NONE)) {
		return false;
	}

	return true;
}

bool GameChara::DownCollisionBlock(int block) {
	if ((m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Left + 1) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y, m_MapPosition.Right) == block)) {
		return true;
	}
	return false;
}

bool GameChara::TopCollisionBlock(int block) {
	if ((m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left + 1) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Right) == block)) {
		return true;
	}
	return false;
}

bool GameChara::LeftCollisionBlock(int block) {
	if ((m_pMapChip->GetMapData(m_MapPosition.Y - 1, m_MapPosition.Left) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 2, m_MapPosition.Left) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 3, m_MapPosition.Left) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 4, m_MapPosition.Left) == block)) {
		return true;
	}
	return false;
}

bool GameChara::RightCollisionBlock(int block) {
	if (m_MapPosition.Right >= m_MapSizeX) {
		m_MapPosition.Right -= 1;
	}
	if ((m_pMapChip->GetMapData(m_MapPosition.Y - 1, m_MapPosition.Right) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 2, m_MapPosition.Right) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 3, m_MapPosition.Right) == block) ||
		(m_pMapChip->GetMapData(m_MapPosition.Y - 4, m_MapPosition.Right) == block)) {
		return true;
	}
	return false;
}

bool GameChara::LeftDirectionCollision() {
	m_MapPosition.Left -= 1;
	while (m_MapPosition.Left < 0) {
		m_MapPosition.Left = 0;
	}

	int HeadBlock=(m_pMapChip->GetMapData(m_MapPosition.Y - 4, m_MapPosition.Left));
	int BodyBlock=(m_pMapChip->GetMapData(m_MapPosition.Y - 3, m_MapPosition.Left));
	int LegBlock=(m_pMapChip->GetMapData(m_MapPosition.Y - 2, m_MapPosition.Left));
	int FootBlock=(m_pMapChip->GetMapData(m_MapPosition.Y - 1, m_MapPosition.Left));

	bool ColHead = (HeadBlock != NONE && HeadBlock != START_ZONE && HeadBlock != DESCRIPTION_BOARD);
	bool ColBody = (BodyBlock != NONE && BodyBlock != START_ZONE && BodyBlock != DESCRIPTION_BOARD);
	bool ColLeg = (LegBlock != NONE && LegBlock != START_ZONE && LegBlock != DESCRIPTION_BOARD);
	bool ColFoot = (FootBlock != NONE && FootBlock != START_ZONE && FootBlock != DESCRIPTION_BOARD);
	if (ColHead || ColBody || ColLeg || ColFoot) {
		return false;
	}
	return true;
}

bool GameChara::RightDirectionCollision() {
	m_MapPosition.Right += 1;
	int HeadBlock = (m_pMapChip->GetMapData(m_MapPosition.Y - 4, m_MapPosition.Right));
	int BodyBlock = (m_pMapChip->GetMapData(m_MapPosition.Y - 3, m_MapPosition.Right));
	int LegBlock = (m_pMapChip->GetMapData(m_MapPosition.Y - 2, m_MapPosition.Right));
	int FootBlock = (m_pMapChip->GetMapData(m_MapPosition.Y - 1, m_MapPosition.Right));

	bool ColHead = (HeadBlock != NONE && HeadBlock != START_ZONE && HeadBlock != DESCRIPTION_BOARD);
	bool ColBody = (BodyBlock != NONE && BodyBlock != START_ZONE && BodyBlock != DESCRIPTION_BOARD);
	bool ColLeg = (LegBlock != NONE && LegBlock != START_ZONE && LegBlock != DESCRIPTION_BOARD);
	bool ColFoot = (FootBlock != NONE && FootBlock != START_ZONE && FootBlock != DESCRIPTION_BOARD);
	if (ColHead || ColBody || ColLeg || ColFoot) {
		return false;
	}
	return true;
}

bool GameChara::LookDownWater() {
	bool buf = false;
	for (int i = 0; i < m_MapSizeY - m_MapPosition.Y - 1; ++i) {
		if (!buf) {
			// 下にブロックがあるかの確認
			int MapdateBuf = m_pMapChip->GetMapData(m_MapPosition.Y + i, m_MapPosition.Left);
			if (MapdateBuf > 0 && MapdateBuf < NOMAL_BLOCK_MAX && MapdateBuf!= SPEAR&& MapdateBuf!= STAGE_DROP_ZONE) {
				return false;
			}
			buf = (BT_WATER == (MapdateBuf / NOMAL_BLOCK_MAX));
		}
	}
	return buf;
}

float GameChara::WaterCollsionCheck()
{
	float PosY = 0;
	if (PosY = m_pMapChip->GetGimmickPosition(false, m_MapPosition.Y, m_MapPosition.Left)) {
		return PosY;
	}
	else return m_Central.y;
}

bool GameChara::FailureGame()
{

	if (LeftCollisionBlock(SPEAR)|| TopCollisionBlock(SPEAR) || DownCollisionBlock(SPEAR) || RightCollisionBlock(SPEAR)) {
		return true;
	}
	if (LeftCollisionBlock(STAGE_DROP_ZONE) || TopCollisionBlock(STAGE_DROP_ZONE) || DownCollisionBlock(STAGE_DROP_ZONE) || RightCollisionBlock(STAGE_DROP_ZONE)) {
		return true;
	}

	return false;
}

void GameChara::Render()
{
	CreateSquareVertex(m_Central, m_WorldCoordinate, 0xFFFFFFFF, 0, 0, m_CollisionTu, m_CollisionTv);
	UpdateMapPos();
#ifdef _DEBUG
	CUSTOMVERTEX DebugColl[4];
	m_DisplayCoordinate[0].color = m_DisplayCoordinate[1].color = m_DisplayCoordinate[2].color = m_DisplayCoordinate[3].color = 0xFF010101;

	TextureRender("TEST_TEX", m_DisplayCoordinate);
	DebugColl[3].y = DebugColl[2].y = m_MapPosition.Y * CELL_SIZE + m_MapScroll.Y;
	DebugColl[0].y = DebugColl[1].y = DebugColl[3].y - CELL_SIZE;
	DebugColl[3].x = DebugColl[0].x = m_MapPosition.Left * CELL_SIZE + m_MapScroll.X;
	DebugColl[1].x = DebugColl[2].x = m_MapPosition.Right * CELL_SIZE + m_MapScroll.X;
	DebugColl[0].color = DebugColl[1].color = DebugColl[2].color = DebugColl[3].color = 0xFFEFEFEF;
	TextureRender("TEST_TEX", DebugColl);
	DebugColl[3].y = DebugColl[2].y = (m_MapPosition.Y - 3) * CELL_SIZE + m_MapScroll.Y;
	DebugColl[0].y = DebugColl[1].y = DebugColl[3].y - CELL_SIZE;
	DebugColl[3].x = DebugColl[0].x = m_MapPosition.Left * CELL_SIZE + m_MapScroll.X;
	DebugColl[1].x = DebugColl[2].x = m_MapPosition.Right * CELL_SIZE + m_MapScroll.X;
	DebugColl[0].color = DebugColl[1].color = DebugColl[2].color = DebugColl[3].color = 0xFFE0E0E0;
	TextureRender("TEST_TEX", DebugColl);

#endif
	CUSTOMVERTEX Chara[4];
	CENTRAL_STATE CharCentral = { 0 };
	TranslateCentral_State(&CharCentral, m_DisplayCoordinate);
	CharCentral.x -= 25.f;
	CharCentral.scaleX = 120.f;
	CreateSquareVertex(CharCentral, Chara, 0xFFFFFFFF,( m_TurnAnimation+m_DirectionBias) * TU, m_ChangeAnimation * TV, TU * m_Facing, TV);
	TextureRender("CHARA_TEX", Chara);

#ifdef _DEBUG

	RECT test = { 0,0,1250,500 };
	char TestText[ARRAY_LONG];
	sprintf_s(TestText, ARRAY_LONG, "MapChara::X-L:%d,X-R:%d,Y:%d", m_MapPosition.Left, m_MapPosition.Right, m_MapPosition.Y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
	sprintf_s(TestText, ARRAY_LONG, "\nPrevMapChara::X-L:%.2f,X-R:%.2f,Y:%.2f", m_PrevPosition.Left, m_PrevPosition.Right, m_PrevPosition.Y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
	sprintf_s(TestText, ARRAY_LONG, "\n\nWorldChara::X-L:%.2f,X-R:%.2f,Y:%.2f", m_WorldCoordinate[3].x, m_WorldCoordinate[2].x, m_WorldCoordinate[3].y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
	sprintf_s(TestText, ARRAY_LONG, "\n\n\nDisplayChara::X-L:%.2f,X-R:%.2f,Y3:%.2f,Y0:%.2f", m_DisplayCoordinate[3].x, m_DisplayCoordinate[2].x, m_DisplayCoordinate[3].y, m_DisplayCoordinate[0].y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
	sprintf_s(TestText, ARRAY_LONG, "\n\n\n\n\n\nAccelerationY::%.2f", m_Acceleration.Y);
	m_pDirectX->DrawWord(test, TestText, "DEBUG_FONT", DT_RIGHT, 0xffffffff);
#endif

}

void GameChara::AddGravity() {
	if (m_Central.y > m_MapSizeY*CELL_SIZE) {
		return;
	}
	//重力を毎フレームかける
	if (m_isScrollingDown) {
		m_Central.y += VERTICAL_SCROLLING_LEVEL;
		SideCollision();
		SetGround();
		return;
	}
	m_Central.y += GRAVITY + m_GravityAcceleration;
	SideCollision();
	SetGround();
}

void GameChara::DebugMove() {
	m_Central.y -= MOVE_SPEED * 1.5f;
}

float GameChara::GetPositionX()
{
	switch (m_Facing) {
	case FACING_LEFT:
		return m_Central.x + static_cast<float>(m_MapScroll.X);
	case FACING_RIGHT:
		return m_Central.x + m_Central.scaleX + static_cast<float>(m_MapScroll.X);
	}
	return 0.f;
}

bool GameChara::SetGround() {
	static int animeCount = 0;

	if (DownCollisionAnything())
	{
		m_Central.y = ((m_MapPosition.Y) * CELL_SIZE);
		if (m_isInTheAir) {
			m_TurnAnimation = 0.f;
			m_pSoundOperater->Start("SET_DOWN", false);
		}
		return true;
	}
	else if (LookDownWater()) {
		if (!m_isJump) {
			float WaterUpperLevel = WaterCollsionCheck();
			if (m_Central.y +m_MapScroll.Y < WaterUpperLevel) {
				return false;
			}
			m_Central.y = WaterUpperLevel - m_MapScroll.Y;
			if (m_isInTheAir) {
				m_pSoundOperater->Start("SET_DOWN_WATER", false);
			}
		}
		m_ChangeAnimation = WATER_ART;
		static int animeCount = 0;
		++animeCount;
		if (animeCount > 10) {
			TurnTheAnimation(3);
			animeCount = 0;
		}
		return true;
	}
	else if (!m_isJump && !m_isUsingArt) {
		m_ChangeAnimation = JUMPING;
		if (animeCount % 3) {
			m_TurnAnimation = (animeCount % ANIMATION_INTERVAL) ? 4.0f : 5.0f;
		}
		++animeCount;
		if (animeCount > 1000) {
			animeCount = 0;
		}

	}
	else if (m_isJump && !m_isUsingArt) {
		m_ChangeAnimation = JUMPING;
	}

	return false;
}

bool GameChara::TopCollision() {
	if (m_PrevPosition.Y > m_Central.y)
	{
		UpdateMapPos();
		bool CollLeft = (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left) < NOMAL_BLOCK_MAX)
			&& (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left) > 0)
			&& START_ZONE != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left)
			&& DESCRIPTION_BOARD != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left);

		bool CollCenter = (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left + 1) < NOMAL_BLOCK_MAX)
			&& (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left + 1) > 0)
			&& START_ZONE != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left + 1)
			&& DESCRIPTION_BOARD != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Left + 1);

		bool CollRight = (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Right) < NOMAL_BLOCK_MAX)
			&& (m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Right) > 0)
			&& START_ZONE != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Right)
			&& DESCRIPTION_BOARD != m_pMapChip->GetMapData(m_MapPosition.Y - CHARA_HEIGHT, m_MapPosition.Right);

		if ((CollLeft || CollRight || CollCenter)) {
			m_CollisionHead = true;
			m_isJump = false;
			m_Central.y = m_PrevPosition.Y;
			m_Acceleration.Y = INITIAL_ACCELERATION;
			return true;
		}
	}
	m_CollisionHead = false;
	return false;
}

bool GameChara::LeftCollision() {
	//左の方向のブロックを確かめる
	if (!LeftDirectionCollision())
	{
		float BehindLeftDirection = m_Central.x;
		if (m_PrevPosition.Left != m_Central.x)
		{
			m_Central.x = ((m_MapPosition.Right - 1)* CELL_SIZE);
		}
		if (BehindLeftDirection == m_Central.x) {
			return false;
		}
		return true;
	}
	return false;
}

bool GameChara::RightCollision() {
	//右方向のブロックを確かめる
	if (!RightDirectionCollision())
	{
		float BehindRightDirection = m_Central.x;
		if (m_PrevPosition.Right != m_Central.x)
		{
			m_Central.x = ((m_MapPosition.Left)* CELL_SIZE);
		}
		if (BehindRightDirection == m_Central.x) {
			return false;
		}
		return true;
	}
	return false;
}

void GameChara::SideCollision() {
	if (m_PrevPosition.Left > m_Central.x)
	{
		while(LeftCollision());
	}
	if (m_PrevPosition.Right < m_Central.x + m_Central.scaleX)
	{
		while (RightCollision());
	}
	UpdateMapPos();
}

void GameChara::MoveInertia() {
	static int InertiaTime = 0;
	int InertiaTimeMax = 10;
	if (!m_isJump) {
		++InertiaTime;
		m_ChangeAnimation = STAND;
	}
	m_Central.x += MOVE_SPEED * static_cast<float>(m_Facing);
	SideCollision();
	if (!m_isInTheAir) {
		m_TurnAnimation = 0.f;
		InertiaTimeMax = 5;
	}
	if (InertiaTime > InertiaTimeMax) {
		m_isInertiaMoving = false;
		InertiaTime = 0;
	}
}
