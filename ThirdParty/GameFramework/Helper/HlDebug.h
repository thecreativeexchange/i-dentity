////////////////////////////////////////////////////////////////////////////////
// HlDebug.h

// Include guard
#include "BtArray.h"
#include "BtTypes.h"
#include "RsColour.h"

const BtU32 HlDebugNode	   = 0;
const BtU32 HlDebugInteger = 1;
const BtU32 HlDebugFloat   = 2;

class RsMaterial;
class BaArchive;

struct HlItem
{
	BtU32							m_type;
	BtChar							m_name[32];
	BtFloat						   *m_pFloat;
	BtS32						   *m_pInteger;
	BtU32							m_minInt;
	BtU32							m_maxInt;
	BtFloat							m_minFloat;
	BtFloat							m_maxFloat;
	BtU32							m_diffInt;
};

// Class definition
class HlDebug
{
public:

	// Public functions
	static void						Setup( BaArchive *pArchive );
	static void						Reset();
	static void						AddNode( BtChar *name );
	static void						AddFloat( BtChar *name, BtFloat *value, BtFloat minFloat, BtFloat maxFloat );
	static void						AddInteger( BtChar *name, BtS32 *value, BtS32 minInt, BtS32 maxInt, BtS32 diffInt );
	static void						Update();
	static void						Render( const RsColour &textColour );
	static BtU32					GetIndex();

private:

	static BtArray<HlItem, 1024>	m_items;
	static RsMaterial			   *m_pWhite2;
};
