////////////////////////////////////////////////////////////////////////////////
// ShWAX9

// Includes
#include "ShWAX9.h"
#include "RsUtil.h"
#include "BtMemory.h"
#include "UiKeyboard.h"
#include "BtQueue.h"

// Statics
MtVector3 ShWAX9::m_v3Accelerometer( 0, 0, 0 );
MtVector3 ShWAX9::m_v3Rotation( 0, 0, 0 );
MtQuaternion ShWAX9::m_quaternion( 0, 0, 0, 1 );
MtMatrix3 ShWAX9::m_m3Transform = MtMatrix3::m_identity;
BtBool ShWAX9::m_isReady = BtFalse;
BtQueue<ShWaxAction, 128> ShWAX9::m_actions;

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShWAX9::Update()
{
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_9 ) )
	{
		m_v3Accelerometer = MtVector3( 0, 9.8f, 0 );

		BtFloat length = m_v3Accelerometer.GetLength();
		(void)length;

		int a=0;
		a++;
	}
	else
	{
		m_v3Accelerometer = MtVector3( 0, 0, 0 );
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetQuaternion

//static
void ShWAX9::SetQuaternion( MtQuaternion &quaternion )
{
    m_quaternion = quaternion;
    m_quaternion.Normalise();
    
    m_m3Transform = MtMatrix3( m_quaternion );
    
    m_isReady = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// SetIMUAccelerometer

//static
void ShWAX9::SetIMUAccelerometer( MtVector3 &v3Accelerometer )
{
    m_v3Accelerometer = v3Accelerometer;
}

////////////////////////////////////////////////////////////////////////////////
// SetIMURotation

//static
void ShWAX9::SetIMURotation( MtVector3 &v3Rotation )
{
    m_v3Rotation = v3Rotation;
}

////////////////////////////////////////////////////////////////////////////////
// GetAccelerometer

//static
MtVector3 ShWAX9::GetAccelerometer()
{
    return m_v3Accelerometer;
}

////////////////////////////////////////////////////////////////////////////////
// GetTransform

//static
MtMatrix3 ShWAX9::GetTransform()
{
    return m_m3Transform;
}

////////////////////////////////////////////////////////////////////////////////
// IsReady

//static
BtBool ShWAX9::IsReady()
{
    return m_isReady;
}

////////////////////////////////////////////////////////////////////////////////
// GetQuaternion

//static
MtQuaternion ShWAX9::GetQuaternion()
{
    return m_quaternion;
}

////////////////////////////////////////////////////////////////////////////////
// GetRotation

//static
MtVector3 ShWAX9::GetRotation()
{
    return m_v3Rotation;
}

////////////////////////////////////////////////////////////////////////////////
// GetRotationTransform

//static
MtMatrix4 ShWAX9::GetRotationTransform()
{
	return MtMatrix4( m_quaternion );
}
