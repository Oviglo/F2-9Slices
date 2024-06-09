/* Copyright (c) 2020 Oviglo
 */
package Extensions;

import Actions.CActExtension;
import Conditions.CCndExtension;
import Expressions.CValue;
import Objects.CExtension;
import Objects.CObject;
import OpenGL.GLRenderer;
import OpenGL.ITexture;
import Banks.CImage;
import Banks.CImageBank;
import Services.CBinaryFile;
import RunLoop.CCreateObjectInfo;
import RunLoop.CObjInfo;
import Runtime.MMFRuntime;
import android.graphics.Bitmap;
import Extensions.OvigloSlices;
import android.util.Log;
import Runtime.SurfaceView;

public class CRun9SlicesObject extends CRunExtension
{
    private OvigloSlices oSlices;
    private int xHotSpot;
    private int yHotSpot;
    private boolean centerHotSpot;
    private int oldXHotSpot;
    private int oldYHotSpot;
    private short [] imageHandles;
    private boolean tilesetGenerated = false;

    private CValue expRet;

    public class CSurface extends CImage
    {
        public CSurface(Bitmap img) {
            synchronized(img)
            {
                allocNative2 (false,
                        (short) -1, getBitmapPixels (img), 0, 0, 0,
                        0, img.getWidth (), img.getHeight (), SurfaceView.ES);

                images.add (this);
            }
            img.recycle();
        }

        @Override
        public void onDestroy ()
        {

        }
    }

    CSurface finalSurf;

    public CRun9SlicesObject() {
        this.oSlices = new OvigloSlices();
        this.imageHandles = new short[9];
    }

    @Override
    public int getNumberOfConditions()
    {
        return 2;
    }

    @Override
    public boolean createRunObject(CBinaryFile file, CCreateObjectInfo cob, int version)
    {
        this.oSlices.setWidth(file.readShort());
        this.oSlices.setHeight(file.readShort());
        for (int i = 0; i < 9; i++) {
            this.imageHandles[i] = file.readShort();
        }
        this.ho.loadImageList(this.imageHandles);
        this.oSlices.setIntegral(file.readShort() == 1);

        this.xHotSpot = file.readInt();
        this.yHotSpot = file.readInt();

        this.centerHotSpot = file.readByte() == 1;
        file.skipBytes(11);
        this.oSlices.setDisplayMode(file.readByte());

        this.ho.hoImgWidth = this.oSlices.getWidth();
        this.ho.hoImgHeight = this.oSlices.getHeight();

        this.oldXHotSpot = 0;
        this.oldYHotSpot = 0;

        return false;
    }

    @Override
    public int handleRunObject()
    {
        Bitmap [] images = new Bitmap[9];
        for (int i = 0; i < 9; i++) {
            CImage cImage =  ho.getImageBank().getImageFromHandle(this.imageHandles[i]);
            int[] mImage = cImage.getRawPixels();
            if(mImage != null) {
                images[i] = Bitmap.createBitmap(cImage.getWidth(), cImage.getHeight(), cImage.getFormat());
                images[i].setPixels(mImage, 0, cImage.getWidth(), 0, 0, cImage.getWidth(), cImage.getHeight());
            }
        }

        this.oSlices.generateTileset(images);
        this.tilesetGenerated = true;
        if (this.centerHotSpot) {
            this.xHotSpot = (int) Math.abs(this.oSlices.getWidth() * this.oSlices.getXScale()) / 2;
            this.yHotSpot = (int) Math.abs(this.oSlices.getHeight() * this.oSlices.getYScale()) / 2;
        }

        this.ho.hoImgWidth = (int) Math.abs(this.oSlices.getWidth() * this.oSlices.getXScale());
        this.ho.hoImgHeight = (int) Math.abs(this.oSlices.getHeight() * this.oSlices.getYScale());
        this.ho.hoX -= (this.xHotSpot - this.oldXHotSpot);
        this.ho.hoY -= (this.yHotSpot - this.oldYHotSpot);

        this.oldXHotSpot = this.xHotSpot;
        this.oldYHotSpot = this.yHotSpot;


        Bitmap finalRender = this.oSlices.draw();
        this.finalSurf = new CSurface(finalRender);

        return 2;
    }

    @Override
    public void displayRunObject()
    {
        if (this.oSlices.getXScale() == 0 || this.oSlices.getYScale() == 0) {
            return;
        }

        if (!this.tilesetGenerated) {
            this.ho.reHandle();
            return;
        }

        int x = this.ho.hoX - this.rh.rhWindowX;
        int y = this.ho.hoY - this.rh.rhWindowY;
        GLRenderer.inst.renderImage(this.finalSurf, x, y, this.oSlices.getWidth(), this.oSlices.getHeight(), 0, 0);
    }

    public void continueRunObject() {
        this.ho.reHandle();
    }

    @Override
    public boolean condition (int num, CCndExtension cnd)
    {
        switch (num)
        {
            case 0:
                return this.oSlices.getIntegral();

            case 1:
                return this.centerHotSpot;
        };

        return false;
    }

    public void action (int num, CActExtension act)
    {
        switch (num) {
            case 0:
                this.oSlices.setWidth(act.getParamExpression(this.rh, 0));
                ho.reHandle();
                break;
            case 1:
                this.oSlices.setHeight(act.getParamExpression(this.rh, 0));
                ho.reHandle();
                break;
            case 2:
                this.oSlices.setXScale(act.getParamExpression(this.rh, 0));
                ho.reHandle();
                break;
            case 3:
                this.oSlices.setYScale(act.getParamExpression(this.rh, 0));
                ho.reHandle();
                break;
            case 4:
                this.xHotSpot = act.getParamExpression(this.rh, 0);
                ho.reHandle();
                break;
            case 5:
                this.yHotSpot = act.getParamExpression(this.rh, 0);
                ho.reHandle();
                break;
            case 6:
                this.centerHotSpot = true;
                ho.reHandle();
                break;
            case 7:
                this.centerHotSpot = false;
                ho.reHandle();
                break;
            case 8:
                this.oSlices.setIntegral(true);
                ho.reHandle();
                break;
            case 9:
                this.oSlices.setIntegral(false);
                break;
            case 10:
                this.oSlices.setDisplayMode(act.getParamExpression(this.rh, 0));
                ho.reHandle();
                break;
        };
    }

    @Override
    public CValue expression (int num)
    {
        switch (num)
        {
            case 0:
                expRet.forceDouble(this.oSlices.getXScale());
                return expRet;
            case 1:
                expRet.forceDouble(this.oSlices.getYScale());
                return expRet;
            case 2:
                expRet.forceInt(this.xHotSpot);
                return expRet;
            case 3:
                expRet.forceInt(this.yHotSpot);
                return expRet;
            case 4:
                expRet.forceInt(this.oSlices.getDisplayMode());
                return expRet;
        };
        return expRet;
    }
}