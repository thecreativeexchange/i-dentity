////////////////////////////////////////////////////////////////////////////////
// HlDebug.cpp

// Includes
#include <stdio.h>
#include "HlDebug.h"
#include "HlFont.h"
#include "BtString.h"
#include "RsRenderTarget.h"
#include "RsMaterial.h"
#include "BaArchive.h"
#include "UiKeyboard.h"

BtArray<HlItem, 1024> HlDebug::m_items;
RsMaterial *HlDebug::m_pWhite2;
BtS32 DebugIndex = 0;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlDebug::Setup( BaArchive *pArchive )
{
	m_pWhite2 = pArchive->GetMaterial( "white2" );
}

////////////////////////////////////////////////////////////////////////////////
// GetIndex

BtU32 HlDebug::GetIndex()
{
	return DebugIndex;
}

////////////////////////////////////////////////////////////////////////////////
// Reset

void HlDebug::Reset()
{
	m_items.Empty();
}

////////////////////////////////////////////////////////////////////////////////
// AddNode

void HlDebug::AddNode( BtChar *name )
{
	HlItem item;
	BtStrCopy( item.m_name, 32, name );
	item.m_type = HlDebugNode;
}

////////////////////////////////////////////////////////////////////////////////
// AddFloat

void HlDebug::AddFloat( BtChar *name, BtFloat *pValue, BtFloat minFloat, BtFloat maxFloat )
{
	HlItem item;
	BtStrCopy( item.m_name, 32, name );
	item.m_pFloat = pValue;
	item.m_type = HlDebugFloat;
	item.m_minFloat = minFloat;
	item.m_maxFloat = maxFloat;
	m_items.Add( item );
}

////////////////////////////////////////////////////////////////////////////////
// AddInteger

void HlDebug::AddInteger( BtChar *name, BtS32 *pValue, BtS32 minInt, BtS32 maxInt, BtS32 diffInt )
{
	HlItem item;
	BtStrCopy( item.m_name, 32, name );
	item.m_pInteger = pValue;
	item.m_type = HlDebugInteger;
	item.m_minInt = minInt;
	item.m_maxInt = maxInt;
	item.m_diffInt = diffInt;
	m_items.Add( item );
}

////////////////////////////////////////////////////////////////////////////////
// Update

void HlDebug::Update()
{
	HlItem &item = m_items[DebugIndex];
	BtFloat diff = ( item.m_maxFloat - item.m_minFloat ) / 1000.0f;
		
	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_MINUS ) )
	{
		if( item.m_type == HlDebugFloat )
		{
			*item.m_pFloat -= diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger -= item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}
	else if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_PLUS ) )
	{
		if( item.m_type == HlDebugFloat )
		{
			*item.m_pFloat += diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger += item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}
	else if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_MINUS ) && UiKeyboard::pInstance()->IsHeld( UiKeyCode_RSHIFT ) )
	{
		diff = diff * 10.0f;

		if( item.m_type == HlDebugFloat )
		{
			*item.m_pFloat -= diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger -= item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}
	else if( UiKeyboard::pInstance()->IsHeld( UiKeyCode_PLUS ) && UiKeyboard::pInstance()->IsHeld( UiKeyCode_RSHIFT ) )
	{
		diff = diff * 10.0f;

		if( item.m_type == HlDebugFloat )
		{
			*item.m_pFloat += diff;

			MtClamp( *item.m_pFloat, item.m_minFloat, item.m_maxFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			*item.m_pInteger += item.m_diffInt;

			MtClamp( *item.m_pInteger, item.m_minInt, item.m_maxInt );
		}
	}

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_UP ) )
	{
		--DebugIndex;

		if( DebugIndex < 0 )
		{
			DebugIndex = 0;
		}
	}

	if( UiKeyboard::pInstance()->IsPressed( UiKeyCode_DOWN ) )
	{
		++DebugIndex;

		BtU32 last = m_items.GetNumItems() - 1;
		if( DebugIndex > last )
		{
			DebugIndex = last;
		}	
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlDebug::Render( const RsColour &textColour )
{
	BtU32 numItems = m_items.GetNumItems();
	
	MtVector2 v2Position( 100, 100.0f );

	for( BtU32 i=0; i<numItems; i++ )
	{
		HlItem &item = m_items[i];

		BtChar text[64];

		if( item.m_type == HlDebugNode )
		{
			sprintf( text, "%s", item.m_name);
		}
		else if( item.m_type == HlDebugFloat )
		{
			sprintf( text, "%s %.5f", item.m_name, *item.m_pFloat );
		}
		else if( item.m_type == HlDebugInteger )
		{
			sprintf( text, "%s %d", item.m_name, *item.m_pInteger );
		}
		
		if( DebugIndex == i )
		{
			sprintf( text + strlen(text), "*" );
		}

		HlFont::Render( v2Position, text, textColour, MaxSortOrders- 1 );

		v2Position.y += 15.0f;
	}
}