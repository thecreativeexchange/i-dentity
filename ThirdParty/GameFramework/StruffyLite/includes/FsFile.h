//----------------------------------------------------------------------------
// FsFile.h

#pragma once
#include "BtTypes.h"
#include "MtMath.h"
#include <stdio.h>

const BtU32 MaxPath = 260;

////////////////////////////////////////////////////////////////////////////////
// FsFile

enum eFsMode
{
	eFsMode_Read,
	eFsMode_Write,
	eFsMode_WriteAscii,
	eFsMode_Append,
	eFsMode_Max,
};

class FsFile
{
public:
	// Constructor
	FsFile();

	// Open a file
	void								Open( BtChar* pFilename, eFsMode mode );

	// Read data from a file
	BtU32								Read( BtU8* pMemory, BtU32 nSize );

	// Read data from a file
	template <typename T> BtU32			Read( T& );

	// Write data to a file
	void								Write( BtU8* pMemory, BtU32 nSize );

	// Write data to a file
	template <typename T> void			Write( T& );

	// Get the size of a file
	BtU32								GetSize();

	// Skip a number of bytes
	void								Skip( BtU32 nBytes );

	// Close a file
	void								Close();

	BtBool								IsEndOfFile();

	BtBool								IsOpen();

	BtU32								Position();

private:
		
	FILE*								m_fileHandle;
	BtBool								m_bOpened;
	eFsMode								m_mode;
};

////////////////////////////////////////////////////////////////////////////////
// Read data from a file

template <typename T> inline BtU32 FsFile::Read( T& memory )
{
	return Read( (BtU8*) &memory, sizeof(T) );
}

////////////////////////////////////////////////////////////////////////////////
// Write data to a file

template <typename T> inline void FsFile::Write( T& memory )
{
	Write( (BtU8*) &memory, sizeof(T) );
}
