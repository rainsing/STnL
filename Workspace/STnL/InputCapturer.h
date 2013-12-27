/********************************************************************
	created:	2012/09/18
	created:	18:9:2012   10:15
	filename: 	e:\GitHub\STnL\Workspace\STnL\InputCapturer.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	InputCapturer
	file ext:	h
	author:		Rainsing
	
	purpose:	≤∂ªÒ Û±Íº¸≈Ã ‰»Î
*********************************************************************/
#ifndef InputCapturer_h__
#define InputCapturer_h__

enum KeyCode
{
	KC_LEFT_ARROW,
	KC_RIGHT_ARROW,
	KC_UP_ARROW,
	KC_DOWN_ARROW,
	KC_A,
	KC_S,
	KC_D,
	KC_W,
	KC_Q,
	KC_E,
	KC_R,
	KC_C,
	KC_T
};

class InputCapturer
{
public:
	bool IsKeyDown(KeyCode keyCode);
	bool IsKeyPressed(KeyCode keyCode);

	static void SetLeftBtnDown(bool leftBtnDown);
	static void SetMousePosition(int x, int y);

	bool IsLeftBtnDown(void);
	void GetMouseMovement(int& dx, int& dy);
	void ClearMouseMovement(void);

private:
	static bool m_mouseLeftBtnDown;
	static int m_mouseX;
	static int m_mouseY;
	static int m_mouseDx;
	static int m_mouseDy;
};

#endif // InputCapturer_h__
