////////////////////////////////////////////////////////////////////////////////
// HlCircles.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtVector3.h"
#include "RsColour.h"
#include "RsRenderTarget.h"

class RsMaterial;
class BaArchive;

// Class definition
class HlCircles
{
public:

	// Public functions
	static void							Setup( RsMaterial *pMaterial2 );
	static void							RenderCircle( const MtVector2 &v2A, BtFloat radius = 0.5f, BtU32 sortOrder = MaxSortOrders - 1 );

	// Accessors

private:

	// Private functions

	// Private members
    static RsMaterial					*m_pMaterial2;
    static RsColour						 m_insideColour;
    static RsColour                      m_outsideColour;
};
