////////////////////////////////////////////////////////////////////////////////
// ShConnectivity

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"

// Class definition
class ShConnectivity
{
public:

	// Access
	static BtBool				GetConnectivity();
	static void					SetConnectivity( BtBool isConnectivity );
    
	// State functions
    
	// Public functions
private:

    static BtBool				m_isConnectivity;
};
