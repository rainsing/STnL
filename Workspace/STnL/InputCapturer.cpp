/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   10:16
	filename: 	e:\GitHub\STnL\Workspace\STnL\InputCapturer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	InputCapturer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	Captures mouse/keyboard input
*********************************************************************/
#include "stdafx.h"
#include "InputCapturer.h"

bool InputCapturer::m_mouseLeftBtnDown = false;
bool InputCapturer::m_mouseRightBtnDown = false;
int InputCapturer::m_mouseX = 0;
int InputCapturer::m_mouseY = 0;
int InputCapturer::m_mouseDx = 0;
int InputCapturer::m_mouseDy = 0;
int InputCapturer::m_mouseWheelRotation = 0;

bool InputCapturer::IsKeyDown( KeyCode keyCode )
{
	int virtKey;
	switch (keyCode)
	{
	case KC_UP_ARROW:
		virtKey = VK_UP;
		break;

	case KC_DOWN_ARROW:
		virtKey = VK_DOWN;
		break;

	case KC_LEFT_ARROW:
		virtKey = VK_LEFT;
		break;

	case KC_RIGHT_ARROW:
		virtKey = VK_RIGHT;
		break;

	case KC_A:
		virtKey = 'A';
		break;

	case KC_S:
		virtKey = 'S';
		break;

	case KC_D:
		virtKey = 'D';
		break;

	case KC_W:
		virtKey = 'W';
		break;

	case KC_Q:
		virtKey = 'Q';
		break;

	case KC_E:
		virtKey = 'E';
		break;

	default:
		return false;
	}

	return (GetKeyState(virtKey) & 0x8000) != 0;
}

bool InputCapturer::IsKeyPressed( KeyCode keyCode )
{
	int virtKey;
	switch (keyCode)
	{
	case KC_R:
		virtKey = 'R';
		break;

	case KC_C:
		virtKey = 'C';
		break;

	case KC_W:
		virtKey = 'W';
		break;

	default:
		return false;
	}

	return (GetAsyncKeyState(virtKey) & 0x0001) != 0;
}

void InputCapturer::SetLeftBtnDown( bool leftBtnDown )
{
	m_mouseLeftBtnDown = leftBtnDown;
}

void InputCapturer::SetRightBtnDown( bool rightBtnDown )
{
    m_mouseRightBtnDown = rightBtnDown;
}

void InputCapturer::SetMousePosition( int x, int y )
{
	m_mouseDx = x - m_mouseX;
	m_mouseDy = y - m_mouseY;
	m_mouseX = x;
	m_mouseY = y;
}

void InputCapturer::SetMouseWheelRotation( int rotation )
{
    m_mouseWheelRotation = rotation;
}

bool InputCapturer::IsLeftBtnDown( void )
{
	return m_mouseLeftBtnDown;
}

bool InputCapturer::IsRightBtnDown( void )
{
    return m_mouseRightBtnDown;
}

void InputCapturer::GetMouseMovement( int& dx, int& dy, int& wheelRotation )
{
	dx = m_mouseDx;
	dy = m_mouseDy;
    wheelRotation = m_mouseWheelRotation;
}

void InputCapturer::ClearMouseMovement( void )
{
	m_mouseDx = m_mouseDy = 0;
    m_mouseWheelRotation = 0;
}
