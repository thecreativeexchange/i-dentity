////////////////////////////////////////////////////////////////////////////////
// HlLines.cpp

// Includes
#include "BaArchive.h"
#include "HlLines.h"
#include "MtVector2.h"
#include "RsColour.h"
#include "RsMaterial.h"
#include "RsRenderTarget.h"
#include "RsTexture.h"
#include "RsUtil.h"
#include "RsShader.h"

//static
RsMaterial *HlLines::m_pMaterial2 = BtNull;
RsMaterial *HlLines::m_pMaterial3 = BtNull;
RsMaterial *HlLines::m_pMaterial3NoZTest = BtNull;
BtBool HlLines::m_ztest = BtFalse;
RsColour HlLines::m_colour1;
RsColour HlLines::m_colour2;
RsColour HlLines::m_colour3;

////////////////////////////////////////////////////////////////////////////////
// Setup

void HlLines::Setup( RsMaterial *pMaterial2,
					 RsMaterial *pMaterial3,
					 RsMaterial *pMaterial3NoZTest )
{
	m_pMaterial2 = pMaterial2;
	m_pMaterial3 = pMaterial3;
	m_pMaterial3NoZTest = pMaterial3NoZTest;
	m_colour1 = RsColour::GreenColour();
	m_colour2 = RsColour::WhiteColour();
	m_colour3 = RsColour::BlueColour();
}

////////////////////////////////////////////////////////////////////////////////
// SetZTest

void HlLines::SetZTest( BtBool isEnabled )
{
	m_ztest = isEnabled;
}

////////////////////////////////////////////////////////////////////////////////
// RenderLine

void HlLines::RenderLine( const MtVector3 &v3Start, const MtVector3 &v3End, const RsColour &colour, BtU32 sortOrder )
{
	// Allocate vertex
	RsVertex3 vertex[2];

	BtU32 col = colour.asWord();

	// Copy these into vertex
	vertex[0].m_v3Position = v3Start;
	vertex[0].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[0].m_v3Normal = MtVector3( 0, 0, 1 );
	vertex[0].m_colour = col;

	vertex[1].m_v3Position = v3End;
	vertex[1].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[1].m_v3Normal = MtVector3( 0, 0, 1 );
	vertex[1].m_colour = col;

	if (m_ztest == BtTrue)
	{
		m_pMaterial3->Render(RsPT_LineList, vertex, 2, sortOrder);
	}
	else
	{
		m_pMaterial3NoZTest->Render(RsPT_LineList, vertex, 2, sortOrder);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Render

void HlLines::RenderLine( const MtVector2 &v2First, const MtVector2 &v2Second, const RsColour &colour, BtU32 sortOrder)
{
	BtAssert( RsShader::GetCurrent() );

	// Allocate vertex
	RsVertex2 vertex[2];

	BtU32 col = colour.asWord();

	// Copy these into vertex
	vertex[0].m_v2Position = MtVector2( v2First.x, v2First.y );
	vertex[0].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[0].m_colour = col;

	vertex[1].m_v2Position = MtVector2( v2Second.x, v2Second.y );
	vertex[1].m_v2UV = MtVector2( 0.5f, 0.5f );
	vertex[1].m_colour = col;

	m_pMaterial2->Render( RsPT_LineList, vertex, 2, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// SetCrossColour

void HlLines::SetCrossColour( const RsColour &colour1, const RsColour &colour2, const RsColour &colour3 )
{
	m_colour1 = colour1;
	m_colour2 = colour2;
	m_colour3 = colour3;
}

////////////////////////////////////////////////////////////////////////////////
// RenderCross

void HlLines::RenderCross( const MtVector2 &v2Position, BtFloat radius, BtU32 sortOrder )
{
	MtVector2 v2A = v2Position - MtVector2(0, radius);
	MtVector2 v2B = v2Position + MtVector2(0, radius);
	HlLines::RenderLine(v2A, v2B, m_colour1, sortOrder );

	v2A = v2Position - MtVector2(radius, 0);
	v2B = v2Position + MtVector2(radius, 0);
	HlLines::RenderLine( v2A, v2B, m_colour2, sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderArrow

void HlLines::RenderArrow( const MtVector2 &v2A, const MtVector2 &v2B, BtFloat radius, BtU32 sortOrder )
{
	HlLines::RenderLine( v2A, v2B, RsColour::BlackColour(), sortOrder );
}

////////////////////////////////////////////////////////////////////////////////
// RenderCross

void HlLines::RenderCross( const MtVector3 &v3Position, BtFloat radius, BtU32 sortOrder )
{
	MtVector3 v3A = v3Position - MtVector3(0, radius, 0);
	MtVector3 v3B = v3Position + MtVector3(0, radius, 0);
	RenderLine(v3A, v3B, m_colour1, sortOrder);

	v3A = v3Position - MtVector3(radius, 0, 0);
	v3B = v3Position + MtVector3(radius, 0, 0);
	RenderLine(v3A, v3B, m_colour2, sortOrder);

	v3A = v3Position - MtVector3(0, radius, 0);
	v3B = v3Position + MtVector3(0, radius, 0);
	RenderLine(v3A, v3B, m_colour3, sortOrder);

	v3A = v3Position - MtVector3(0, 0, radius );
	v3B = v3Position + MtVector3(0, 0, radius );
	RenderLine(v3A, v3B, RsColour::BlackColour(), sortOrder);
}