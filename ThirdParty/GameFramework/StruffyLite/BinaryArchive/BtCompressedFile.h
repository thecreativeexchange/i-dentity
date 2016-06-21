////////////////////////////////////////////////////////////////////////////////
// LBtCompressedFile

// Include guard
#ifndef __LBtCompressedFile_H__
#define __LBtCompressedFile_H__

// Includes
#include "BtTypes.h"
#include "BaFileData.h"
#include "zlib.h"
#include <stdio.h>

const BtU32 StreamingSize = 1024 * 128;

// Class definition
class LBtCompressedFile
{
public:

	// Public functions
	void					Read( const BtChar* pFilename, BtU8* pMemory, BtU32 targetSize );

private:

	// Private functions
	friend class TdTopEntity;
	friend class TdDebug;

	FILE*					m_file;
	z_stream				m_strm;
	BtU8					m_buffer[StreamingSize];
};

#endif // __LBtCompressedFile_H__ 
