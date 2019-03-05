#pragma once
/**
* @file XinputDevice.h
* @brief XinputDevice関連処理を行う
* @author Toshiya Matsuoka
*/
#include<Xinput.h>


/**
* @enum PADSTATE
* ボタンの押下状態
*/
enum PADSTATE
{
	//! 押されていない
	PadOff,
	//! 押されている
	PadOn,
	//! 押された
	PadPush,
	//! 離された
	PadRelease
};
/**
* @enum ButtonIndex
* パッドのボタンの列挙
*/
enum ButtonIndex
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	//! R1ボタン
	ButtonRB,
	//! L1ボタン
	ButtonLB,
	ButtonStart,
	ButtonBack,
	ButtonUP,
	ButtonDOWN,
	ButtonLEFT,
	ButtonRIGHT,
	//!L3ボタン
	ButtonLeftThumb,
	//!R3ボタン
	ButtonRightThumb,
	ButtomIndexMAX
};

/**
* @enum Analog
* アナログスティックの傾き方向
*/
enum Analog
{
	ANALOGUP,
	ANALOGDOWN,
	ANALOGLEFT,
	ANALOGRIGHT,
	//!横方向
	ANALOG_X,
	//!縦方向
	ANALOG_Y,
	ANALOGMAX
};
/**
* @enum AnalogTrigger
* 左右のトリガー識別
*/
enum AnalogTrigger
{
	LEFT_TRIGGER,
	RIGHT_TRIGGER
};

class XinputDevice {
	
public:

	/**
	* @brief XinputDeviceでゲームパッドデバイスの取得
	* @param gamePadNumber パッド番号（0ベース）
	*/
	void GetControl(int gamePadNumber = 0);

	/**
	* @brief XinputDeviceでゲームパッドの入力取得
	* @param index 取得したいボタンの配列番号
	* @return PADSTATEの値
	* @sa enum PADSTATE
	*/
	PADSTATE GetButton(ButtonIndex index);

	/**
	* @brief 全てのボタンに対してCheckButtonStateを行う
	* @sa CheckButtonState(WORD buttonID, int buttonIndex)
	*/
	void BottonCheck();

	/**
	* @brief GetControlとBottonCheckを纏めて行う
	* @param gamePadNumber パッド番号（0ベース）
	* @sa GetControl
	* @sa BottonCheck
	*/

	void DeviceUpdate(int gamePadNumber = 0);
	/**
	* @brief XinputDeviceでゲームパッドの左アナログスティック入力取得
	* @param trigger トリガーの左右認識番号
	* @return 0～255の値、押してなければ0
	* @sa enum AnalogTrigger
	*/
	int GetAnalogTrigger(AnalogTrigger trigger);

	/**
	* @brief XinputDeviceでゲームパッドの左アナログスティック入力取得
	* @param analogState スティックの方向け先番号
	* @return 傾いていればTrue、そうでなければFalse
	* @sa enum Analog
	*/
	bool GetAnalogL(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの右アナログスティック入力取得
	* @param analogState スティックの方向け先番号
	* @return 傾いていればTrue、そうでなければFalse
	* @sa enum Analog
	*/
	bool GetAnalogR(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの左アナログスティック入力取得
	* @param analogState スティックの方向け先番号
	* @return 傾き具合の数値　MAX＝32767　MIN＝-32768
	* @sa enum Analog
	*/
	int GetAnalogLValue(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの右アナログスティック入力取得
	* @param analogState スティックの方向け先番号
	* @return 傾き具合の数値　MAX＝32767　MIN＝-32768
	* @sa enum Analog
	*/
	int GetAnalogRValue(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの左アナログスティック入力状態取得
	* @param analogState スティックの方向け先番号
	* @return 方向け状態
	* @sa enum Analog
	* @sa enum PADSTATE
	*/
	PADSTATE GetAnalogLState(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの左アナログスティック入力状態取得
	* @param analogState スティックの方向け先番号
	* @return 方向け状態
	* @sa enum Analog
	* @sa enum PADSTATE
	*/
	PADSTATE GetAnalogRState(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの右トリガー入力状態取得
	* @return 押下状態
	* @sa enum PADSTATE
	*/
	PADSTATE GetTriggerRState();

	/**
	* @brief  XinputDeviceでゲームパッドの左トリガー入力状態取得
	* @return 押下状態
	* @sa enum PADSTATE
	*/
	PADSTATE GetTriggerLState();

	/**
	* @brief  XinputDeviceでゲームパッドの左アナログスティック入力状態振り分け
	* @param analogState ANALOG_X　or　ANALOG_Y
	* @sa enum Analog
	*/
	void AnalogLStateDivide(Analog analogState);

	/**
	* @brief  XinputDeviceでゲームパッドの右トリガー入力状態振り分け
	*/
	void TriggerRStateDivide();

	/**
	* @brief  XinputDeviceでゲームパッドの左トリガー入力状態振り分け
	*/
	void TriggerLStateDivide();

	/**
	* @brief 左右のバイブレーションモーターを動作させる
	* @param leftValue バイブレーション値 MAX＝65535　MIN＝0
	* @param rightValue バイブレーション値 MAX＝65535　MIN＝0
	* @details 右は高周波モーター、左は低周波モーター
	*/
	void RunVibration(unsigned int leftValue = 0, unsigned int rightValue = 0);

	/**
	* @brief XinputDeviceでゲームパッドの右アナログスティック入力状態振り分け
	* @param analogState ANALOG_X　or　ANALOG_Y
	* @sa enum Analog
	*/
	void AnalogRStateDivide(Analog analogState);

	/**
	* @brief Dinputでキーボードの状態取得
	* @param buttonID 取得したいXInputボタン番号
	* @param buttonIndex 取得したいボタンの配列番号
	* @sa enum ButtonIndex
	* @details 使用時には　m_PadState[buttonID]　の中身を確認すること
	*/
	void CheckButtonState(WORD buttonID, ButtonIndex buttonIndex);

	XinputDevice() {};
	~XinputDevice() {};
private:
	XINPUT_STATE m_Xinput = { NULL };

	PADSTATE m_PadState[ButtomIndexMAX] = { PadOff };
	PADSTATE m_PadOldState[ButtomIndexMAX] = { PadOff };

	PADSTATE m_AnalogROldState[ANALOGMAX] = { PadOff };
	PADSTATE m_AnalogRState[ANALOGMAX] = { PadOff };

	PADSTATE m_AnalogLOldState[ANALOGMAX] = { PadOff };
	PADSTATE m_AnalogLState[ANALOGMAX] = { PadOff };
	
	PADSTATE m_TriggerROldState = PadOff;
	PADSTATE m_TriggerRState = PadOff;

	PADSTATE m_TriggerLOldState = PadOff;
	PADSTATE m_TriggerLState = PadOff;
	//! デバイスの状態確認用
	DWORD XINPUT_INFO = -1;
	
	const int INCLINATION_THRESHOLD_VALUE = 65535 / 4;
	
};
