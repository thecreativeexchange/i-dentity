////////////////////////////////////////////////////////////////////////////////
// HlMouse.cpp

// Includes
#include "BaArchive.h"
#include "HlMouse.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "ShTouch.h"
#include "RsSprite.h"
#include "ApConfig.h"
#include "HlLines.h"
#include "UiKeyboard.h"

//static
RsSprite *HlMouse::m_pSprite = BtNull;
MtVector2 HlMouse::m_v2Position[32];
BtBool HlMouse::m_isVisible[32];
BtFloat HlMouse::m_scalar = 1.0f;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlMouse::Setup( BaArchive *pUtilityArchive )
{
	m_pSprite = pUtilityArchive->GetSprite("MousePointers");
	BtAssert( m_pSprite );
}

////////////////////////////////////////////////////////////////////////////////
// SetScalar

void HlMouse::SetScalar( BtFloat s )
{
	m_scalar = s;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlMouse::Render()
{
	if(UiKeyboard::pInstance()->IsPressed(UiKeyCode_F11))
	{
		return;
	}

	if( ApConfig::IsWin() )
	{
		static BtBool isReset = BtTrue;
		if( isReset )
		{
			isReset = BtFalse;

			// Render the mouse pointers
			for (BtU32 i = 0; i < MaxTouches; i++)
			{
				m_v2Position[i] = ShTouch::GetPosition(i);
			}
		}

		// Render the mouse pointers
		for( BtU32 i = 0; i < MaxTouches; i++ )
		{
			if( ShTouch::IsHeld(i) )
			{
				m_isVisible[i] = BtTrue;
			}

			MtVector2 v2Position = ShTouch::GetPosition(i);

			MtVector2 v2Delta = m_v2Position[i] - v2Position;

			if (v2Delta.GetLength() )
			{
				m_isVisible[i] = BtTrue;
			}
			
			if( m_isVisible[i] )
			{
				BtFloat alpha = 1.0f;

				if( ShTouch::IsHeld(i) == BtFalse )
				{
					alpha = 0.25f;
				}
				else
				{
					alpha = 1.0f;
				}

				const BtU32 sprite = 1;
				MtVector2 v2Dimension = m_pSprite->GetDimension( sprite );

				v2Position.x -= v2Dimension.x * ( 0.40f * m_scalar );
				v2Position.y -= v2Dimension.y * ( 0.15f * m_scalar );

				//HlLines::RenderCross( v2Position, 5.0f, MaxSortOrders-1 );

				m_pSprite->Render( BtFalse,
					v2Position,
					v2Dimension * m_scalar,
					sprite,
					0,
					RsColour(1.0f, 1.0f, 1.0f, alpha),
					MaxSortOrders - 1 );
			}
		}
	}
}
