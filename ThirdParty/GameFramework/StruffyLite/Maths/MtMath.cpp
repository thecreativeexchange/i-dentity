// Local includes
#include "BtTypes.h"
#include "MtMath.h"

MtVector3 Normalise (MtVector3 &v1)
{
	BtFloat magnitude = v1.Magnitude();

	MtVector3 result;

	if( magnitude )
	{
		BtFloat one_over_magnitude = 1.0f / magnitude;

		result = v1 * one_over_magnitude;
	}
	else
	{
		result.x = 0;
		result.y = 0;
		result.z = 0;
	}
	return result;
}

MtVector3 MakeEulerAnglesFromQuaternion(MtQuaternion q)
{
   double r11, r21, r31, r32, r33, r12, r13;
   double q00, q11, q22, q33;
   double tmp;
   MtVector3 u;

   q00 = q.w * q.w;
   q11 = q.x * q.x;
   q22 = q.y * q.y;
   q33 = q.z * q.z;

   r11 = q00 + q11 - q22 - q33;
   r21 = 2 * (q.x * q.y + q.w * q.z);
   r31 = 2 * (q.x * q.z + q.w * q.y);
   r32 = 2 * (q.y * q.z + q.w * q.x);
   r33 = q00 - q11 - q22 + q33;

   tmp = fabs( r31 );

   if( tmp > 0.999999 )
   {
      r12 = 2 * (q.x * q.y - q.w * q.z);
      r13 = 2 * (q.x * q.z - q.w * q.y);

      u.x = MtRadiansToDegrees(0.0f);                                // roll
      u.y = MtRadiansToDegrees( (BtFloat) (-MtHalfPI * r31/tmp));   // pitch
      u.z = MtRadiansToDegrees( (BtFloat) atan2( -r12, -r31*r13) );    // yaw
   }

   u.x = MtRadiansToDegrees( (BtFloat) atan2(  r32, r33) ); // roll
   u.y = MtRadiansToDegrees( (BtFloat)  asin( -r31     ) ); // pitch
   u.z = MtRadiansToDegrees( (BtFloat) atan2(  r21, r11) ); // yaw
   return u;
}

MtQuaternion Conjugate(MtQuaternion quat)
{
   MtQuaternion q ( -quat.x, -quat.y, -quat.z, quat.w );
   return q;
}

MtVector3 Multiply(MtVector3 &u, MtMatrix4 &m)
{
   MtVector3 temp;

   temp.x = u.x * m._00 + u.y * m._10 + u.z * m._20;
   temp.y = u.x * m._01 + u.y * m._11 + u.z * m._21;
   temp.z = u.x * m._02 + u.y * m._12 + u.z * m._22;
   return temp;
}

MtVector3 Multiply(MtMatrix4 &m, MtVector3 &u)
{
   MtVector3 temp;

   temp.x = m._00 * u.x + m._01 * u.y + m._02 * u.z;
   temp.y = m._10 * u.x + m._11 * u.y + m._12 * u.z;
   temp.z = m._20 * u.x + m._21 * u.y + m._22 * u.z;
   return temp;
}

MtQuaternion Multiply( const MtQuaternion &q, const MtVector3 &v )
{
   MtQuaternion quat;

   quat.w =-(q.x * v.x + q.y * v.y + q.z * v.z);
   quat.x =  q.w * v.x + q.y * v.z - q.z * v.y;
   quat.y =  q.w * v.y + q.z * v.x - q.x * v.z;
   quat.z =  q.w * v.z + q.x * v.y - q.y * v.x;
   return quat;
}

MtQuaternion Multiply( const MtVector3 &v, const MtQuaternion &q )
{
   MtQuaternion quat;

   quat.w = -(q.x * v.x + q.y * v.y + q.z * v.z);
   quat.x =   q.w * v.x + q.z * v.y - q.y * v.z;
   quat.y =   q.w * v.y + q.x * v.z - q.z * v.x;
   quat.z =   q.w * v.z + q.y * v.x - q.x * v.y;
   return quat;
}

BtFloat Magnitude (const MtQuaternion& q)
{
    BtFloat mag = (BtFloat) sqrt( (q.w*q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z) );
    return mag;
}

void Zero( MtVector3 &my_vector )
{
   my_vector.x = my_vector.y = my_vector.z = 0.0f;
}

BtFloat DotProduct (const MtVector3& v1, const MtVector3& v2)
{
   return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

BtFloat Magnitude (const MtVector3& v1)
{
    BtFloat mag = (BtFloat) sqrt( (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z) );
    return mag;
}


BtU32 UpperPowerOfTwo( BtU32 value )
{
	value--;
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	value++;
	return value;

}

BtFloat LengthSquared (const MtVector3& v1)
{
    BtFloat mag = (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z);
    return mag;
}

MtVector3 I(const MtMatrix4 &m)
{
   return MtVector3(m._00, m._01, m._02);
}

MtVector3 J(const MtMatrix4 &m)
{
   return MtVector3(m._10, m._11, m._12);
}

MtVector3 K(const MtMatrix4 &m)
{
   return MtVector3(m._20, m._21, m._22);
}

MtVector3 Rotate( const MtQuaternion &quaternion, const MtVector3 &vec)
{
	MtMatrix4 matrix = quaternion;

	MtVector3 v;
	
	v.x = vec.x * matrix.m[0][0] + vec.y * matrix.m[1][0] + vec.z * matrix.m[2][0] + matrix.m[3][0];
	v.y = vec.x * matrix.m[0][1] + vec.y * matrix.m[1][1] + vec.z * matrix.m[2][1] + matrix.m[3][1];
	v.z = vec.x * matrix.m[0][2] + vec.y * matrix.m[1][2] + vec.z * matrix.m[2][2] + matrix.m[3][2];
	
	return v;
}

MtVector3 Rotate( BtFloat yaw, BtFloat pitch, BtFloat roll, const MtVector3 &vec)
{
	MtMatrix4 yaw_matrix;
	MtMatrix4 pitch_matrix;
	MtMatrix4 roll_matrix;

	yaw_matrix.SetRotationX(yaw);
	pitch_matrix.SetRotationY(pitch);
	roll_matrix.SetRotationZ(roll);

	const MtMatrix4 temp = yaw_matrix * pitch_matrix * roll_matrix;

	MtVector3 result = vec * temp;

	return result;
}

void SquareOff(MtMatrix3 &m)
{
   MtVector3 x = MtVector3( m._00, m._10, m._20 );
   MtVector3 y = MtVector3( m._01, m._11, m._21 );
   MtVector3 z;

   x = Normalise(x);
   z = MtVector3::CrossProduct(x,y);
   z = Normalise(z);

   y = MtVector3::CrossProduct(z,x);
   y = Normalise(y);

   m._00 = x.x;      m._01 = y.x;     m._02 = z.x;
   m._10 = x.y;      m._11 = y.y;     m._12 = z.y;
   m._20 = x.z;      m._21 = y.z;     m._22 = z.z;
}

BtFloat MtSqrt( BtFloat fValue )
{
	return (BtFloat) sqrt( fValue );
}

BtDouble MtSqrt( double fValue )
{
	return (double) sqrt( fValue );
}

BtU32 MtSqrt( BtU32 Value )
{
	return (BtU32) sqrt( (BtFloat) Value );
}

////////////////////////////////////////////////////////////////////////////////
// MtProjectPoint

//static
MtVector3 MtProjectPointW( const MtVector4& v4Position,
						   const MtMatrix4& m4ViewProjection,
						   BtU32 viewportWidth,
						   BtU32 viewportHeight )
{
	// Multiply by the world to screen transform
	MtVector4 v4ScreenSpacePosition = v4Position * m4ViewProjection;

	// Make a 2d vector
	MtVector2 v2Position( v4ScreenSpacePosition.x, v4ScreenSpacePosition.y );

	BtFloat z = 1.0f / v4ScreenSpacePosition.w;

	// Maps -1.0f to 1.0f
	v2Position.x = v2Position.x * z;
	v2Position.y = v2Position.y * z;

	// Maps 0 to 2
	v2Position.x = v2Position.x + 1.0f;
	v2Position.y = v2Position.y + 1.0f;

	// Maps 0 to 1
	v2Position.x = v2Position.x * 0.5f;
	v2Position.y = v2Position.y * 0.5f;

	// Maps 1 to 0
	v2Position.y = 1.0f - v2Position.y;

	// Maps 0 to width, 0 to height
	MtVector3 v3Result;

	v3Result.x = v2Position.x * viewportWidth;
	v3Result.y = v2Position.y * viewportHeight;
	v3Result.z = v4ScreenSpacePosition.z;

	return v3Result;
}

////////////////////////////////////////////////////////////////////////////////
// MtProjectPointZ

//static
MtVector3 MtProjectPointZ( const MtVector4& v4Position,
						   const MtMatrix4& m4ViewProjection,
						   BtU32 viewportWidth,
						   BtU32 viewportHeight )
{
	// Multiply by the world to screen transform
	MtVector4 v4ScreenSpacePosition = v4Position * m4ViewProjection;

	// Make a 2d vector
	MtVector2 v2Position( v4ScreenSpacePosition.x, v4ScreenSpacePosition.y );

	BtFloat z = 1.0f / v4ScreenSpacePosition.z;

	// Maps -1.0f to 1.0f
	v2Position.x = v2Position.x * z;
	v2Position.y = v2Position.y * z;

	// Maps 0 to 2
	v2Position.x = v2Position.x + 1.0f;
	v2Position.y = v2Position.y + 1.0f;

	// Maps 0 to 1
	v2Position.x = v2Position.x * 0.5f;
	v2Position.y = v2Position.y * 0.5f;

	// Maps 1 to 0
	v2Position.y = 1.0f - v2Position.y;

	// Maps 0 to width, 0 to height
	MtVector3 v3Result;

	v3Result.x = v2Position.x * viewportWidth;
	v3Result.y = v2Position.y * viewportHeight;
	v3Result.z = v4ScreenSpacePosition.z;

	return v3Result;
}
