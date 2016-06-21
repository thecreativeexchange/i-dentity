////////////////////////////////////////////////////////////////////////////////
// ShAnalytics

// Includes
#include "ShAnalytics.h"
#include "RsUtil.h"
#include "BtString.h"

//static

// Public functions
BtQueue<ShAnalyticsItem, MaxItems> ShAnalytics::m_actions;

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShAnalytics::PushAction( const BtChar *description )
{
	ShAnalyticsItem action;
	BtStrCopy( action.m_action, (BtU32)strlen(description), description );
	m_actions.Push(action);
}

////////////////////////////////////////////////////////////////////////////////
// PushAction

//static
void ShAnalytics::PushAction( ShAnalyticsItem action )
{
	if( GetNumItems() < MaxItems - 2 )
	{
		m_actions.Push( action );
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetNumItems

//static
BtU32 ShAnalytics::GetNumItems()
{
    return m_actions.GetItemCount();
}

////////////////////////////////////////////////////////////////////////////////
// PopAction

//static
ShAnalyticsItem ShAnalytics::PopAction()
{
    ShAnalyticsItem action;
    action = m_actions.Pop();
    return action;
}
