////////////////////////////////////////////////////////////////////////////////
// ShBadge

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

enum ShBadgeAction
{
	ShBadgeAction_Add,
    ShBadgeAction_Remove,
	ShBadgeAction_MAX,
};

// Class definition
class ShBadge
{
public:

	// Access
	static void                             PushAction( ShBadgeAction action );
	static ShBadgeAction                    PopAction();
	static ShBadgeAction                    PeekAction();
    static BtU32                            GetNumItems();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShBadgeAction, 128>      m_actions;
};
