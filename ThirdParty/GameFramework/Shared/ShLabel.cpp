////////////////////////////////////////////////////////////////////////////////
// ShLabel

// Includes
#include "ShLabel.h"
#include "RsUtil.h"
#include "BtString.h"

//static

// Public functions
BtQueue<ShLabelItem, MaxLabelItems> ShLabel::m_actions;

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShLabel::PushAction( const BtChar *description, BtU32 labelID )
{
	ShLabelItem action;
    action.m_labelID = labelID;
	BtStrCopy( action.m_action, (BtU32)strlen(description), description );
	m_actions.Push(action);
}

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShLabel::PushAction( ShLabelItem action )
{
	if( GetNumItems() < MaxLabelItems - 2 )
	{
		m_actions.Push( action );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

//static
BtU32 ShLabel::GetNumItems()
{
    return m_actions.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopAction

//static
ShLabelItem ShLabel::PopAction()
{
    ShLabelItem action;
    action = m_actions.Pop();
    return action;
}
