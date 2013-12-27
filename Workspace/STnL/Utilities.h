/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\Utilities.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Utilities
	file ext:	h
	author:		Rainsing
	
	purpose:	一些helper函数
*********************************************************************/
#ifndef Utilities_h__
#define Utilities_h__

#define PI 3.141592653f

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

// 交换两个整数的值
inline void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// 角度转弧度
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

// 快速浮点转整数
// 经过VTune的profiling，我发现Intel的机器上浮点转整形的操作慢的不像话，
// 而Pixel Shader算完的浮点颜色值是必须要转成整形才能写到frame buffer里的……
// 每像素四次的这种转换（对应四个颜色通道）足以拖慢整个程序。
// 于是乎在http://stereopsis.com/FPU.html 找到了一个很神奇的算法来fix这个问题。

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

// SSE线性插值
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
