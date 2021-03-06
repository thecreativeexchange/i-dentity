////////////////////////////////////////////////////////////////////////////////
// ShMovie

// Includes
#include "ShMovie.h"
#include "RsUtil.h"

////////////////////////////////////////////////////////////////////////////////
// statics

//static

// Public functions
BtQueue<ShMovieAction, 128> ShMovie::m_actions;

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShMovie::PushAction( ShMovieAction action )
{
    m_actions.Push( action );
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

//static
BtU32 ShMovie::GetNumItems()
{
    return m_actions.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopAction

//static
ShMovieAction ShMovie::PopAction()
{
    ShMovieAction action;
    action = m_actions.Pop();
    return action;
}

