﻿/**
* @file GameChara.h
* @brief GameCharaクラス
* @author Kojiro Kawahara
*/
#pragma once

#include <vector>
#include "Object/Object.h"
#include "Object/MapChip.h"

class GameScene;
class MapReverse;
/**
* @namespace PlayerAnimation
* @author Toshiya Matsuoka
*/
namespace PlayerAnimation {
	enum MOTION {
		STAND,
		DASH,
		JUMPING,
		THROWING,
		WATER_ART,
		//! 壁掴まり
		WALLHOLD,
		FIREART,
		ROPEHOLD,
	};
	enum DIRECTION {
		FACING_NOTHING = 0,
		//! param 1
		FACING_RIGHT = 1,
		//! param -1
		FACING_LEFT = -1,
	};
	//! アニメ用の仮列挙
	enum DIRECTION_BIAS {
		ZERO,
		ONE,
	};
};

class GameChara :public Object
{
public:
	void KeyOperation(KeyDirection vec);
	bool Update();
	void Render();
	GameChara(DirectX* pDirectX, SoundOperater* pSoundOperater, MapChip* mapChip);
	~GameChara();

	/**
	* @breaf どんでん返し処理
	* @author Toshiya Matsuoka
	*/
	void Reverce(MapChip* mapChip, int blockNumber);

	/**
	* @brief 前フレームの位置保存
	* @author Toshiya Matsuoka
	*/
	void PrevSaveMapPos();

	/**
	* @brief マップ上の左側のX座標取得
	*/
	int GetMapLeftDirectionPosition() { return m_MapPosition.Left; }

	/**
	* @brief マップ上のY座標取得
	*/
	int GetMapPositionY() { return m_MapPosition.Y; }

	/**
	* @brief ゲームの終了イベントブロックとの接触判定
	* @return 接触していればtrue
	* @author Toshiya Matsuoka
	*/
	bool CollisionIventBlock();

	/**
	* @brief 下へのスクロール操作
	* @author Toshiya Matsuoka
	*/
	void DownScrollOparation();

	/**
	* @brief 上へのスクロール操作
	* @author Toshiya Matsuoka
	*/
	void UpScrollOparation();

	/**
	* @brief 無操作時動作
	* @author Toshiya Matsuoka
	*/
	void NoOperation();

	/**
	* @brief Debug用キャラ上昇処理
	* @author Toshiya Matsuoka
	*/
	void DebugMove();

	/**
	* @brief キャラのディスプレイ上前方の取得
	* @author Toshiya Matsuoka
	*/
	float GetPositionX();
	/**
	* @brief キャラのディスプレイ上中心Y座標の取得
	* @author Toshiya Matsuoka
	*/
	float GetPositionY() { return m_Central.y - (m_Central.scaleY/2.f) + static_cast<float>(m_MapScroll.Y);}

	/**
	* @brief キャラの向いている方向の取得
	* @sa PlayerAnimation::DIRECTION
	* @author Toshiya Matsuoka
	*/
	PlayerAnimation::DIRECTION GetFacing() {return m_Facing;}

	/**
	* @brief 火遁アニメーション動作
	* @author Toshiya Matsuoka
	*/
	void FireArtAnime();

	/**
	* @brief ゲーム失敗フラグの取得
	* @author Toshiya Matsuoka
	*/
	bool GetGameFailure() {
		return m_GameFailure;
	}


private:

	struct MapPosition {
		int Left, Right, Y;
	};
	struct Pos3Way {
		float Left = 0;
		float Right = 0;
		float Y = 0;
	};

	MapScroll m_MapScroll;
	Pos3Way m_PrevPosition;

	MapPosition m_MapPosition;

	//! 毎フレームかける重力の値
	const float GRAVITY = CELL_SIZE * 0.375f;
	//! ダッシュ動作
	void Dash();
	//! 移動量
	const float MOVE_SPEED = CELL_SIZE * 0.375f;
	const int VERTICAL_SCROLLING_LEVEL = 20;
	const int SCROLL_SPEED = 15;
	bool m_isDash = false;
	//両端からのX座標の稼働範囲
	const int SCROLL_VARTICAL_RANGE = 300;
	//m_DisplayCoordinateのY座標がこの値を下回ると上にスクロールする
	const int SCROLL_UP_RANGE = 150;
	//m_DisplayCoordinateのY座標がこの値を超えると上にスクロールする
	const int SCROLL_DOWN_RANGE = 630;
	//m_DisplayCoordinateのX座標がこの値を下回ると左にスクロールする
	const int SCROLL_LEFT_RANGE = SCROLL_VARTICAL_RANGE;
	//m_DisplayCoordinateのX座標がこの値を超えると右にスクロールする
	const int SCROLL_RIGHT_RANGE = DISPLAY_WIDTH - SCROLL_VARTICAL_RANGE;
	//! 慣性移動の許可
	bool m_isInertiaMoving = false;

	void MapReversePointSearch(int pairNumber, MapDataState mapState);
	//! 当たり判定描画用
	float m_CollisionTu = 80 / 512.f;
	//! 当たり判定描画用
	float m_CollisionTv = 160 / 512.f;

	MapReverse* m_pMapReverse = NULL;
	MapChip* m_pMapChip = NULL;

	float m_GravityAcceleration = 0;

	//! ディスプレイ上のキャラの矩形
	CUSTOMVERTEX m_DisplayCoordinate[4];
	//! 全体的なキャラの矩形
	CUSTOMVERTEX m_WorldCoordinate[4];

	//仮統合ファイルの切り取り情報
	const float CHARA_TEXTURE_WIDTH = 4096.f;
	//! キャラ統合画像に対する1枚の幅
	const float TU = 320.f / CHARA_TEXTURE_WIDTH;
	//! キャラ統合画像に対する1枚の高さ
	const float TV = 321.f / CHARA_TEXTURE_WIDTH;
	float m_TurnAnimation = 0;
	//! キャラのTvを変更するための値
	PlayerAnimation::MOTION m_ChangeAnimation = PlayerAnimation::STAND;
	//! キャラの向き
	PlayerAnimation::DIRECTION m_Facing = PlayerAnimation::FACING_RIGHT;
	//! キャラの向きに応じてTuの値を変える為の値
	PlayerAnimation::DIRECTION_BIAS m_DirectionBias = PlayerAnimation::ZERO;
	void TurnTheAnimation(int AnimationPage);
	const int ANIMATION_INTERVAL = 2;
	const int CHARA_HEIGHT = 4;
	bool m_isScrollingDown = false;
	bool m_isInTheAir = false;
	bool m_isJump = false;
	bool m_HeldOntoWallLeft = false;
	bool m_HeldOntoWallRight = false;
	bool m_CollisionHead = false;
	bool m_isFire = false;

	int m_RiseFlameTime = 0;
	bool m_isJumpRight = false;
	bool m_isJumpLeft = false;
	bool m_isUsingArt = false;
	const float INITIAL_ACCELERATION = CELL_SIZE * 1.25f;
	Vector2 m_Acceleration = { MOVE_SPEED * 1.5f,INITIAL_ACCELERATION };

	bool m_GameFailure = false;

	/**
	* @brief マップ座標の更新
	*/
	void UpdateMapPos();

	/**
	* @brief 画面スクロール
	*/
	void MapScrool();

	/**
	* @breaf 移動処理
	*/
	void MoveOperation(KeyDirection vec);

	/**
	* @breaf ジャンプの起動
	* @return ジャンプが起動すればtrue
	* @author Toshiya Matsuoka
	*/
	bool PermitJumping();

	/**
	* @breaf ジャンプの動作
	* @author Toshiya Matsuoka
	*/
	void Jump();

	/**
	* @breaf 壁ジャンプ用横移動処理
	* @author Toshiya Matsuoka
	*/
	void JumpingLateralMotion();

	/**
	* @breaf ジャンプの動作変数の初期化
	* @author Toshiya Matsuoka
	*/
	void InitJumpParam();

	/**
	* @breaf ジャンプの上昇量調整
	* @author Toshiya Matsuoka
	*/
	void AccelarationControl();

	/**
	* @breaf 投擲アニメ処理
	* @author Toshiya Matsuoka
	*/
	void ThrowAnime();

	/**
	* @breaf 課重力処理
	* @author Toshiya Matsuoka
	*/
	void AddGravity();

	/**
	* @breaf 下に対する何かしらの当たり判定
	* @return 当たっていればtrue
	* @author Toshiya Matsuoka
	*/
	bool DownCollisionAnything();

	/**
	* @breaf 下方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool DownCollisionBlock(int block);

	/**
	* @breaf 上方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool TopCollisionBlock(int block);

	/**
	* @breaf 右方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool RightCollisionBlock(int block);

	/**
	* @breaf 左方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool LeftCollisionBlock(int block);

	/**
	* @breaf 接地判定
	* @author Toshiya Matsuoka
	*/
	bool SetGround();

	/**
	* @breaf 上方当たり判定
	* @author Toshiya Matsuoka
	*/
	bool TopCollision();

	/**
	* @breaf 左方当たり判定
	* @author Toshiya Matsuoka
	*/
	bool LeftCollision();

	/**
	* @breaf 右方当たり判定
	* @author Toshiya Matsuoka
	*/
	bool RightCollision();

	/**
	* @breaf 左右の当たり判定
	* @author Toshiya Matsuoka
	*/
	void SideCollision();

	/**
	* @breaf 慣性移動
	* @author Toshiya Matsuoka
	*/
	void MoveInertia();

	/**
	* @breaf 左方の当たり判定
	* @author Toshiya Matsuoka
	*/
	bool LeftDirectionCollision();

	/**
	* @breaf 右方の当たり判定
	* @author Toshiya Matsuoka
	*/
	bool RightDirectionCollision();

	/**
	* @breaf 下方向に水ギミックブロックがあるか確認
	* @return 下にあればtrue
	* @author Toshiya Matsuoka
	*/
	bool LookDownWater();

	/**
	* @breaf 水面のY座標取得
	* @author Toshiya Matsuoka
	*/
	float WaterCollsionCheck();

	/**
	* @breaf ゲーム失敗判定
	* @author Toshiya Matsuoka
	*/
	bool FailureGame();
};
