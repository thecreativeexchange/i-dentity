////////////////////////////////////////////////////////////////////////////////
// HlLines.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "RsRenderTarget.h"

class RsMaterial;
class BaArchive;

// Class definition
class HlLines
{
public:

	// Public functions
	static void							Setup( RsMaterial *pMaterial2,
											   RsMaterial *pMaterial3,
											   RsMaterial *pMaterial3NoZTest = BtNull );

	static void							SetZTest( BtBool isEnabled );
	static void							SetCrossColour( const RsColour &colour1, const RsColour &colour2, const RsColour &colour3 );

	static void							RenderLine( const MtVector2 &v2First, const MtVector2 &v2Second, const RsColour &colour, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderLine( const MtVector3 &v3Start, const MtVector3 &v3End, const RsColour &colour, BtU32 sortOrder = MaxSortOrders - 1);

	static void							RenderCross( const MtVector2 &v2Position, BtFloat radius = 5.0f, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderCross( const MtVector3 &v3Position, BtFloat radius = 0.5f, BtU32 sortOrder = MaxSortOrders - 1 );
	static void							RenderArrow( const MtVector2 &v2A, const MtVector2 &v2B, BtFloat radius = 0.5f, BtU32 sortOrder = MaxSortOrders - 1 );

	// Accessors

private:

	// Private functions

	// Private members
	static RsMaterial					*m_pMaterial2;
	static RsMaterial					*m_pMaterial3;
	static RsMaterial					*m_pMaterial3NoZTest;
	static BtBool						 m_ztest;
	static RsColour						 m_colour1;
	static RsColour						 m_colour2;
	static RsColour						 m_colour3;
};
