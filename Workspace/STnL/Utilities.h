/********************************************************************
	created:	2012/09/14
	created:	14:9:2012   17:13
	filename: 	e:\GitHub\STnL\Workspace\STnL\Utilities.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Utilities
	file ext:	h
	author:		Rainsing
	
	purpose:	a few helper functions and macros
*********************************************************************/
#ifndef Utilities_h__
#define Utilities_h__

#define PI 3.141592653f

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

// swap the values of the two integers
inline void swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

// degrees to radians
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

// Fast floating point to fixed point conversion.
// After the pixel shader outputs the final pixel color in floating point format,
// the floats have to be converted to integers before they can be written to the frame buffer.
// This was proven (by the profiler) to be pretty slow and has resulted in very low FPS.
// So I googled and found a solution at http://stereopsis.com/FPU.html to fix this.
// However, after I upgraded the project to VS2012, the compiler by default generates faster 
// SSE instructions to do the conversion, and no longer emits the long latency FLDCW instruction.
// The fast conversion trick thus became a futility.

//const double _double2fixmagic = 68719476736.0*1.5;		//2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
//const int _shiftamt        = 16;						//16.16 fixed point representation,
//
//inline int Double2Int(double val)
//{
//	val		= val + _double2fixmagic;
//	return ((int*)&val)[0] >> _shiftamt; 
//}
//
//inline int Float2Int(float val)
//{
//	return Double2Int ((double)val);
//}

inline int Float2Int(float val)
{
    return static_cast<int>(val);
}

// scalar clamp and saturate, same as corresponding HLSL intrinsic functions
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
