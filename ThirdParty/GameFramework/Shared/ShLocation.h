////////////////////////////////////////////////////////////////////////////////
// ShLocation

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtTypes.h"

// Class definition
class ShLocation
{
public:

	// Constructor
	ShLocation();

	// Accessors
    static void                 SetSatellite( BtBool isSatellite );
    static BtBool               IsSatellite();

private:

	// Private members
    static BtBool               m_isSatellite;
};
