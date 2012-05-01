#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber) {
    // Set the Controller Number
    _controllerNum = playerNumber - 1;
}

XINPUT_STATE CXBOXController::GetState() {
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    XInputGetState(_controllerNum, &_controllerState);

    return _controllerState;
}

bool CXBOXController::IsConnected() {
    // Zeroise the state
    ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

    // Get the state
    DWORD Result = XInputGetState(_controllerNum, &_controllerState);

    if(Result == ERROR_SUCCESS) {
        return true;
    } else {
        return false;
    }
}

void CXBOXController::Vibrate(int leftVal, int rightVal) {
    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = leftVal;
    Vibration.wRightMotorSpeed = rightVal;

    // Vibrate the controller
    XInputSetState(_controllerNum, &Vibration);
}

qreal CXBOXController::ThumbRX() {
	return ((qreal)GetState().Gamepad.sThumbRX)/32768.0;
}

qreal CXBOXController::ThumbRY() {
	return ((qreal)GetState().Gamepad.sThumbRY)/32768.0;
}

qreal CXBOXController::ThumbLX() {
	return ((qreal)GetState().Gamepad.sThumbLX)/32768.0;
}

qreal CXBOXController::ThumbLY() {
	return ((qreal)GetState().Gamepad.sThumbLY)/32768.0;
}

qreal CXBOXController::TriggerR() {
	return ((qreal)GetState().Gamepad.bRightTrigger)/255.0;
}

qreal CXBOXController::TriggerL() {
	return ((qreal)GetState().Gamepad.bLeftTrigger)/255.0;
}

bool CXBOXController::ButtonPressed(WORD w) {
	return (GetState().Gamepad.wButtons & w) != 0;
}