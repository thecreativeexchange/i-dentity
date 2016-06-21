////////////////////////////////////////////////////////////////////////////////
// ShBeacon.cpp

// Includes
#include "ShBeacon.h"
#include "UiKeyboard.h"

// Static variables
BtArray<ShBeaconNode, 32> ShBeacon::m_beacons;
BtQueue<ShBeaconNode, 32> ShBeacon::m_beaconAlert;
BtBool ShBeacon::m_debugBeacons[MaxDebugBeacons];
BtBool ShBeacon::m_isEnabled = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ShBeacon::ShBeacon()
{
	for (BtU32 i = 0; i < MaxDebugBeacons; i++)
	{
		m_debugBeacons[i] = BtFalse;
	}
}

////////////////////////////////////////////////////////////////////////////////
// IsEnabled

BtBool ShBeacon::IsEnabled()
{
	return m_isEnabled;
}

////////////////////////////////////////////////////////////////////////////////
// SetEnabled

void ShBeacon::SetEnabled(BtBool isEnabled)
{
	m_isEnabled = isEnabled;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

//static
void ShBeacon::Reset()
{
	m_beacons.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// Update

//static
void ShBeacon::Update()
{
	Reset();

	if (UiKeyboard::pInstance()->IsPressed(UiKeyCode_0))
	{
		m_debugBeacons[0] = !m_debugBeacons[0];
	}
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_1 ) )
	{
		m_debugBeacons[1] = !m_debugBeacons[1];
	}
	if (UiKeyboard::pInstance()->IsPressed( UiKeyCode_2 ) )
	{
		m_debugBeacons[2] = !m_debugBeacons[2];
	}
	if (UiKeyboard::pInstance()->IsPressed( UiKeyCode_3 ) )
	{
		m_debugBeacons[3] = !m_debugBeacons[3];
	}
	if (UiKeyboard::pInstance()->IsPressed( UiKeyCode_4 ) )
	{
		m_debugBeacons[4] = !m_debugBeacons[4];
	}
	if (UiKeyboard::pInstance()->IsPressed( UiKeyCode_5 ) )
	{
		m_debugBeacons[5] = !m_debugBeacons[5];
	}

	for( BtU32 i=0; i<6; i++ )
	{
		if( m_debugBeacons[i] )
		{
			AddBeacon( i, i );
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetBeacon

//static
ShBeaconNode &ShBeacon::GetBeacon( BtU32 i )
{
    return m_beacons[i];
}

////////////////////////////////////////////////////////////////////////////////
// GetNumBeacons

//static
BtU32 ShBeacon::GetNumBeacons()
{
	return m_beacons.GetNumItems();
}

////////////////////////////////////////////////////////////////////////////////
// FindBeacon

ShBeaconNode *ShBeacon::FindBeacon( ShBeaconNode &node )
{
    BtU32 numBeacons = GetNumBeacons();
    
    for( BtU32 i=0; i<numBeacons; i++ )
    {
        ShBeaconNode &beacon = GetBeacon(i);
        if( ( beacon.m_major == node.m_major ) && ( beacon.m_minor == node.m_minor ) )
        {
            return &beacon;
        }
    }
    return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// AddBeacon

void ShBeacon::AddBeacon( BtU32 major, BtU32 minor )
{
    BtU32 numItems = m_beacons.GetNumItems();
    
    if( numItems < m_beacons.GetMaxItems() )
    {
        ShBeaconNode beacon;
        beacon.m_major = major;
        beacon.m_minor = minor;
        m_beacons.Add( beacon );
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetNumBeaconAlerts

//static
BtU32 ShBeacon::GetNumBeaconAlerts()
{
	return m_beaconAlert.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopAlert

//static
ShBeaconNode ShBeacon::PopAlert()
{
	return m_beaconAlert.Pop();
}

////////////////////////////////////////////////////////////////////////////////
// PushAlert

//static
void ShBeacon::PushAlert( ShBeaconNode node )
{
	m_beaconAlert.Push( node );
}

////////////////////////////////////////////////////////////////////////////////
// PeekAlert

//static
ShBeaconNode ShBeacon::PeekAlert()
{
	return m_beaconAlert.Peek();
}
