////////////////////////////////////////////////////////////////////////////////
// ErrorLog.cpp

// Includes
#include <stdio.h>
#include <string.h>
#include <cctype>
#include <stdarg.h>
#include <stdio.h>
#include "BtString.h"
#include "BtBase.h"

#ifdef DX11
#define DX11_LEAN_AND_MEAN
#define INITGUID
#include <process.h>
#include <windows.h>
#endif

#include "ErrorLog.h"

const BtU32 MaxErrorBuffer = 1024;

// Private members
BtChar	m_szFilename[256];
FILE*	m_handle;

BtChar buffer1[MaxErrorBuffer];
BtChar buffer2[MaxErrorBuffer];

////////////////////////////////////////////////////////////////////////////////
// Filename

void ErrorLog::Filename( const BtChar* pFilename )
{
	BtStrCopy( m_szFilename, 256, pFilename );
}

////////////////////////////////////////////////////////////////////////////////
// Create

void ErrorLog::Create()
{
	FILE* f;
	f = fopen( m_szFilename, "w" );
	fclose(f);
}

////////////////////////////////////////////////////////////////////////////////
// Printf

void ErrorLog::Printf( const BtChar* pFormat, ... )
{
	va_list marker;
	va_start( marker, pFormat );
	vsprintf( buffer2, pFormat, marker );
	Print( buffer2 );
	va_end ( marker );
}

////////////////////////////////////////////////////////////////////////////////
// Fatal_Printf

void ErrorLog::Fatal_Printf( const BtChar* pFormat, ... )
{
	va_list marker;
	va_start( marker, pFormat );
	vsprintf( buffer1, pFormat, marker );
	Print( buffer1 );
	Print( "\r\n" );
	va_end ( marker );

#ifdef DX11
	exit(0);
#endif // _DEBUG
}

////////////////////////////////////////////////////////////////////////////////
// Print

void ErrorLog::Print( const BtChar* pString )
{
	printf( "%s\n", pString );

#ifdef DX11
	OutputDebugString( pString );
#endif
}
