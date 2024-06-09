
#include "Common.h"


///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr)
    : rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr)
{
    /*
        Link all your action/condition/expression functions to their IDs to match the
        IDs in the JSON here
    */

    LinkAction(0, setWidth);
    LinkAction(1, setHeight);
    LinkAction(2, setXScale);
    LinkAction(3, setYScale);
    LinkAction(4, setXHotSpot);
    LinkAction(5, setYHotSpot);
    LinkAction(6, enableCenterHotSpot);
    LinkAction(7, disableCenterHotSpot);
    LinkAction(8, enableIntegralDimensions);
    LinkAction(9, disableIntegralDimensions);
    LinkAction(10, setDisplayMode);

    LinkCondition(0, isCenterHotSpot);
    LinkCondition(1, isIntegralDimensions);

    LinkExpression(0, getXScale);
    LinkExpression(1, getYScale);
    LinkExpression(2, getXHotSpot);
    LinkExpression(3, getYHotSpot);
    LinkExpression(4, getDisplayMode);

    /*
        This is where you'd do anything you'd do in CreateRunObject in the original SDK

        It's the only place you'll get access to edPtr at runtime, so you should transfer
        anything from edPtr to the extension class here.
    
    */
    oSlices = new Slices();

    LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
    isHwa = ps->GetDriver() >= SD_3DFX;
    oSlices->setIntegral(edPtr->isIntegral);
    oSlices->generateTileset(edPtr->images, ps, rhPtr->rhIdAppli);
    oSlices->setDisplayMode(edPtr->displayMode);

    oSlices->setWidth(edPtr->width);
    oSlices->setHeight(edPtr->height);
    rdPtr->rHo.hoImgWidth = oSlices->getWidth();
    rdPtr->rHo.hoImgHeight = oSlices->getHeight();
    centerHotSpot = edPtr->ishotSpotAlwaysCenter;
    
    if (centerHotSpot) {
        xHotSpot = oSlices->getWidth() /2;
        yHotSpot = oSlices->getHeight() / 2;
    }
    else {
        xHotSpot = edPtr->hotSpotX;
        yHotSpot = edPtr->hotSpotY;
    }

    oldXHotSpot = 0;
    oldYHotSpot = 0;
    //rdPtr->rHo.hoX -= xHotSpot;
    //rdPtr->rHo.hoY -= yHotSpot;
}

Extension::~Extension()
{
    /*
        This is where you'd do anything you'd do in DestroyRunObject in the original SDK.
        (except calling destructors and other such atrocities, because that's automatic in Edif)
    */
    delete(oSlices);

}


short Extension::Handle()
{
    if (this->centerHotSpot) {
        this->xHotSpot = abs(this->oSlices->getWidth() * this->oSlices->getXScale()) / 2;
        this->yHotSpot = abs(this->oSlices->getHeight() * this->oSlices->getYScale()) / 2;
    }

    rdPtr->rHo.hoImgWidth = this->oSlices->getWidth() * abs(this->oSlices->getXScale());
    rdPtr->rHo.hoImgHeight = this->oSlices->getHeight() * abs(this->oSlices->getYScale());

    rdPtr->rHo.hoX -= (this->xHotSpot - this->oldXHotSpot);
    rdPtr->rHo.hoY -= (this->yHotSpot - this->oldYHotSpot);

    this->oldXHotSpot = this->xHotSpot;
    this->oldYHotSpot = this->yHotSpot;

    rdPtr->rHo.hoRect.left = rdPtr->rHo.hoX;
    rdPtr->rHo.hoRect.top = rdPtr->rHo.hoY;
    rdPtr->rHo.hoRect.right = rdPtr->rHo.hoX + rdPtr->rHo.hoImgWidth;
    rdPtr->rHo.hoRect.bottom = rdPtr->rHo.hoY + rdPtr->rHo.hoImgHeight;

    this->isTilesetGenerated = true;

    return REFLAG_DISPLAY;
}


short Extension::Display()
{
    /*
       If you return REFLAG_DISPLAY in Handle() this routine will run.
    */
    LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
    LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
    
    if (ps != NULL) {
        if (oSlices->getXScale() == 0 || oSlices->getYScale() == 0) {
            return 0;
        }

        if (!this->isTilesetGenerated) {
            Runtime.Rehandle();
            return 0;
        }
        RECT rc = rdPtr->rHo.hoRect;
        BlitMode bm = (rdPtr->rs->rsEffect & EFFECTFLAG_TRANSPARENT) ? BMODE_TRANSP : BMODE_OPAQUE;
        BlitOp bo = (BlitOp)(rdPtr->rs->rsEffect & EFFECT_MASK);
        int effectParam = rdPtr->rs->rsEffectParam;
        int flags = (rdPtr->rs->rsEffect & EFFECTFLAG_TRANSPARENT) ? 0 : BLTF_COPYALPHA;
       
        cSurface slicesSurface;
        oSlices->draw(ps, slicesSurface);
        slicesSurface.Blit(*ps, rc.left, rc.top, bm, bo, effectParam, flags);
        
        WinAddZone(rhPtr->rhIdEditWin, &rc);
    }

    // Ok
    return 0;
}

short Extension::Pause()
{

    // Ok
    return 0;
}

short Extension::Continue()
{

    // Ok
    return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Save the object's data here

	    OK = true;

    #endif

	return OK;
}

bool Extension::Load(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Load the object's data here

	    OK = true;

    #endif

	return OK;
}


// These are called if there's no function linked to an ID

void Extension::setWidth(int value)
{
    oSlices->setWidth(value);
    Runtime.Rehandle();
}

void Extension::setHeight(int value)
{
    oSlices->setHeight(value);
    Runtime.Rehandle();
}

void Extension::setXScale(float value)
{
    oSlices->setXScale(value);
    Runtime.Rehandle();
}

void Extension::setYScale(float value)
{
    oSlices->setYScale(value);
    Runtime.Rehandle();
}

void Extension::setXHotSpot(int value)
{
    xHotSpot = value;
}

void Extension::setYHotSpot(int value)
{
    yHotSpot = value;
}

void Extension::enableCenterHotSpot()
{
    centerHotSpot = true;
}

void Extension::disableCenterHotSpot()
{
    centerHotSpot = false;
}

void Extension::enableIntegralDimensions()
{
    oSlices->setIntegral(true);
    Runtime.Rehandle();
}

void Extension::disableIntegralDimensions()
{
    oSlices->setIntegral(false);
    Runtime.Rehandle();
}

void Extension::setDisplayMode(int mode)
{
    this->oSlices->setDisplayMode(mode);
}

bool Extension::isIntegralDimensions()
{
    return oSlices->getIntegral();
}

bool Extension::isCenterHotSpot()
{
    return centerHotSpot;
}

float Extension::getXScale()
{
    return oSlices->getXScale();
}

float Extension::getYScale()
{
    return oSlices->getYScale();
}

int Extension::getXHotSpot()
{
    return xHotSpot;
}

int Extension::getYHotSpot()
{
    return yHotSpot;
}

int Extension::getDisplayMode()
{
    return this->oSlices->getDisplayMode();
}

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{

}

long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{

    return false;
}

long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{

    return 0;
}

