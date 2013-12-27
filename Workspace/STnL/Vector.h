/********************************************************************
	created:	2012/09/17
	created:	17:9:2012   11:30
	filename: 	e:\GitHub\STnL\Workspace\STnL\Vector.h
	file path:	e:\GitHub\STnL\Workspace\STnL
	file base:	Vector
	file ext:	h
	author:		Rainsing
	
	purpose:	
*********************************************************************/
#ifndef Vector_h__
#define Vector_h__

class Vector4;

class Vector2
{
public:
	float x;
	float y;
};

class Vector3
{
public:
	inline Vector3(void);
	inline Vector3(float _x, float _y, float _z);

	inline Vector3 operator -(const Vector3& rhs) const;
	inline Vector3 operator +(float f) const;
	inline Vector3 operator +(Vector3& rhs) const;
	inline Vector3 operator *(float f) const;

	inline float Length(void);
	inline void Normalize(void);
	inline float Dot(Vector3& rhs) const;
	inline Vector3 Cross(Vector3& rhs) const;
	inline bool Equal(const Vector3& rhs, float tolerance);

	static const Vector3 ZERO;

	float x;
	float y;
	float z;
};

Vector3::Vector3( void )
{

}

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

Vector3 Vector3::operator-( const Vector3& rhs ) const
{
	return Vector3(x - rhs.x , y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator+( Vector3& rhs ) const
{
	return Vector3(x + rhs.x , y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator+( float f ) const
{
	return Vector3(x + f , y + f, z + f);
}

Vector3 Vector3::operator*( float f ) const
{
	Vector3 out;
	out.x = x * f;
	out.y = y * f;
	out.z = z * f;
	return out;
}

float Vector3::Length( void )
{
	return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalize( void )
{
	float lenReciprocal = 1.0f / Length();
	x *= lenReciprocal;
	y *= lenReciprocal;
	z *= lenReciprocal;
}

float Vector3::Dot( Vector3& rhs ) const
{
	return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3 Vector3::Cross( Vector3& rhs ) const
{
	Vector3 out;

	out.x = y * rhs.z - z * rhs.y;
	out.y = z * rhs.x - x * rhs.z;
	out.z = x * rhs.y - y * rhs.x;

	return out;
}

bool Vector3::Equal( const Vector3& rhs, float tolerance )
{
	return fabs(x - rhs.x) < tolerance && fabs(y - rhs.y) < tolerance && fabs(z - rhs.z) < tolerance;
}

// -------------------------------------------------

class Vector4
{
public:
	inline Vector4(void);
	inline Vector4(float _x, float _y, float _z, float _w);
	inline Vector4(const Vector3& vec3);

	inline Vector4 operator -(const Vector4& rhs) const;

	float x;
	float y;
	float z;
	float w;
};

Vector4::Vector4( void )
{

}

Vector4::Vector4( float _x, float _y, float _z, float _w )
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Vector4::Vector4( const Vector3& vec3 )
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
	w = 1.0f;
}

Vector4 Vector4::operator-( const Vector4& rhs ) const
{
	return Vector4(x - rhs.x , y - rhs.y, z - rhs.z, w - rhs.w);
}

Vector4 Lerp(Vector4& v0, Vector4& v1, float t);
void Lerp(Vector4& out, Vector4& v0, Vector4& v1, float t);

Vector4& Clamp(Vector4& value, float minValue, float maxValue);
Vector4& Saturate(Vector4& value);

#endif // Vector_h__
