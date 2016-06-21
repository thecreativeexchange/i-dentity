////////////////////////////////////////////////////////////////////////////////
// HlCircles.cpp

// Includes
#include "BaArchive.h"
#include "HlCircles.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "RsShader.h"

//static
RsMaterial *HlCircles::m_pMaterial2 = BtNull;
RsColour HlCircles::m_insideColour( 1.0f, 1.0f, 1.0f, 1.0f );
RsColour HlCircles::m_outsideColour( 1.0f, 1.0f, 1.0f, 1.0f );

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlCircles::Setup( RsMaterial *pMaterial2 )
{
	m_pMaterial2 = pMaterial2;
}

////////////////////////////////////////////////////////////////////////////////
// RenderCircle

void HlCircles::RenderCircle( const MtVector2 &v2A, BtFloat radius, BtU32 sortOrder )
{
    int a=0;
    a++;
}