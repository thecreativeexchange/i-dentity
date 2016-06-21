//////////////////////////////////////////////////////////////////////////////
// MtMath.h

// Include guards
#pragma once
#include <math.h>
#include <stdlib.h>
#include "BtTypes.h"

// Defines
#define MtPI      3.141592654f
#define MtHalfPI  1.570796327f
#define Mt2PI     6.283185307f
#define MtRoot2   1.414213562f
#define MtFloatingPointTolerance 0.0001f

// Includes
#include "MtVector2.h"
#include "MtVector3.h"
#include "MtVector4.h"
#include "MtQuaternion.h"
#include "MtMatrix3.h"
#include "MtMatrix4.h"
#include "MtMath.h"
#include "MtPlane.h"
#include "MtPoint.h"
#include "MtLine.h"
#include "MtTriangle.h"
#include "MtRay.h"
#include "MtTessellation.h"

// Global function declarations
template <class _Value> _Value MtSquare( _Value f )
{
   return f * f;
}

// Return the maximum of two values
template <class _Value> _Value MtMax( _Value f, _Value s )
{
   return f < s ? s : f;
}

// Return the minimum of two values
template <class _Value> _Value MtMin( _Value f, _Value s)
{
	return f > s ? s : f;
}


inline BtFloat MtCeil( BtFloat f )
{
	return ceil( f );
}

inline BtFloat MtFloor( BtFloat f )
{
	return floor( f );
}

inline BtBool MtIsEquivalent(float f1, float f2)
{
	return f1 - f2 < MtFloatingPointTolerance && f2 - f1 < MtFloatingPointTolerance;
}

inline BtFloat MtAtoF( const BtChar *text )
{
	return (BtFloat)atof( text );
}

// Return the absolute value of the passed value
template <class _Value> _Value MtAbs( _Value v )
{
   return v < _Value(0) ? -v : v;
}

template <class _Value> void MtClamp( _Value& value, const _Value min, const _Value max )
{
    if ( value < min )
    {
        value = min;
    }
    else if ( value > max )
    {
        value = max;
    }
}

inline void MtClamp( BtFloat& x, BtFloat min, BtFloat max )
{
	if ( x < min )
	{
		x = min;
	}
	else if ( x > max )
	{
		x = max;
	}
}

inline void MtClamp( BtS32& x, BtS32 min, BtS32 max )
{
	if ( x < min )
	{
		x = min;
	}
	else if ( x > max )
	{
		x = max;
	}
}

//////////////////////////////////////////////////////////////////////////////
// RadiansToDegrees

inline BtFloat MtRadiansToDegrees(BtFloat fRadians)
{
	return fRadians * 180.0f / MtPI;
}

//////////////////////////////////////////////////////////////////////////////
// MtKnotsToMetersPerSecond

inline BtFloat MtKnotsToMetersPerSecond(BtFloat knots)
{
	return knots * 0.51444f;
}

//////////////////////////////////////////////////////////////////////////////
// DegreesToRadians

inline BtFloat MtDegreesToRadians(BtFloat fDegrees)
{
	return fDegrees * MtPI / 180.0f;
}

//////////////////////////////////////////////////////////////////////////////
// MtATan

inline BtFloat MtTan(BtFloat fRadians)
{
	return tan(fRadians);
}

//////////////////////////////////////////////////////////////////////////////
// MtCos

inline BtFloat MtCos(BtFloat fRadians)
{
	return cos(fRadians);
}

//////////////////////////////////////////////////////////////////////////////
// MtSin

inline BtFloat MtSin(BtFloat fRadians)
{
	return sin(fRadians);
}

//////////////////////////////////////////////////////////////////////////////
// MtACos

inline BtFloat MtACos(BtFloat fValue)
{
	return acos(fValue);
}

//////////////////////////////////////////////////////////////////////////////
// MtATan

inline BtFloat MtATan(BtFloat fOpposite, BtFloat fAdjacent)
{
	return atan2(fOpposite, fAdjacent);
}

//////////////////////////////////////////////////////////////////////////////
// MtASin

inline BtFloat MtASin(BtFloat fAngle)
{
	return asin(fAngle);
}

//////////////////////////////////////////////////////////////////////////////
// MtPow

inline BtFloat MtPow(BtFloat base, BtFloat exponent)
{
	return pow(base, exponent);
}

////////////////////////////////////////////////////////////////////////////////
// Linear interpolation

template<class T> inline T MtLerp(BtFloat t, const T& l, const T& h)
{
	return l + ((h - l) * t);
}
const BtFloat MtTol = 0.0001f;

BtU32 UpperPowerOfTwo(BtU32 v);

BtFloat MtRadiansToDegrees(BtFloat rad);

BtFloat MtDegreesToRadians(BtFloat deg);

MtVector3 Normalise (MtVector3 &v1);

MtVector3 MakeEulerAnglesFromQuaternion(MtQuaternion q);

MtQuaternion Conjugate(MtQuaternion quat);

MtVector3 Multiply(MtVector3 &u, MtMatrix4 &m);

MtVector3 Multiply(MtMatrix4 &m, MtVector3 &u);

MtQuaternion Multiply( const MtQuaternion &q, const MtVector3 &v );

MtQuaternion Multiply( const MtVector3 &v, const MtQuaternion &q );

BtFloat Magnitude (const MtQuaternion& q);

void Zero( MtVector3 &my_vector );

BtFloat DotProduct (const MtVector3& v1, const MtVector3& v2);

BtFloat Magnitude (const MtVector3& v1);

BtFloat LengthSquared (const MtVector3& v1);

MtVector3 I(const MtMatrix4 &m);

MtVector3 J(const MtMatrix4 &m);

MtVector3 K(const MtMatrix4 &m);

MtVector3 Rotate( const MtQuaternion &quaternion, const MtVector3 &vec);

MtVector3 Rotate( BtFloat yaw, BtFloat pitch, BtFloat roll, const MtVector3 &vec);

inline BtFloat MtSign(const BtFloat& val) { return (val >= 0.0f) ? 1.0f : -1.0f; }

void SquareOff(MtMatrix3 &m);

BtFloat MtSqrt( BtFloat fValue );
BtDouble MtSqrt( double fValue );
BtU32 MtSqrt( BtU32 fValue );
inline BtFloat MtPow( BtFloat base, BtFloat exponent );

MtVector3 MtProjectPointW( const MtVector4& v4Position, const MtMatrix4& m4ViewProjection, BtU32 viewportWidth, BtU32 viewportHeight );
MtVector3 MtProjectPointZ( const MtVector4& v4Position, const MtMatrix4& m4ViewProjection, BtU32 viewportWidth, BtU32 viewportHeight );
