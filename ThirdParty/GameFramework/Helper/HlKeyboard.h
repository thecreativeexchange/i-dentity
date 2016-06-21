////////////////////////////////////////////////////////////////////////////////
// HlKeyboard

// Include guard
#pragma once

// Includes
#include "BtTypes.h"
#include "UiKeyboard.h"

const UiKeyCode DebugKey	    = UiKeyCode_F1;
const UiKeyCode SaveCameraKey   = UiKeyCode_F2;
const UiKeyCode ToggleViewKey   = UiKeyCode_F3;
const UiKeyCode CloseGameKey    = UiKeyCode_F4;
const UiKeyCode ScreenShotKey   = UiKeyCode_F11;
const UiKeyCode RecentreViewKey = UiKeyCode_R;

// Class definition
class HlKeyboard
{
public:

	static BtBool IsNumericPressed();
};
