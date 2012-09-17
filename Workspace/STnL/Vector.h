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

class Vector3
{
public:
	float x;
	float y;
	float z;
};

class Vector4
{
public:
	inline Vector4(void);
	inline Vector4(const Vector3& vec3);

	float x;
	float y;
	float z;
	float w;
};

Vector4::Vector4( void )
{

}

Vector4::Vector4( const Vector3& vec3 )
{
	x = vec3.x;
	y = vec3.y;
	z = vec3.z;
	w = 1.0f;
}

#endif // Vector_h__
