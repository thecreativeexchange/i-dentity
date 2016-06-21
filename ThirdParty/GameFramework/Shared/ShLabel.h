////////////////////////////////////////////////////////////////////////////////
// ShLabel

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

const BtU32 MaxLabelItems = 16;

struct ShLabelItem
{
    BtU32                                   m_labelID;
    BtChar                                  m_action[256];
};

// Class definition
class ShLabel
{
public:

	// Access
	static void                             PushAction( const BtChar *desc, BtU32 labelID = 0 );
	static ShLabelItem                      PopAction();
    static BtU32                            GetNumItems();
    static BtU32                            GetNumAlerts();
    
	// State functions
    
	// Public functions
    
private:
    
    static void                             PushAction( ShLabelItem action );
	
    static BtQueue<ShLabelItem, MaxLabelItems>	m_actions;
};
