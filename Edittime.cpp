
// ============================================================================
//
// This file contains routines that are handled only during the Edittime,
// under the Frame and Event editors.
//
// Including creating, display, and setting up your object.
// 
// ============================================================================

#include "Common.h"

#if !defined(RUN_ONLY)
BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID);
void WINAPI	DLLExport CreateFromFile(LPMV mV, LPTSTR fileName, LPEDATA edPtr);
Slices oSlices;
bool loaded = false;
// PROPERTIES /////////////////////////////////////////////////////////////////

// Property identifiers
enum {
	PROPID_SETTINGS = PROPID_EXTITEM_CUSTOM_FIRST,

	PROPID_GROUP0,
	PROPID_IMAGES,
	PROPID_GROUP1,
	PROPID_ISINTEGRAL,
	PROPID_GROUP2,
	PROPID_ALLWAYSCENTER,
	PROPID_HOTSPOTX,
	PROPID_HOTSPOTY,
	PROPID_MODE,
};

const char* ModeComboList[] = {
	0,	// reserved
	_T("Tile"),	
	_T("Stretch"),		
	0
};

// Property definitions
//
// Type, ID, Text, Text of Info box [, Options, Init Param]
//
PropData Properties[] = {

	PropData_Group(PROPID_GROUP0, (int)_T("Image bank"),(int)_T("")),
	PropData_ImageList(PROPID_IMAGES, (int)_T("Images"),(int)_T("")),

	PropData_Group(PROPID_GROUP1, (int)_T("Settings"),(int)_T("")),
	PropData_ComboBox(PROPID_MODE,(int)_T("Display mode"),(int)_T(""), ModeComboList),
	PropData_CheckBox(PROPID_ISINTEGRAL, (int)_T("Integral dimensions"), (int)_T("")),

	PropData_Group(PROPID_GROUP2, (int)_T("Hot spot"),(int)_T("")),
	PropData_CheckBox(PROPID_ALLWAYSCENTER, (int)_T("Always center hotspot"), (int)_T("")),
	

	// End of table (required)
	PropData_End()
};

PropData HotSpotCoord[] = {
	PropData_EditNumber(PROPID_HOTSPOTX, (int)_T("X"), (int)_T("")),
	PropData_EditNumber(PROPID_HOTSPOTY, (int)_T("Y"), (int)_T("")),
PropData_End() };


#endif // !defined(RUN_ONLY)


// ============================================================================
//
// ROUTINES USED UNDER FRAME EDITOR
// 
// ============================================================================

// --------------------
// MakeIcon
// --------------------
// Called once object is created or modified, just after setup.
// Also called before showing the "Insert an object" dialog if your object
// has no icon resource

int WINAPI DLLExport MakeIconEx(mv _far* mV, cSurface* pIconSf, LPTSTR lpName, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	if (!loaded) {
		pIconSf->Delete();
		pIconSf->Clone(*SDK->Icon);

		pIconSf->SetTransparentColor(RGB(255, 0, 255));
		loaded = true;
		return 0;
	}
	int error = 1;
	if (edPtr->images[0] && edPtr->width > 0) {
		if (oSlices.generateTileset(edPtr->images, NULL, mV->mvIdAppli))
		{
			cSurface& surf = oSlices.getTileset();
			if (surf.GetWidth() > 0 && surf.GetHeight() > 0 && surf.IsValid()) {
				if (surf.Stretch(*pIconSf, 0, 0, 32, 32, BMODE_TRANSP, BOP_COPY, 0, STRF_RESAMPLE_TRANSP))
					error = 0;
			}
		}
		
	}

	if (error == 1) {
		pIconSf->Delete();
		pIconSf->Clone(*SDK->Icon);

		pIconSf->SetTransparentColor(RGB(255, 0, 255));
	}

#endif // !defined(RUN_ONLY)
	return 0;
}


// --------------------
// CreateObject
// --------------------
// Called when you choose "Create new object". It should display the setup box 
// and initialize everything in the datazone.

int WINAPI DLLExport CreateObject(mv _far* mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	// Check compatibility
	if (IS_COMPATIBLE(mV))
	{
		Edif::Init(mV, edPtr);

		// Set default object settings
		/*edPtr->width = 32;
		edPtr->height = 32;
		edPtr->isIntegral = true;
		edPtr->hotSpotX = 0;
		edPtr->hotSpotY = 0;
		edPtr->ishotSpotAlwaysCenter = false;
		edPtr->xScale = 0.0;
		edPtr->yScale = 0.0;

		for (int i = 0; i < 9; i++)
			edPtr->images[i] = 0;

		oSlices = new Slices();*/
		CreateFromFile(mV, 0, edPtr);
		//oSlices.setIntegral(edPtr->isIntegral);

		return 0;
	}
#endif // !defined(RUN_ONLY)

	// Error
	return -1;
}

// --------------------
// EditObject
// --------------------
// Called when the user selects the Edit command in the object's popup menu
//
BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	if ( IS_COMPATIBLE(mV) )
	{
		return EditProp(mV, edPtr, PROPID_IMAGES);
	}
#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// SetEditSize
// --------------------
// Called when the object has been resized
//
// Note: remove the comments if your object can be resized (and remove the comments in the .def file)

BOOL WINAPI SetEditSize(LPMV mv, LPEDATA edPtr, int cx, int cy)
{
#ifndef RUN_ONLY
	/*if (edPtr->images[0]) {
		oSlices.generateTileset(edPtr->images, WinGetSurface((int)mv->mvIdEditWin), mv->mvIdAppli);
	}
	oSlices.setIntegral(edPtr->isIntegral);
	oSlices.setWidth(cx);
	oSlices.setHeight(cy);
	edPtr->width = oSlices.getWidth();
	edPtr->height = oSlices.getHeight();*/
	edPtr->width = cx;
	edPtr->height = cy;
#endif // !defined(RUN_ONLY)
	return TRUE;	// OK
}


// --------------------
// PutObject
// --------------------
// Called when each individual object is dropped in the frame.
//
void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// RemoveObject
// --------------------
// Called when each individual object is removed from the frame.
//
void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	// Is the last object removed?
    if (0 == cpt)
	{
		//delete(oSlices);
        Edif::Free(edPtr);

		// Do whatever necessary to remove our data
	}
#endif // !defined(RUN_ONLY)
}

// --------------------
// DuplicateObject
// --------------------
// Called when an object is created from another one (note: should be called CloneObject instead...)
//
void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetObjectRect
// --------------------
// Returns the size of the rectangle of the object in the frame editor.
//
void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	oSlices.generateTileset(edPtr->images, NULL, mV->mvIdAppli);

	if (edPtr->ishotSpotAlwaysCenter) {
		edPtr->hotSpotX = edPtr->width / 2;
		edPtr->hotSpotY = edPtr->height / 2;
	}
	oSlices.setIntegral(edPtr->isIntegral);
	oSlices.setMinWidth(32);
	oSlices.setMinHeight(32);
	oSlices.setWidth(edPtr->width);
	oSlices.setHeight(edPtr->height);
	rc->left -= edPtr->hotSpotX;
	rc->top -= edPtr->hotSpotY;
	rc->right = rc->left + oSlices.getWidth();
	rc->bottom = rc->top + oSlices.getHeight();

#endif // !defined(RUN_ONLY)
	return;
}


// --------------------
// EditorDisplay
// --------------------
// Displays the object under the frame editor
//

void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY

	LPSURFACE Surface = WinGetSurface((int) mV->mvIdEditWin);

	if(!Surface) 
        return;


	bool showIcon = true;
	for (int i = 0; i < 9; i++) {
		if (edPtr->images[i] != 0)
			showIcon = false;
	}

	if (showIcon) {
		rc->right = rc->left + 32;
		rc->bottom = rc->top + 32;
		SDK->Icon->Blit(*Surface, rc->left, rc->top, BMODE_TRANSP, BOP_COPY, 0);
		return;
	}

	BlitMode bm = oiPtr->oiHdr.oiInkEffect & EFFECTFLAG_TRANSPARENT ? BMODE_TRANSP : BMODE_OPAQUE;
	BlitOp bo = (BlitOp)(oiPtr->oiHdr.oiInkEffect & EFFECT_MASK);
	LPARAM boParam = oiPtr->oiHdr.oiInkEffectParam;

	oSlices.generateTileset(edPtr->images, Surface, mV->mvIdAppli);
	oSlices.setDisplayMode(edPtr->displayMode);
	oSlices.setWidth(edPtr->width);
	oSlices.setHeight(edPtr->height);
	oSlices.setIntegral(edPtr->isIntegral);
	edPtr->width = oSlices.getWidth();
	edPtr->height = oSlices.getHeight();
	rc->right = rc->left + oSlices.getWidth();
	rc->bottom = rc->top + oSlices.getHeight();
	cSurface slicesSurface;
	oSlices.draw(Surface, slicesSurface);
	slicesSurface.Blit(*Surface, rc->left, rc->top, bm, bo, boParam, 0);

#endif // !defined(RUN_ONLY)
}


// --------------------
// IsTransparent
// --------------------
// This routine tells MMF2 if the mouse pointer is over a transparent zone of the object.
// 

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// PrepareToWriteObject
// --------------------
// Just before writing the datazone when saving the application, MMF2 calls this routine.
// 
void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY

	// Write your code here
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetFilters
// --------------------

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved)
{
#ifndef RUN_ONLY
	// If your extension uses image filters
//	if ( (dwFlags & GETFILTERS_IMAGES) != 0 )
//		return TRUE;

	// If your extension uses sound filters
//	if ( (dwFlags & GETFILTERS_SOUNDS) != 0 )
//		return TRUE;
#endif // RUN_ONLY
	return FALSE;
}

// --------------------
// UsesFile
// --------------------
// Triggers when a file is dropped onto the frame
// Return TRUE if you can create an object from the given file
//
BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPTSTR fileName)
{
	BOOL r = FALSE;
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return r;
}


// --------------------
// CreateFromFile
// --------------------
// Creates a new object from file
//
void WINAPI	DLLExport CreateFromFile (LPMV mV, LPTSTR fileName, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	edPtr->width = 32;
	edPtr->height = 32;
	edPtr->isIntegral = true;
	edPtr->hotSpotX = 0;
	edPtr->hotSpotY = 0;
	edPtr->ishotSpotAlwaysCenter = false;
	edPtr->xScale = 0.0;
	edPtr->yScale = 0.0;
	edPtr->displayMode = 0; // Tile mode

	oSlices.setMinWidth(32);
	oSlices.setMinHeight(32);
	oSlices.setIntegral(true);
	oSlices.generateTileset(edPtr->images, NULL, mV->mvIdAppli);

	for (int i = 0; i < 9; i++)
		edPtr->images[i] = 0;

	//oSlices = new Slices();
#endif // !defined(RUN_ONLY)
}

// ============================================================================
//
// PROPERTIES
// 
// ============================================================================

// --------------------
// GetProperties
// --------------------
// Inserts properties into the properties of the object.
//
BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	mvInsertProps(mV, edPtr, Properties, PROPID_TAB_GENERAL, TRUE);

	if (!edPtr->ishotSpotAlwaysCenter) {
		mvInsertProps(mV, edPtr, HotSpotCoord, PROPID_ALLWAYSCENTER, TRUE);
	}
#endif // !defined(RUN_ONLY)

	// OK
	return TRUE;
}

// --------------------
// ReleaseProperties
// --------------------
// Called when the properties are removed from the property window.
//
void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetPropCreateParam
// --------------------
// Called when a property is initialized and its creation parameter is NULL (in the PropData).
// Allows you, for example, to change the content of a combobox property according to specific settings in the EDITDATA structure.
//
LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return NULL;
}

// ----------------------
// ReleasePropCreateParam
// ----------------------
// Called after a property has been initialized.
// Allows you, for example, to free memory allocated in GetPropCreateParam.
//
void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
}

// --------------------
// GetPropValue
// --------------------
// Returns the value of properties that have a value.
// Note: see GetPropCheck for checkbox properties
//
LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------
	switch (nPropID) {
		case PROPID_IMAGES:
		{
			CPropDataValue* pv = new CPropDataValue(10*sizeof(WORD), NULL);
			if (pv)
			{
				if (pv->m_pData)
				{
					LPWORD pw = (LPWORD)pv->m_pData;
					*pw++ = 9;
					for (WORD w = 0; w < 9; w++)
						*pw++ = edPtr->images[w];
					return pv;
				}
				pv->Delete();
			}
			break;
		}

		case PROPID_HOTSPOTX:
			return new CPropIntValue(edPtr->hotSpotX);

		case PROPID_HOTSPOTY:
			return new CPropIntValue(edPtr->hotSpotY);

		case PROPID_MODE:
			return new CPropDWordValue(edPtr->displayMode);
	}

#endif // !defined(RUN_ONLY)
	return NULL;
}

// --------------------
// GetPropCheck
// --------------------
// Returns the checked state of properties that have a check box.
//
BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	switch (nPropID) {
	case PROPID_ISINTEGRAL:
		return edPtr->isIntegral;
			break;

	case PROPID_ALLWAYSCENTER:
		return edPtr->ishotSpotAlwaysCenter;
		break;
	}
	
#endif // !defined(RUN_ONLY)
	return 0;		// Unchecked
}

// --------------------
// SetPropValue
// --------------------
// This routine is called by MMF after a property has been modified.
//
void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	// Gets the pointer to the CPropValue structure
	CPropValue* pValue = (CPropValue*)lParam;

	// Example
	// -------
	switch (nPropID) {

	case PROPID_IMAGES:
		if (((CPropDataValue*)pValue)->m_pData)
		{
			LPWORD pw = (LPWORD)((CPropDataValue*)pValue)->m_pData;
			*pw++;
			for (WORD w = 0; w < 9; w++)
				edPtr->images[w] = *pw++;

			/*oSlices.setIntegral(edPtr->isIntegral);
			edPtr->width = oSlices.getWidth();
			edPtr->height = oSlices.getHeight();*/
		}
		
		break;

		case PROPID_HOTSPOTX:
			if (!edPtr->ishotSpotAlwaysCenter)
				edPtr->hotSpotX = ((CPropIntValue*)pValue)->m_nValue;
			break;

		case PROPID_HOTSPOTY:
			if (!edPtr->ishotSpotAlwaysCenter)
				edPtr->hotSpotY = ((CPropIntValue*)pValue)->m_nValue;
			break;

		case PROPID_MODE:
			edPtr->displayMode = ((CPropDWordValue*)pValue)->m_dwValue;
			break;
	}
	mvInvalidateObject(mV, edPtr);

	
#endif // !defined(RUN_ONLY)
}

// --------------------
// SetPropCheck
// --------------------
// This routine is called by MMF when the user modifies a checkbox in the properties.
//
void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY
	switch (nPropID)
	{
		case PROPID_ISINTEGRAL:
			edPtr->isIntegral = nCheck;
			
			mvInvalidateObject(mV, edPtr);
			mvRefreshProp(mV, edPtr, PROPID_ISINTEGRAL, TRUE);
		break;

		case PROPID_ALLWAYSCENTER:
			edPtr->ishotSpotAlwaysCenter = nCheck;

			if (nCheck) {
				mvRemoveProps(mV, edPtr, HotSpotCoord);
			}
			else {
				mvInsertProps(mV, edPtr, HotSpotCoord, PROPID_ALLWAYSCENTER, TRUE);
			}

			mvInvalidateObject(mV, edPtr);
			mvRefreshProp(mV, edPtr, PROPID_ALLWAYSCENTER, TRUE);
		break;
	}
#endif // !defined(RUN_ONLY)
}

// --------------------
// EditProp
// --------------------
// This routine is called when the user clicks the button of a Button or EditButton property.
//
BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY

	if (nPropID == PROPID_IMAGES)
	{
		EditAnimationParams eap;
		eap.m_dwSize = sizeof(EditAnimationParams);
		eap.m_pWindowTitle = 0;
		eap.m_dwOptions = PICTEDOPT_CANBETRANSPARENT|PICTEDOPT_FIXEDNFRAMES;
		eap.m_dwFixedHeight = 48;
		eap.m_dwFixedWidth = 48;
		eap.m_nMaxImages = 9;
		eap.m_nImages = 9;
		eap.m_nStartIndex = 0;
		eap.m_pImageTitles = 0;
		eap.m_pImages = (LPWORD)&edPtr->images[0];
		BOOL output = mV->mvEditAnimation(edPtr, &eap, mV->mvHEditWin);
		if (output) {
			mvRefreshProp(mV, edPtr, PROPID_IMAGES, true);
			mvInvalidateObject(mV, edPtr);
		}

		return output;
	}


#endif // !defined(RUN_ONLY)
	return FALSE;
}

// --------------------
// IsPropEnabled
// --------------------
// This routine returns the enabled state of a property.
//
BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
#endif // !defined(RUN_ONLY)
	return TRUE;
}

// ----------------------------------------------------------
// Custom Parameters
// ----------------------------------------------------------

// --------------------
// InitParameter
// --------------------
// Initialize the parameter.
//
void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !defined(RUN_ONLY)
#endif // !defined(RUN_ONLY)
}

// --------------------
// EditParameter
// --------------------
// Edit the parameter.
//
void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
	//DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_SETUP), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)pExt);

#endif // !defined(RUN_ONLY)
}

// --------------------
// GetParameterString
// --------------------
// Initialize the parameter.
//
void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
	// wsprintf(pDest, "Super parameter %s", pExt->pextData);

#endif // !defined(RUN_ONLY)
}
