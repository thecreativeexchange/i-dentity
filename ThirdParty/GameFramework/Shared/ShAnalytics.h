////////////////////////////////////////////////////////////////////////////////
// ShAnalytics

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

const BtU32 MaxItems = 1024;

struct ShAnalyticsItem
{
    BtChar                                  m_action[32];
};

// Class definition
class ShAnalytics
{
public:

	// Access
	static void                             PushAction( const BtChar *desc );
	static ShAnalyticsItem				    PopAction();
    static BtU32                            GetNumItems();
    static BtU32                            GetNumAlerts();
    
	// State functions
    
	// Public functions
    
private:
    
    static void                             PushAction( ShAnalyticsItem action );
	
    static BtQueue<ShAnalyticsItem, MaxItems>	m_actions;
};
