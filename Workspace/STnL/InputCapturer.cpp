/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   10:16
	filename: 	e:\GitHub\STnL\Workspace\STnL\InputCapturer.cpp
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	InputCapturer
	file ext:	cpp
	author:		Rainsing
	
	purpose:	≤∂ªÒ Û±Íº¸≈Ã ‰»Î
*********************************************************************/
#include "stdafx.h"
#include "InputCapturer.h"

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

	default:
		return false;
	}

	return (GetKeyState(virtKey) & 0x8000) != 0;
}
