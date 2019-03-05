/**
* @file XinputDevice.cpp
* @brief XinputDevice関連処理を行う
* @author Toshiya Matsuoka
*/
#include "DirectX.h"

#include "XinputDevice.h"



void XinputDevice::GetControl(int gamePadNumber)
{
	XINPUT_INFO = XInputGetState(gamePadNumber, &m_Xinput);
}

PADSTATE XinputDevice::GetButton(ButtonIndex index)
{
	return m_PadState[index];
}

void XinputDevice::BottonCheck() {

	CheckButtonState(XINPUT_GAMEPAD_A, ButtonA);
	CheckButtonState(XINPUT_GAMEPAD_B, ButtonB);
	CheckButtonState(XINPUT_GAMEPAD_X, ButtonX);
	CheckButtonState(XINPUT_GAMEPAD_Y, ButtonY);
	CheckButtonState(XINPUT_GAMEPAD_START, ButtonStart);
	CheckButtonState(XINPUT_GAMEPAD_BACK, ButtonBack);
	CheckButtonState(XINPUT_GAMEPAD_LEFT_SHOULDER, ButtonLB);
	CheckButtonState(XINPUT_GAMEPAD_RIGHT_SHOULDER, ButtonRB);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_UP, ButtonUP);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_DOWN, ButtonDOWN);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_LEFT, ButtonLEFT);
	CheckButtonState(XINPUT_GAMEPAD_DPAD_RIGHT, ButtonRIGHT);
	CheckButtonState(XINPUT_GAMEPAD_LEFT_THUMB, ButtonLeftThumb);
	CheckButtonState(XINPUT_GAMEPAD_RIGHT_THUMB, ButtonRightThumb);

	AnalogLStateDivide(ANALOGUP);
	AnalogRStateDivide(ANALOGUP);
	AnalogLStateDivide(ANALOGDOWN);
	AnalogRStateDivide(ANALOGDOWN);
	AnalogLStateDivide(ANALOGLEFT);
	AnalogRStateDivide(ANALOGLEFT);
	AnalogLStateDivide(ANALOGRIGHT);
	AnalogRStateDivide(ANALOGRIGHT);

	TriggerRStateDivide();
	TriggerLStateDivide();
}

void XinputDevice::DeviceUpdate(int gamePadNumber) {

	GetControl(gamePadNumber);
	BottonCheck();
}

void XinputDevice::CheckButtonState(WORD buttonID, ButtonIndex buttonIndex)
{
	if (m_Xinput.Gamepad.wButtons & buttonID)
	{
		if (m_PadOldState[buttonIndex] == PadOn)
		{
			m_PadState[buttonIndex] = PadOn;
		}
		else
		{
			m_PadState[buttonIndex] = PadPush;
		}

		m_PadOldState[buttonIndex] = PadOn;
	}
	else
	{
		if (m_PadOldState[buttonIndex] == PadOn)
		{
			m_PadState[buttonIndex] = PadRelease;
		}
		else
		{
			m_PadState[buttonIndex] = PadOff;
		}
		m_PadOldState[buttonIndex] = PadOff;
	}
}

int XinputDevice::GetAnalogTrigger(AnalogTrigger trigger)
{
	switch (trigger)
	{
	case LEFT_TRIGGER:
		return m_Xinput.Gamepad.bLeftTrigger;
	case RIGHT_TRIGGER:
		return m_Xinput.Gamepad.bRightTrigger;
	}
	return 0;
}

bool XinputDevice::GetAnalogL(Analog analogState)
{
	switch (analogState)
	{
	case ANALOGUP:
		if (GetAnalogLValue(ANALOG_Y) > INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGDOWN:
		if (GetAnalogLValue(ANALOG_Y) < -INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGRIGHT:
		if (GetAnalogLValue(ANALOG_X) > INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGLEFT:
		if (GetAnalogLValue(ANALOG_X) < -INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

bool XinputDevice::GetAnalogR(Analog analogState)
{
	switch (analogState)
	{
	case ANALOGUP:
		if (GetAnalogRValue(ANALOG_Y) > INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGDOWN:
		if (GetAnalogRValue(ANALOG_Y) < -INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGRIGHT:
		if (GetAnalogRValue(ANALOG_X) > INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	case ANALOGLEFT:
		if (GetAnalogRValue(ANALOG_X) < -INCLINATION_THRESHOLD_VALUE)
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

int XinputDevice::GetAnalogLValue(Analog analogState)
{
	switch (analogState)
	{
	case ANALOG_Y:
		return m_Xinput.Gamepad.sThumbLY;
		break;
	case ANALOG_X:
		return m_Xinput.Gamepad.sThumbLX;
		break;
	default:
		return 0;
	}
}

int XinputDevice::GetAnalogRValue(Analog analogState)
{
	switch (analogState)
	{
	case ANALOG_Y:
		return m_Xinput.Gamepad.sThumbRY;
		break;
	case ANALOG_X:
		return m_Xinput.Gamepad.sThumbRX;
		break;
	default:
		return 0;
	}
}

PADSTATE XinputDevice::GetAnalogLState(Analog analogState)
{
	return m_AnalogLState[analogState];
}

PADSTATE XinputDevice::GetAnalogRState(Analog analogState)
{
	return m_AnalogRState[analogState];
}

PADSTATE XinputDevice::GetTriggerRState()
{
	return m_TriggerRState;
}

PADSTATE XinputDevice::GetTriggerLState()
{
	return m_TriggerLState;
}

void XinputDevice::AnalogRStateDivide(Analog analogState) {
	if (GetAnalogR(analogState))
	{
		if (m_AnalogROldState[analogState] == PadOn)
		{
			m_AnalogRState[analogState] = PadOn;
		}
		else
		{
			m_AnalogRState[analogState] = PadPush;
		}
		m_AnalogROldState[analogState] = PadOn;
	}
	else
	{
		if (m_AnalogROldState[analogState] == PadOn)
		{
			m_AnalogRState[analogState] = PadRelease;
		}
		else
		{
			m_AnalogRState[analogState] = PadOff;
		}
		m_AnalogROldState[analogState] = PadOff;
	}
}

void XinputDevice::AnalogLStateDivide(Analog analogState) {
	if (GetAnalogL(analogState))
	{
		if (m_AnalogLOldState[analogState] == PadOn)
		{
			m_AnalogLState[analogState] = PadOn;
		}
		else
		{
			m_AnalogLState[analogState] = PadPush;
		}
		m_AnalogLOldState[analogState] = PadOn;
	}
	else
	{
		if (m_AnalogLOldState[analogState] == PadOn)
		{
			m_AnalogLState[analogState] = PadRelease;
		}
		else
		{
			m_AnalogLState[analogState] = PadOff;
		}
		m_AnalogLOldState[analogState] = PadOff;
	}
}

void XinputDevice::TriggerRStateDivide() {
	if (GetAnalogTrigger(RIGHT_TRIGGER))
	{
		if (m_TriggerROldState == PadOn)
		{
			m_TriggerRState = PadOn;
		}
		else
		{
			m_TriggerRState = PadPush;
		}
		m_TriggerROldState = PadOn;
	}
	else
	{
		if (m_TriggerROldState == PadOn)
		{
			m_TriggerRState = PadRelease;
		}
		else
		{
			m_TriggerRState = PadOff;
		}
		m_TriggerROldState = PadOff;
	}
}
void XinputDevice::TriggerLStateDivide() {
	if (GetAnalogTrigger(LEFT_TRIGGER))
	{
		if (m_TriggerLOldState == PadOn)
		{
			m_TriggerLState = PadOn;
		}
		else
		{
			m_TriggerLState = PadPush;
		}
		m_TriggerLOldState = PadOn;
	}
	else
	{
		if (m_TriggerLOldState == PadOn)
		{
			m_TriggerLState = PadRelease;
		}
		else
		{
			m_TriggerLState = PadOff;
		}
		m_TriggerLOldState = PadOff;
	}
}

void XinputDevice::RunVibration(unsigned int leftValue, unsigned int rightValue) {
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftValue;
	vibration.wRightMotorSpeed = rightValue;
	XInputSetState(0, &vibration);
}