////////////////////////////////////////////////////////////////////////////////
// ApConfig.cpp

// Includes
#include "ApConfig.h"
#include "BtString.h"
#include "BtPrint.h"
#include "ErrorLog.h"
#include "stdio.h"
#include "stdlib.h"

////////////////////////////////////////////////////////////////////////////////
// Statics
ApConfig config;
ApPlatform ApConfig::m_platform = ApPlatform_Max;
BtChar ApConfig::m_resourcePath[256];
BtChar ApConfig::m_extension[256];
BtChar ApConfig::m_documents[256];
BtChar ApConfig::m_title[256];
ApDevice ApConfig::m_device = ApDevice_Max;
BtBool ApConfig::m_isDebug = BtFalse;
BtBool ApConfig::m_isCheatMode = BtFalse;
BtBool ApConfig::m_isSimulator = BtFalse;

////////////////////////////////////////////////////////////////////////////////
// Constructor

ApConfig::ApConfig()
{
	BtStrCopy( m_documents, 256, "" );
    BtStrCopy( m_resourcePath, 256, "" );
	BtStrCopy( m_title, 256, "Struffy" );
    m_isSimulator = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// IsSimulator

BtBool ApConfig::IsSimulator()
{
    return m_isSimulator;
}

////////////////////////////////////////////////////////////////////////////////
// SetSimulator

void ApConfig::SetSimulator( BtBool isSimulator )
{
    m_isSimulator = isSimulator;
}

////////////////////////////////////////////////////////////////////////////////
// IsCheatMode

BtBool ApConfig::IsCheatMode()
{
	return m_isCheatMode;
}

////////////////////////////////////////////////////////////////////////////////
// SetCheatMode

void ApConfig::SetCheatMode( BtBool isCheatMode )
{
	m_isCheatMode = isCheatMode;
}

////////////////////////////////////////////////////////////////////////////////
// IsDebug

BtBool ApConfig::IsDebug()
{
    return m_isDebug;
}

////////////////////////////////////////////////////////////////////////////////
// SetDebug

void ApConfig::SetDebug( BtBool isDebug )
{
    m_isDebug = isDebug;
}

////////////////////////////////////////////////////////////////////////////////
// SetTitle

void ApConfig::SetTitle( const BtChar* title )
{
	BtStrCopy(m_title, sizeof(m_title), title);
}

////////////////////////////////////////////////////////////////////////////////
// GetTitle

const BtChar *ApConfig::GetTitle()
{
	return m_title;
}

////////////////////////////////////////////////////////////////////////////////
// GetPlatform

//static
ApPlatform ApConfig::GetPlatform()
{
	return m_platform;
}

//static
const BtChar *ApConfig::GetDelimitter()
{
	if( ApConfig::GetDevice() == ApDevice_WIN )
	{
		return "\\";
	}
	else
	{
		return "/";
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetPlatformName

//static
const BtChar *ApConfig::GetPlatformName()
{
	switch( m_platform )
	{
		case ApPlatform_GLES:
			return "GLES";

		case ApPlatform_DX11:
			return "WinDX";

		case ApPlatform_WinGL:
			return "WinGL";
            
        default:
            return "Platform not set";
	}
	return "Platform not set";
}

////////////////////////////////////////////////////////////////////////////////
// SetOrientation

//static
void ApConfig::SetPlatform( ApPlatform platform )
{
	m_platform = platform;
}

///////////////////////////////////////////////////////////////////////////////
// SetExtension

void ApConfig::SetExtension( const BtChar* pExtension )
{
	BtStrCopy( m_extension, 256, pExtension );
}

///////////////////////////////////////////////////////////////////////////////
// SetResourcePath

void ApConfig::SetResourcePath( const BtChar* pResourcePath )
{
	BtStrCopy( m_resourcePath, 256, pResourcePath );

    BtPrint( "%s\n", m_resourcePath );
	CheckResourcePath();
}

///////////////////////////////////////////////////////////////////////////////
// SetDocuments

void ApConfig::SetDocuments( const BtChar* documents )
{
	BtStrCopy( m_documents, 256, documents );
}

///////////////////////////////////////////////////////////////////////////////
// CheckResourcePath

void ApConfig::CheckResourcePath()
{
	if( ApConfig::GetPlatform() == ApPlatform_DX11 )
	{
		BtChar filename[256];
		sprintf( filename, "%sArchive.WinDX", GetResourcePath() );
		FILE *f = fopen( filename, "rb" );
		if( f != BtNull )
		{
			fclose(f);
		}
		else
		{
			BtStrCopy( m_resourcePath, 256, "" );
		}
	}
	else if(ApConfig::GetPlatform() == ApPlatform_WinGL)
	{
		BtChar filename[256];
		sprintf(filename, "%sArchive.WinGL", GetResourcePath());
		FILE *f = fopen(filename, "rb");
		if(f != BtNull)
		{
			fclose(f);
		}
		else
		{
			BtStrCopy(m_resourcePath, 256, "");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetResourcePath

const BtChar* ApConfig::GetResourcePath()
{
	return m_resourcePath;
}

///////////////////////////////////////////////////////////////////////////////
// GetDocuments

const BtChar* ApConfig::GetDocuments()
{
	return m_documents;
}

///////////////////////////////////////////////////////////////////////////////
// GetExtension

const BtChar* ApConfig::GetExtension()
{
	return m_extension;
}

///////////////////////////////////////////////////////////////////////////////
// GetDevice

ApDevice ApConfig::GetDevice()
{
    return m_device;
}

///////////////////////////////////////////////////////////////////////////////
// IsWin

BtBool ApConfig::IsWin()
{
    if( m_device == ApDevice_WIN )
	{
		return BtTrue;
	}
	return BtFalse;
}

///////////////////////////////////////////////////////////////////////////////
// SetDevice

void ApConfig::SetDevice( ApDevice device )
{
    m_device = device;
}
