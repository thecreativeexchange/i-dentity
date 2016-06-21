////////////////////////////////////////////////////////////////////////////////
// FsFile.cpp

#include "FsFile.h"
#include "ErrorLog.h"
#include "BtBase.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

FsFile::FsFile()
{
	m_fileHandle = BtNull;
	m_bOpened = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Open a file

void FsFile::Open( BtChar* pFilename, eFsMode mode )
{
	m_mode = mode;

	if( mode == eFsMode_Read )
	{
		m_fileHandle = fopen( pFilename, "rb" );
	}
	else if( mode == eFsMode_Write )
	{
		m_fileHandle = fopen( pFilename, "wb" );
	}
	else if( mode == eFsMode_WriteAscii )
	{
		m_fileHandle = fopen( pFilename, "w" );
	}
	else if( mode == eFsMode_Append )
	{
		m_fileHandle = fopen( pFilename, "ab" );
	}

	if( m_fileHandle == BtNull )
	{
		m_bOpened = BtFalse;

        ErrorLog::Printf( "Could not open file %s", pFilename );
	}
	else
	{
		m_bOpened = BtTrue;
	}
}

////////////////////////////////////////////////////////////////////////////////
// bOpened

BtBool FsFile::IsOpen()
{
	return m_bOpened;
}

////////////////////////////////////////////////////////////////////////////////
// Read data to a file

BtU32 FsFile::Read( BtU8* pMemory, BtU32 nSize )
{
	if( m_bOpened == BtFalse )
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Read failed as no file is open." );
	}

	BtAssert( m_mode == eFsMode_Read );

	// Read the bytes from the file
	BtU32 nBytesRead = (BtU32)fread( pMemory, 1, nSize, m_fileHandle );

	// Return the number of bytes read
	return nBytesRead;
}

////////////////////////////////////////////////////////////////////////////////
// Write data to a file

void FsFile::Write( BtU8* pMemory, BtU32 nSize )
{
	if( m_bOpened == BtFalse )
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Write failed as no file is open." );
	}
    else
    {
        fwrite( pMemory, 1, nSize, m_fileHandle );
    }
}

////////////////////////////////////////////////////////////////////////////////
// GetSize

BtU32 FsFile::GetSize()
{
	if( m_bOpened == BtTrue )
	{
		BtU32 nCurrentPosition = Position();

		fseek( m_fileHandle, 0, SEEK_END );

		BtU32 nEndPosition = Position();

		fseek( m_fileHandle, nCurrentPosition, SEEK_SET );

		return nEndPosition;
	}
	else
	{
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Skip

void FsFile::Skip( BtU32 nBytes )
{
	fseek( m_fileHandle, nBytes, SEEK_CUR );
}

////////////////////////////////////////////////////////////////////////////////
// Position

BtU32 FsFile::Position()
{
	return (BtU32)ftell( m_fileHandle );
}

////////////////////////////////////////////////////////////////////////////////
// Close a file

void FsFile::Close()
{
	if( m_bOpened == BtFalse )
	{
		//LgErrorLog::Fatal_Printf( "FsFile: Close failed as no file is open." );
	}

	// Close the handle
	fclose( m_fileHandle );

	// Flag the file as closed
	m_bOpened = BtFalse;
}

////////////////////////////////////////////////////////////////////////////////
// Close a file

BtBool FsFile::IsEndOfFile()
{
	if( m_bOpened == BtFalse )
	{
		return BtFalse;
	}

	if( feof( m_fileHandle) )
	{
		return BtTrue;
	}
	return BtFalse;
}