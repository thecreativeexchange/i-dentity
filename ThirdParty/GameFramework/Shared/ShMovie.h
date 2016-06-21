////////////////////////////////////////////////////////////////////////////////
// ShMovie

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

enum ShMovieActionType
{
	ShMovieActionType_Play,
	ShMovieActionType_Stop,
	ShMovieActionType_MAX,
};

struct ShMovieAction
{
    ShMovieActionType                       m_type;
    BtChar                                  m_buffer[64];
};

// Class definition
class ShMovie
{
public:

	// Access
	static void                             PushAction( ShMovieAction action );
	static ShMovieAction                    PopAction();
    static BtU32                            GetNumItems();
    
	// State functions
    
	// Public functions
private:

    static BtQueue<ShMovieAction, 128>      m_actions;
};
