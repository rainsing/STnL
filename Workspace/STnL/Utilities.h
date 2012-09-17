/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\Utilities.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Utilities
	file ext:	h
	author:		Rainsing
	
	purpose:	һЩhelper����
*********************************************************************/
#ifndef Utilities_h__
#define Utilities_h__

#define PI 3.141592653f

// ��������������ֵ
inline void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// �Ƕ�ת����
#define TO_RADIAN(a) (a * PI / 180.0f)

#endif // Utilities_h__
