////////////////////////////////////////////////////////////////////////////////
// ShMusic

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

enum ShMusicActionType
{
	ShMusicActionType_Play,
	ShMusicActionType_Stop,
    ShMusicActionType_DidFinish,
	ShMusicActionType_MAX,
};

struct ShMusicAction
{
    ShMusicActionType                       m_type;
    BtChar                                  m_buffer[64];
};

// Class definition
class ShMusic
{
public:

	// Access
	static void                             PushAction( ShMusicAction action );
    static void                             PushAlert( ShMusicAction action );
	static ShMusicAction                    PopAction();
    static ShMusicAction                    PopAlert();
    static BtU32                            GetNumItems();
    static BtU32                            GetNumAlerts();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShMusicAction, 128>      m_actions;
    static BtQueue<ShMusicAction, 128>      m_alerts;
};
