////////////////////////////////////////////////////////////////////////////////
// ShLocation.h

// Includes
#include "ShLocation.h"

// Static variables
BtBool ShLocation::m_isSatellite = BtFalse;

//static
void ShLocation::SetSatellite( BtBool isSatellite )
{
    m_isSatellite = isSatellite;
}

//static
BtBool ShLocation::IsSatellite()
{
    return m_isSatellite;
}
