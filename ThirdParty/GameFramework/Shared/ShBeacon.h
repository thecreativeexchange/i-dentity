////////////////////////////////////////////////////////////////////////////////
// ShBeacon

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"
#include "BtArray.h"
#include "BtQueue.h"

const BtU32 MaxDebugBeacons = 6;

struct ShBeaconNode
{
	BtU32								m_minor;
	BtU32								m_major;
};

// Class definition
class ShBeacon
{
public:

	// Constructor
	ShBeacon();

	// Accessors
	static void							Update();
	static void							Reset();
	static void							AddBeacon( BtU32 major, BtU32 minor );
    static ShBeaconNode                &GetBeacon( BtU32 i );
	static BtU32						GetNumBeacons();
    static ShBeaconNode                *FindBeacon( ShBeaconNode &node );
    
    static BtU32                        GetNumBeaconAlerts();
    static ShBeaconNode                 PopAlert();
    static ShBeaconNode                 PeekAlert();
    static void                         PushAlert( ShBeaconNode node );
    static BtU32                        GetNumAlerts();

	static BtBool						IsEnabled();
	static void							SetEnabled( BtBool isEnabled );

private:

	// Private members
	static BtBool						m_debugBeacons[MaxDebugBeacons];
    static BtArray<ShBeaconNode, 32>	m_beacons;
    static BtQueue<ShBeaconNode, 32>    m_beaconAlert;
	static BtBool						m_isEnabled;
};
