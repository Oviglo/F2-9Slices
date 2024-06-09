#pragma once

// #define TGFEXT       // TGF2, MMF2, MMF2 Dev
   #define MMFEXT       // MMF2, MMF2 Dev
// #define PROEXT       // MMF2 Dev only

#include    "Edif.h"
#include	"Resource.h"
#include	"Slices.h"

// edPtr : Used at edittime and saved in the MFA/CCN/EXE files

struct EDITDATA
{
	// Header - required
	extHeader		eHeader;

	// Object's data
	short			width;
	short			height;
	WORD			images[9];
	bool			isIntegral;
	int				hotSpotX;
	int				hotSpotY;
	bool			ishotSpotAlwaysCenter;
	float			xScale;
	float			yScale;
	unsigned char 	displayMode;
};

#include "Extension.h"
