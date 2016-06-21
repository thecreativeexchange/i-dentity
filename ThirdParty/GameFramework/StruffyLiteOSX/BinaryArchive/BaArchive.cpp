////////////////////////////////////////////////////////////////////////////////
// BaArchive

// Includes
#include <new>
#include "BtBase.h"
#include "BtString.h"
#include "BtPrint.h"
#include "BtTypes.h"
#include "BaFileData.h"
#include "BaArchive.h"
#include "BaUserData.h"
#include "BaResource.h"
#include "BtMemory.h"
#include "BtCompressedFile.h"
#include "ApConfig.h"
#include "DyCollisionMesh.h"
#include "DyCollisionAnalytical.h"

// WinGL
#include "SdSoundWinGL.h"
#include "RsMaterialWinGL.h"
#include "RsSpriteWinGL.h"
#include "RsTextureWinGL.h"
#include "RsFontWinGL.h"
#include "RsShaderWinGL.h"
#include "RsSceneWinGL.h"
#include "SgNodeWinGL.h"
#include "SgAnimatorWinGL.h"
#include "RsVertexBufferWinGL.h"
#include "RsIndexBufferWinGL.h"
#include "RsFlashWinGL.h"

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Constructor

BaArchive::BaArchive()
{
	m_pMemoryArchiveDictionary = BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// AddDuplicate

void BaArchive::AddDuplicate( BaResource* pResource )
{
	// Create the duplicate
	BaDuplicate* pDuplicate = new BaDuplicate( pResource );

	// Set the archive
	pResource->m_pArchive = this;

	// Add it to the list
	m_duplicates.AddHead( pDuplicate );
}

////////////////////////////////////////////////////////////////////////////////
// LoadDictionary

void BaArchive::LoadDictionary()
{
	// Load the file
	BtChar filename[MaxFilePath];

	BtStrCopy( filename, MaxFilePath, ApConfig::GetResourcePath() );
	BtStrCat(  filename, MaxFilePath, "Archive" );
	BtStrCat(  filename, MaxFilePath, ApConfig::GetExtension() );

	// Open the archive list
	FILE* f;

	f = fopen( filename, "rb" );

	// Don't load a dictionary if one is not present
	if( f == BtNull )
	{
		BtPrint( "Could not load archive dictionary %s.\r\n", filename );
		return;
	}
	else
	{
		BtPrint( "Loaded archive %s.\r\n", filename );
	}

	// Seek the end of the file
	fseek( f, 0, SEEK_END );

	// Get the position
	BtU32 nSize = ftell( f );

	// Seek the start of the file
	fseek( f, 0, SEEK_SET );

	// Allocate the memory for the dictionary file
	m_pMemoryArchiveDictionary = BtMemory::Allocate( nSize );

	// Read the file into the memory
	fread( m_pMemoryArchiveDictionary, 1, nSize, f );

	// Close the archive list
	fclose( f );

	// Set the number of archives
	m_nArchives = *( (BtU32*) m_pMemoryArchiveDictionary );

	// Set the archive dictionary pointer
	m_pArchiveDictionary = (LBaArchiveEntry*) ( m_pMemoryArchiveDictionary + sizeof(BtU32) );
}

////////////////////////////////////////////////////////////////////////////////
// LoadFile

void BaArchive::LoadFile( const BtChar* archiveName )
{
	BtU32 dataAndInstanceSize = 0;

	for (BtU32 i = 0; i < m_nArchives; i++)
	{
		LBaArchiveEntry* pEntry = &m_pArchiveDictionary[i];

		if (BtStrCompare(pEntry->m_szTitle, archiveName) == BtTrue)
		{
			dataAndInstanceSize = pEntry->m_nArchiveSize + pEntry->m_nInstanceSize;

			// Load the file
			BtChar filename[256];

			BtStrCopy( filename, 256, ApConfig::GetResourcePath() );
			BtStrCat(  filename, 256, archiveName );
			BtStrCat(  filename, 256, ApConfig::GetExtension() );

			// Make the memory
			m_pMemory = (BtU8*)BtMemory::Allocate( dataAndInstanceSize );

			if( 0 )
			{
				// Open a file
				FILE* pFileHandle = fopen( filename, "rb" );

				if( pFileHandle == BtNull )
				{
					BtPrint( "Failed to find file %s\r\n", archiveName );
				}
				else
				{
					BtPrint( "Found file %s\r\n", archiveName );
				}

				// Load in the full archive
				fread( m_pMemory, 1, dataAndInstanceSize, pFileHandle );

				// Close the file
				fclose(pFileHandle);
			}
			else
			{
				BtStrCat( filename, 256, "z" );

				// Load the compressed file
				LBtCompressedFile compressedFile;
				compressedFile.Read( filename, m_pMemory, dataAndInstanceSize );
			}
			return;
		}
	}
	BtPrint( "Could not find archive %s\r\n", archiveName );
}

////////////////////////////////////////////////////////////////////////////////
// CreateResourceInstance

void BaArchive::CreateResourceInstance( BaResourceType resourceType, BtU8 *pMemory )
{
	// Cache the platform
	ApPlatform platform = ApConfig::GetPlatform();

	// Validate the platform
	BtAssert( platform != ApPlatform_Max );

	switch( resourceType )
	{
	case BaRT_CollisionMesh:
		{
			new(pMemory) DyCollisionMesh;
		}
		break;

	case BaRT_CollisionAnalytical:
		{
			new(pMemory) DyCollisionAnalytical;
		}
		break;

	case BaRT_VertexBuffer:
		new (pMemory) RsVertexBufferWinGL;
		break;

	case BaRT_IndexBuffer:
		new (pMemory) RsIndexBufferWinGL;
		break;

	case BaRT_Material:
		new( pMemory ) RsMaterialWinGL;
		break;

	case BaRT_UserData:
		new( pMemory ) BaUserData;
		break;

	case BaRT_Sprite:
		new( pMemory ) RsSpriteWinGL;
		break;

	case BaRT_Font:
		new( pMemory ) RsFontWin32GL;
		break;

	case BaRT_Texture:
		new( pMemory ) RsTextureWinGL;
		break;

	case BaRT_Sound:
		new( pMemory ) SdSoundWinGL;
		break;

	case BaRT_Scene:
		new (pMemory) RsSceneWinGL;
		break;

	case BaRT_SgNode:
		new( pMemory ) SgNodeWinGL;
		break;

	case BaRT_Animator:
		new( pMemory ) SgAnimatorWinGL;
		break;

	case BaRT_Shader:
		new( pMemory ) RsShaderWinGL;
		break;

	case BaRT_Flash:
		new( pMemory ) RsFlashWinGL;
		break;

	default:
		new( pMemory ) BaResource;
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
// CreateResources

void BaArchive::CreateResources()
{
	// Cache the archive header pointer
	LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader* ) m_pMemory;

	// Validate the archive header
	BtAssert( pArchiveHeader != BtNull );

	// Cache each resource header
	LBaResourceHeader* pResource = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Loop through the resources and markup any pointers
	for( BtU32 i=0; i<pArchiveHeader->m_nNumResources; i++ )
	{
		// Calculate the instance pointer
		BtU8* pInstance = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );

		// Validate that the instances start after the archive and it's data
		BtAssert( pResource->m_nInstanceOffset < pArchiveHeader->m_nDataAndInstanceSize );

		// Create the instance
		CreateResourceInstance( pResource->m_type, pInstance );

		// Cache the base type
		BaResource* pResourceInstance = (BaResource*) pInstance;

		// Set the resource title
		pResourceInstance->SetTitle( pResource->m_szTitle );

		// Increment the resource header
		pResource++;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Load

void BaArchive::Load( const BtChar* archiveName )
{
	if( m_pMemoryArchiveDictionary == BtNull )
	{
		LoadDictionary();
	}
	LoadFile( archiveName );
	CreateResources();
	FixPointers();
	ValidateResources();
}

////////////////////////////////////////////////////////////////////////////////
// FixPointers

void BaArchive::FixPointers()
{
	// Cache the archive header pointer
	LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	LBaResourceHeader* pResourceHeader = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Cache each resource header
	pResourceHeader = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );
				
	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{		
		// Cache each resource
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );

		// Locate the file data
		BtU8 *pFileData = ( m_pMemory + pResourceHeader->m_nResourceOffset );

		BaResourceType resourceType = pResource->resourceType();

		// Check the resource type
		if( resourceType != BaRT_Max )
		{			
			BtAssert( pResource->resourceType() == pResourceHeader->m_type );
		
            // Fix pointers
			pResource->FixPointers( pFileData, this );
		
			// Create on the device
			pResource->CreateOnDevice();
		}
		
		// Move to the next resource
		pResourceHeader++;	
	}
	
	BtPrint( "Ended creating resources.\r\n" );	
}


////////////////////////////////////////////////////////////////////////////////
// ValidateResources

void BaArchive::ValidateResources()
{
	RsSprite *pSprite;

	// Cache the archive header pointer
	LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	LBaResourceHeader* pResourceHeader = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Cache each resource header
	pResourceHeader = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{		
		// Cache each resource
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );

		BtChar *pTitle = pResource->GetTitle();
		(void)pTitle;

		BaResourceType resourceType = pResource->resourceType();
		(void)resourceType;

		if( resourceType == BaRT_Sprite )
		{
			pSprite = (RsSprite*)pResource;
			int a=0;
			a++;
		}

		// Move to the next resource
		pResourceHeader++;
	}

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Unload

void BaArchive::Unload()
{
	// Cache the archive header pointer
	LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader* ) m_pMemory;

	// Cache each resource header
	LBaResourceHeader* pResourceHeader = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Loop through the resources and markup any pointers
	for( BtU32 i=0; i<pArchiveHeader->m_nNumResources; i++ )
	{
		// Get the instance address
		BaResource* pResource = (BaResource*)( m_pMemory + pResourceHeader->m_nInstanceOffset );

		if( pResource->resourceType() != BaRT_Max )
		{
			// Remove from the resource list
			pResource->Remove();

			// Remove from the device
			pResource->RemoveFromDevice();

			// Destroy
			pResource->Destroy();	
		}
		
		// Increment the resource header
		pResourceHeader++;		
	}

	BtMemory::Free( m_pMemory );
}


////////////////////////////////////////////////////////////////////////////////
// GetResource

BtU8* BaArchive::GetResource( BaResourceType eType, const BtChar* szTitle ) const
{
    // Cache the archive header pointer
    LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader* ) m_pMemory;
    
    // Validate the archive header
    BtAssert( pArchiveHeader != BtNull );
    
    // Cache each resource header
    LBaResourceHeader* pResource = (LBaResourceHeader*)( m_pMemory + sizeof( LBaArchiveHeader ) );

	// Loop through the resources
	for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
	{
		// Get the instance address
		BtU8* pInstanceAddress = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );

		// Is this the string resource?
		if( pResource->m_type == eType )
		{
			if( BtStrCompareNoCase( pResource->m_szTitle, szTitle ) == BtTrue )
			{
				return pInstanceAddress;
			}
		}

		// Get the next resource header
		pResource++;
	}

	// Return BtNull though we will never reach here
	return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetResource

BtU8* BaArchive::GetResource( BtU32 nResourceID ) const
{
    // Don't search for a null resource
    if( nResourceID == 0  )
    {
        return BtNull;
    }
    
    // Get the archive header
    LBaArchiveHeader* pArchiveHeader = (LBaArchiveHeader*) m_pMemory;
    
    // Get the start of the resource headers
    BtChar *pData = ( (BtChar*) ( m_pMemory ) + sizeof( LBaArchiveHeader ) );
    
    // Get the first resource header
    LBaResourceHeader* pResource = (LBaResourceHeader*) pData;
    
    // Loop through the resources
    for( BtU32 nResourceIndex=0; nResourceIndex<pArchiveHeader->m_nNumResources; nResourceIndex++ )
    {
        // Get the instance address
        BtU8* pInstanceAddress = (BtU8*) ( m_pMemory + pResource->m_nInstanceOffset );
        
        // Is this the string resource?
        if( pResource->m_nResourceID == nResourceID )
        {
            return pInstanceAddress;
        }
        
        // Get the next resource header
        pResource++;
    }
    
    // Return BtNull though we will never reach here
    return BtNull;
}

////////////////////////////////////////////////////////////////////////////////
// GetFont

RsFont* BaArchive::GetFont( const BtChar* name )
{
	RsFont* pFont = (RsFont*)GetResource( BaRT_Font, name );

	if( pFont == BtNull )
	{
		BtPrint( "Could not find Font %s\r\n", name );
	}
	return pFont;
}

////////////////////////////////////////////////////////////////////////////////
// GetNode

SgNode* BaArchive::GetNode( const BtChar *name )
{
	SgNode* pNode = (SgNode*)GetResource( BaRT_SgNode, name );

	if( pNode == BtNull )
	{
		BtPrint( "Could not find node %s\r\n", name );
	}
	return pNode;
}

////////////////////////////////////////////////////////////////////////////////
// GetMaterial

RsMaterial* BaArchive::GetMaterial( const BtChar *name )
{
	RsMaterial* pMaterial = (RsMaterial*)GetResource( BaRT_Material, name );

	if( pMaterial == BtNull )
	{
		BtPrint( "Could not find Material %s\r\n", name );
	}
	return pMaterial;
}

////////////////////////////////////////////////////////////////////////////////
// GetShader

RsShader* BaArchive::GetShader( const BtChar *name )
{
	RsShader* pShader = (RsShader*)GetResource( BaRT_Shader, name );

	if( pShader == BtNull )
	{
		BtPrint( "Could not find shader %s\r\n", name );
	}
	return pShader;
}

////////////////////////////////////////////////////////////////////////////////
// GetAnimator

SgAnimator* BaArchive::GetAnimator( const BtChar *name )
{
	SgAnimator* pAnimator = (SgAnimator*)GetResource( BaRT_Animator, name );

	if( pAnimator == BtNull )
	{
		BtPrint( "Could not find animator %s\r\n", name );
	}
	return pAnimator;
}

////////////////////////////////////////////////////////////////////////////////
// GetTexture

RsTexture* BaArchive::GetTexture( const BtChar* name )
{
	RsTexture* pTexture = (RsTexture*)GetResource( BaRT_Texture, name );

	if( pTexture == BtNull )
	{
		BtPrint( "Could not find texture %s\r\n", name );
	}
	return pTexture;
}

////////////////////////////////////////////////////////////////////////////////
// GetSprite

RsSprite* BaArchive::GetSprite( const BtChar* name )
{
	RsSprite* pSprite = (RsSprite*)GetResource( BaRT_Sprite, name );

	if( pSprite == BtNull )
	{
		BtPrint( "Could not find sprite %s\r\n", name );
	}
	return pSprite;
}

////////////////////////////////////////////////////////////////////////////////
// GetUserData

BaUserData* BaArchive::GetUserData( const BtChar* name )
{
	BaUserData* pUserData = (BaUserData*)GetResource( BaRT_UserData, name );

	if( pUserData == BtNull )
	{
		BtPrint( "Could not find user data %s\r\n", name );
	}
	return pUserData;
}

////////////////////////////////////////////////////////////////////////////////
// GetSound

SdSound* BaArchive::GetSound( const BtChar* name )
{
	SdSound* pSound = (SdSound*)GetResource( BaRT_Sound, name );

	if( pSound == BtNull )
	{
		BtPrint( "Could not find sound %s\r\n", name );
	}
	return pSound;
}

////////////////////////////////////////////////////////////////////////////////
// GetFlash

RsFlash* BaArchive::GetFlash( const BtChar* name )
{
	RsFlash* pFlash = (RsFlash*)GetResource( BaRT_Flash, name );

	if( pFlash == BtNull )
	{
		BtPrint( "Could not find Flash %s\r\n", name );
	}
	return pFlash;
}
