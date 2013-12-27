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

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

// ��������������ֵ
inline void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// �Ƕ�ת����
#define TO_RADIAN(a) (a * PI / 180.0f)

// Safe delete
#define SAFE_DELETE(x) \
	if (x) \
	{ \
		delete x; \
		x = NULL; \
	}

#define SAFE_VECTOR_DELETE(x) \
	if (x) \
	{ \
		delete[] x; \
		x = NULL; \
	}

// ���ٸ���ת����
// ����VTune��profiling���ҷ���Intel�Ļ����ϸ���ת���εĲ������Ĳ��񻰣�
// ��Pixel Shader����ĸ�����ɫֵ�Ǳ���Ҫת�����β���д��frame buffer��ġ���
// ÿ�����Ĵε�����ת������Ӧ�ĸ���ɫͨ��������������������
// ���Ǻ���http://stereopsis.com/FPU.html �ҵ���һ����������㷨��fix������⡣

const double _double2fixmagic = 68719476736.0*1.5;		//2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
const int _shiftamt        = 16;						//16.16 fixed point representation,

inline int Double2Int(double val)
{
	val		= val + _double2fixmagic;
	return ((int*)&val)[0] >> _shiftamt; 
}

inline int Float2Int(float val)
{
	return Double2Int ((double)val);
}

// SSE���Բ�ֵ
//inline void Lerp_SSE(__m128& out, __m128& v0, __m128& v1, __m128& t)
//{
//	out = _mm_sub_ps(v0, v1);
//	out = _mm_mul_ps(out, t);
//	out = _mm_add_ps(out, v1);
//}

// clamp
inline float& Clamp(float& value, float minValue, float maxValue)
{
	if (value < minValue)
	{
		value = minValue;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}

	return value;
}

inline float& Saturate(float& value)
{
	return Clamp(value, 0.0f, 1.0f);
}

#endif // Utilities_h__
