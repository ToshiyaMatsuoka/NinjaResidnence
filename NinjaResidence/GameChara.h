/**
* @file GameChara.h
* @brief GameCharaクラス
* @author Kojiro Kawahara
*/
#pragma once

#include <vector>
#include "Object.h"
#include "MapChip.h"

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
	CUSTOMVERTEX GetDisplayCharaCoordinate() { return m_DisplayCoordinate[4]; }
	void PrevSaveMapPos();
	void KeyOperation(KeyDirection vec);
	void PositionSave(Object* MapChip, int BlockNumber);
	bool Update();
	bool CollisionIventBlock();
	void Render();
	GameChara(DirectX* pDirectX, SoundOperater* pSoundOperater, Object* MapChip);
	~GameChara();

	int GetMapLeftDirectionPosition() { return m_MapLeftDirectionPosition; }
	int GetMapPositionY() { return m_MapPositionY; }

	void NoOperation();
	/**
	* @brief Debug用キャラ上昇処理
	* @author Toshiya Matsuoka
	*/
	void DebugMove();

	/**
	* @brief キャラのディスプレイ上前端の取得
	* @author Toshiya Matsuoka
	*/
	float GetPositionX();
	/**
	* @brief キャラのディスプレイ上中心Y座標の取得
	* @author Toshiya Matsuoka
	*/
	float GetPositionY() {
		return m_DisplayCoordinate[0].y + (CELL_SIZE * 2);
	}

	/**
	* @brief キャラの向いている方向の取得
	* @sa PlayerAnimation::DIRECTION
	* @author Toshiya Matsuoka
	*/
	PlayerAnimation::DIRECTION GetFacing() {
		return m_Facing;
	}
	/**
	* @brief 火遁アニメーション動作
	* @author Toshiya Matsuoka
	*/
	void FireArtAnime();
	bool GetGameFailure() {
		return m_GameFailure;
	}


private:
	//前フレームのm_MapLeftDirectionPositionの値
	int m_PrevMapLeftDirectionPosition = 0;
	//前フレームのm_MapRightDirectionPositionの値
	int m_PrevMapRightDirectionPosition = 0;
	//前フレームのm_MapPositionYの値
	int m_PrevMapCharaPositionY = 0;
	//キャラの左側のX座標
	int m_MapLeftDirectionPosition = 0;
	//キャラの右側のX座標
	int m_MapRightDirectionPosition = 0;
	int m_MapPositionY = 0;//キャラの上側のx座標
	//! 毎フレームかける重力の値
	const float GRAVITY = 15.f;
	void Dash();
	//! 移動量
	float MOVE_SPEED = 15.f;
	const int VERTICAL_SCROLLING_LEVEL = 20;
	const int ScrollSpeed = 15;

	//m_DisplayCoordinateのY座標がこの値を下回ると上にスクロールする
	const int DisplayCharMoveScopeUp = 100;
	//m_DisplayCoordinateのY座標がこの値を超えると上にスクロールする
	const int DisplayCharMoveScopeDown = 630;
	//m_DisplayCoordinateのX座標がこの値を下回ると左にスクロールする
	const int DisplayCharMoveScopeLeft = 300;
	//m_DisplayCoordinateのX座標がこの値を超えると右にスクロールする
	const int DisplayCharMoveScopeRight = 980;
	//両端からのX座標の稼働範囲
	const int DisplayCharMoveScopeX = 300;
	void UpdateMapPos();
	/**
	* @brief 画面スクロール
	*/
	void MapScrool();

	bool m_isInertiaMoving = false;
	//void MoveOperation(KeyDirection vec, CUSTOMVERTEX* pWorldCharaCoordinate, CUSTOMVERTEX* pDisplayCharaCoordinate, float MoveQuantity);
	void MapReversePointSearch(int BlockNumber, MapDataState MapState);
	//! 当たり判定描画用
	float m_CollisionTu = 80 / 512.f;
	//! 当たり判定描画用
	float m_CollisionTv = 160 / 512.f;
	MapReverse* m_pMapReverse = NULL;
	Object* m_pMapChip = NULL;
	float GravityAcceleration = 0;


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
	const float InitialAcceleration = 50.0f;
	float m_AccelerationY = InitialAcceleration;
	float m_AccelerationX = MOVE_SPEED * 1.5f;

	bool m_GameFailure = false;

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
	bool DownCollisionAnything(void);
	/**
	* @breaf 下方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool DownCollisionCheck(int block);
	/**
	* @breaf 上方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool TopCollisionCheck(int block);
	/**
	* @breaf 右方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool RightCollisionCheck(int block);
	/**
	* @breaf 左方向に対する当たり判定
	* @param block 指定のブロック番号
	* @return 当たっていればtrue
	* @sa  MapBlock::BLOCKTYPE
	* @author Toshiya Matsuoka
	*/
	bool LeftCollisionCheck(int block);
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

	void MoveInertia();


	bool LeftDirectionCollision();

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
