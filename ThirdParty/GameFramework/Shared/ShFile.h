////////////////////////////////////////////////////////////////////////////////
// ShFile

// Include guard
#pragma once

// Includes
#include "BtBase.h"
#include "BtQueue.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "MtQuaternion.h"
#include "MtMatrix4.h"
#include "MtMatrix3.h"
#include "FsFile.h"
#include "BtString.h"
#include "ApConfig.h"
#include "ErrorLog.h"

// Class definition
class ShFile
{
public:

	// Access
	template <typename T> void		operator >> ( T& );
	template <typename T> void		operator << ( T& );
	// State functions
    
	// Public functions
private:
};


////////////////////////////////////////////////////////////////////////////////
// operator << (write)

template< typename T >  inline void ShFile::operator << ( T& data )
{
    BtChar filename[256];
    BtStrCopy( filename, 256, ApConfig::GetDocuments() );
    BtStrCat(  filename, 256, "/config.txt" );
    
    FsFile file;
    file.Open( filename, eFsMode_Write );
    if( file.IsOpen() )
    {
        file.Write( data );
        file.Close();
    }
}

////////////////////////////////////////////////////////////////////////////////
// operator >> (read)

template< typename T > inline void ShFile::operator >> (T& data)
{
    BtChar filename[256];
    BtStrCopy( filename, 256, ApConfig::GetDocuments() );
    BtStrCat(  filename, 256, "/config.txt" );
    
    FsFile file;
    file.Open( filename, eFsMode_Read );
    if( file.IsOpen() )
    {
        file.Read( (BtU8*) &data, sizeof(data) );
        file.Close();
    }
}