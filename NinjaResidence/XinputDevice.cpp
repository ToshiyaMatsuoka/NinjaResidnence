/**
* @file XinputDevice.cpp
* @brief XinputDevice関連処理を行う
* @author Toshiya Matsuoka
*/
#include "DirectX.h"

#include "XinputDevice.h"



void XinputDevice::GetControl(int GamePadNumber)
{
	XINPUT_INFO = XInputGetState(GamePadNumber, &m_Xinput);
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

}

void XinputDevice::DeviceUpdate(int GamePadNumber) {

	GetControl(GamePadNumber);
	BottonCheck();
}
void XinputDevice::CheckButtonState(WORD ButtomID, ButtonIndex ButtomIndex)
{
	if (m_Xinput.Gamepad.wButtons & ButtomID)
	{
		if (m_PadOldState[ButtomIndex] == PadOn)
		{
			m_PadState[ButtomIndex] = PadOn;
		}
		else
		{
			m_PadState[ButtomIndex] = PadPush;
		}

		m_PadOldState[ButtomIndex] = PadOn;
	}
	else
	{
		if (m_PadOldState[ButtomIndex] == PadOn)
		{
			m_PadState[ButtomIndex] = PadRelease;
		}
		else
		{
			m_PadState[ButtomIndex] = PadOff;
		}
		m_PadOldState[ButtomIndex] = PadOff;
	}
}

int XinputDevice::GetAnalogTrigger(AnalogTrigger Trigger)
{
	switch (Trigger)
	{
	case LEFT_TRIGGER:
		return m_Xinput.Gamepad.bLeftTrigger;
	case RIGHT_TRIGGER:
		return m_Xinput.Gamepad.bRightTrigger;
	}
	return 0;
}

bool XinputDevice::GetAnalogL(Analog AnalogState)
{
	switch (AnalogState)
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

bool XinputDevice::GetAnalogR(Analog AnalogState)
{
	switch (AnalogState)
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

int XinputDevice::GetAnalogLValue(Analog AnalogState)
{
	switch (AnalogState)
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

int XinputDevice::GetAnalogRValue(Analog AnalogState)
{
	switch (AnalogState)
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

PADSTATE XinputDevice::GetAnalogLState(Analog AnalogState)
{
	return m_AnalogLState[AnalogState];
}

PADSTATE XinputDevice::GetAnalogRState(Analog AnalogState)
{
	return m_AnalogRState[AnalogState];
}

void XinputDevice::AnalogRStateDivide(Analog AnalogState) {
	if (GetAnalogR(AnalogState))
	{
		if (m_AnalogROldState[AnalogState] == PadOn)
		{
			m_AnalogRState[AnalogState] = PadOn;
		}
		else
		{
			m_AnalogRState[AnalogState] = PadPush;
		}
		m_AnalogROldState[AnalogState] = PadOn;
	}
	else
	{
		if (m_AnalogROldState[AnalogState] == PadOn)
		{
			m_AnalogRState[AnalogState] = PadRelease;
		}
		else
		{
			m_AnalogRState[AnalogState] = PadOff;
		}
		m_AnalogROldState[AnalogState] = PadOff;
	}
}

void XinputDevice::AnalogLStateDivide(Analog AnalogState) {
	if (GetAnalogL(AnalogState))
	{
		if (m_AnalogLOldState[AnalogState] == PadOn)
		{
			m_AnalogLState[AnalogState] = PadOn;
		}
		else
		{
			m_AnalogLState[AnalogState] = PadPush;
		}
		m_AnalogLOldState[AnalogState] = PadOn;
	}
	else
	{
		if (m_AnalogLOldState[AnalogState] == PadOn)
		{
			m_AnalogLState[AnalogState] = PadRelease;
		}
		else
		{
			m_AnalogLState[AnalogState] = PadOff;
		}
		m_AnalogLOldState[AnalogState] = PadOff;
	}
}
