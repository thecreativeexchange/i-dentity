////////////////////////////////////////////////////////////////////////////////
// ShDialog

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

enum ShDialogActionType
{
    ShDialogActionType_Open,
    ShDialogActionType_MAX,
};

// Class definition
class ShDialog
{
public:
    
    // Access
    static void                                 PushAction( ShDialogActionType action );
    static ShDialogActionType					PopAction();
    static BtU32								GetNumItems();
    
    // State functions
    
    // Public functions
private:
    
    static BtQueue<ShDialogActionType, 128>		 m_actions;
};

