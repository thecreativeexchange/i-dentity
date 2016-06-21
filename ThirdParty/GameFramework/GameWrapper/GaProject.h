////////////////////////////////////////////////////////////////////////////////
// GaProject.h

// Include guard
#pragma once
#include "BtTypes.h"
#include "MtMatrix4.h"

class RsMaterial;

// Class definition
class GaProject
{
public:

	// Public functions
	virtual void					Init() = 0;
	virtual void					Reset() = 0;
	virtual void					Create() = 0;
	virtual void					Update() = 0;
	virtual void					Render() = 0;
	virtual void					Destroy() = 0;

	// Accessors
	virtual void					SetClosing() = 0;
	virtual BtBool					IsClosed() = 0;
	virtual BtBool					IsClosing() = 0;
};
