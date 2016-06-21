////////////////////////////////////////////////////////////////////////////////
// HlUserData

// Includes
#include "HlUserData.h"
#include "BtTime.h"
#include "BtString.h"
#include "RsUtil.h"
#include "BaUserData.h"

// statics

////////////////////////////////////////////////////////////////////////////////
// GetSize

BtU32 HlUserData::GetSize( BaArchive *pArchive, const BtChar *name )
{
	// Get the length of data saved by the user
	BtU32 length = pArchive->GetUserData( name )->GetSize();

	// Return this length
	return length;
}

////////////////////////////////////////////////////////////////////////////////
// GetData

BtU8 *HlUserData::GetData( BaArchive *pArchive, const BtChar *name )
{
	// Get the data pointer
	BtU8 *pData = (BtU8*)pArchive->GetUserData( name )->GetData();

	// Return the data
	return pData;
}
