////////////////////////////////////////////////////////////////////////////////
// HlView.h

// Include guard
#pragma once

// Includes
#include "BaArchive.h"
#include "BtBase.h"
#include "BtArray.h"
#include "BtTypes.h"
#include "MtVector2.h"
#include "ShJoystick.h"
#include <vector>

class RsTexture;
class RsMaterial;

enum HlMenuJoystick
{
	HlMenuJoystick_Yes,
	HlMenuJoystick_No,
	HlMenuJoystick_Any,
	HlMenuJoystick_MAX,
};


const BtU32 HlMenuItemAlignment_HorizontalCentre = 1 << 0;
const BtU32 HlMenuItemAlignment_VerticalCentre = 1 << 1;
const BtU32 HlMenuItemAlignment_Left = 1 << 2;
const BtU32 HlMenuItemAlignment_Right = 1 << 3;
const BtU32 HlMenuItemAlignment_Top = 1 << 4;
const BtU32 HlMenuItemAlignment_Bottom = 1 << 5;

struct HlMenuItems
{
	RsSprite							   *m_pSprite;
	BtU32									m_index;
	MtVector2								m_v2OriginalScreenSize;
	MtVector2								m_v2OriginalPosition;
	MtVector2								m_v2OriginalDimension;
	MtVector2								m_v2Position;
	MtVector2								m_v2Dimension;
	BtChar									m_id[64];
	BtBool									m_isVisible;
	BtFloat									m_fade;
	BtFloat									m_fadeWhenSelected;
	BtBool									m_isPressed;
	BtBool									m_isReleased;
	BtBool									m_isHeld;
	HlMenuJoystick							m_joystick;
	ShJoystickButton						m_joystickButtonID;
	BtBool									m_isSelectable;
	BtU32									m_sortOrder;
	BtU32									m_alignment;
};

// Class definition
class HlView
{
public:
	
	// Public functions		
	void									Setup( const BtChar *archiveName,
												   const BtChar *screenName,
												   BtBool isBackground );
	void									Load();
	void									Update();
	void									Render();
	void									RenderOVR();
	static void								Destroy();

	// Accessors
	void									SetVisible( BtU32 index, BtBool isVisible );
	void									SetVisible( const BtChar *name, BtBool isVisible );
	BtBool									GetVisible( BtU32 index );
	HlMenuItems							   *GetCurrentItemSelected();
	HlMenuItems						  	   *FindItem( const BtChar *name );
	HlMenuItems						  	   *FindItem( BtU32 id );
	BtU32									GetNumItems();
	void									SetBackground( BtBool isBackground );

	static BtBool							IsEqual( const BtChar *string );

protected:

	static HlMenuItems					   *m_pCurrentMenuItem;
	static HlMenuItems					   *m_pCurrentMenuItemSelected;
	BtBool									m_isBackground;

	static BaArchive						m_archive;
	static BtBool							m_isJustLoaded;

	void									UpdateMenu();
	void									RenderMenu();
	void									LoadMenu();
	static void								Unload();

	BtChar									m_archiveName[32];
	BtChar									m_screenName[32];
	
	// Background
	RsMaterial							   *m_pBackgroundMaterial;					// For the background material
	
	// Menu
	static BtArray<HlMenuItems, 64>			m_items;

	// This is so that we can have one archive open for views at any one time
	static BtChar							m_lastArchiveName[32];
    static BtBool                           m_isLoaded;
    static BtU32                            m_alignment;
};

inline HlMenuItems *HlView::GetCurrentItemSelected()
{
	return m_pCurrentMenuItemSelected;
}
