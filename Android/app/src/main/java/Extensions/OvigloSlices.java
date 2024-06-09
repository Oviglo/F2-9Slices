/* Copyright (c) 2020 Oviglo
 */
package Extensions;

import java.lang.Math;
import android.graphics.Canvas;
import Banks.CImage;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Rect;

public class OvigloSlices
{
    private int width;
    private int height;
    private int minWidth;
    private int minHeight;
    private boolean isIntegral;
    private float xScale = (float)1.0;
    private float yScale = (float)1.0;
    private int displayMode;
    private Bitmap tileset;

    class OvigloSliceTilesetSize { public int x, y, w, h; };

    OvigloSliceTilesetSize [] tilesetSizes;

    public OvigloSlices() {
        this.tilesetSizes = new OvigloSliceTilesetSize[9];
    }

    public int getWidth() { return this.width; }
    public int getHeight() { return this.height; }
    public void setWidth(int value) {
        this.width = Math.max(this.minWidth, value);
        this.updateSize();
    }
    public void setHeight(int value) {
        this.height = Math.max(this.minHeight, value);
        this.updateSize();
    }
    public void setIntegral(boolean val)
    {
        this.isIntegral = val;
    }
    public float getXScale() {
        return this.xScale;
    }
    public float getYScale() {
        return this.yScale;
    }
    public void setXScale(float val) {
        this.xScale = val;
    }
    public void setYScale(float val) {
        this.yScale = val;
    }
    public boolean getIntegral() { return this.isIntegral; }
    public void setDisplayMode(int val) { this.displayMode = val; }
    public int getDisplayMode() { return this.displayMode; }

    public void generateTileset(Bitmap [] images) {
        int n = 0;
        for ( Bitmap image : images ) {
            this.tilesetSizes[n] = new OvigloSliceTilesetSize();
            this.tilesetSizes[n].w = image.getWidth();
            this.tilesetSizes[n].h = image.getHeight();
            n++;
        };

        int w = this.tilesetSizes[0].w + this.tilesetSizes[1].w + this.tilesetSizes[2].w;
        int h = this.tilesetSizes[0].h + this.tilesetSizes[3].h + this.tilesetSizes[6].h;

        this.tileset = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        Canvas tilesetCanvas = new Canvas(this.tileset);

        this.minWidth = this.tilesetSizes[0].w + this.tilesetSizes[2].w;
        this.minHeight = this.tilesetSizes[0].h + this.tilesetSizes[6].h;

        int xOffset;
        int yOffset = 0;

        for (int i = 0; i < 3; i++) {
            xOffset = 0;
            for (int j = 0; j < 3; j++) {
                tilesetCanvas.drawBitmap(images[(i*3)+j], (float)xOffset, (float)yOffset, null);

                this.tilesetSizes[(i*3)+j].x = xOffset;
                this.tilesetSizes[(i*3)+j].y = yOffset;

                xOffset += this.tilesetSizes[(i*3)+j].w;
            }

            yOffset += this.tilesetSizes[i*3].h;
        }
    }

    public void updateSize() {
        if (this.isIntegral) {
            if (this.tilesetSizes[4].w > 0 && this.tilesetSizes[4].h > 0) {
                int middleXSize = (int) Math.ceil((this.width - (this.tilesetSizes[0].w + this.tilesetSizes[2].w)) / this.tilesetSizes[4].w);
                int middleYSize = (int) Math.ceil((this.height - (this.tilesetSizes[0].h + this.tilesetSizes[6].h)) / this.tilesetSizes[4].h);

                this.width = (this.tilesetSizes[0].w + this.tilesetSizes[2].w) + (middleXSize * this.tilesetSizes[4].w);
                this.height = (this.tilesetSizes[0].h + this.tilesetSizes[6].h) + (middleYSize * this.tilesetSizes[4].h);
            } else {
                this.width = this.tilesetSizes[0].w + this.tilesetSizes[2].w;
                this.height = this.tilesetSizes[0].h + this.tilesetSizes[6].h;
            }
        }
    }

    public void drawPart(Canvas dest, int imageId, int x, int y, int w, int h)
    {
        Rect rcDest = new Rect();
        Rect rcSrc = new Rect(
                this.tilesetSizes[imageId].x,
                this.tilesetSizes[imageId].y,
                this.tilesetSizes[imageId].x + this.tilesetSizes[imageId].w,
                this.tilesetSizes[imageId].y + this.tilesetSizes[imageId].h
        );

        if (this.displayMode == 0) {
            int i = 0;
            int j = 0;
            int initW = w;
            int initH = h;
            int repeatX = (int) Math.floor(w / this.tilesetSizes[imageId].w);
            int repeatY = (int) Math.floor(h / this.tilesetSizes[imageId].h);

            for (i = 0; i < repeatX; i++) {
                initH = h;
                for (j = 0; j < repeatY; j++) {
                    // DRAW IMAGE
                    rcDest.left = x + (i*this.tilesetSizes[imageId].w);
                    rcDest.top = y + (j*this.tilesetSizes[imageId].h);
                    rcDest.right = rcDest.left + this.tilesetSizes[imageId].w;
                    rcDest.bottom = rcDest.top + this.tilesetSizes[imageId].h;

                    dest.drawBitmap(this.tileset, rcSrc, rcDest, null);

                    initH -= this.tilesetSizes[imageId].h;
                }

                // Draw verticaly complete part
                if (initH > 0) {
                    rcSrc.right = rcSrc.left + this.tilesetSizes[imageId].w;
                    rcSrc.bottom = rcSrc.top + initH;

                    rcDest.left = x + (i*this.tilesetSizes[imageId].w);
                    rcDest.top = y + (j*this.tilesetSizes[imageId].h);
                    rcDest.right = rcDest.left + this.tilesetSizes[imageId].w;
                    rcDest.bottom = rcDest.top + initH;
                    dest.drawBitmap(this.tileset, rcSrc, rcDest, null);
                }

                initW -= this.tilesetSizes[imageId].w;
            }

            // Draw horizontaly complete part
            if (initW > 0) {
                for (j = 0; j < repeatY; j++) {
                    rcSrc.right = rcSrc.left + initW;
                    rcSrc.bottom = rcSrc.top + this.tilesetSizes[imageId].h;

                    rcDest.left = x + (i*this.tilesetSizes[imageId].w);
                    rcDest.top = y + (j*this.tilesetSizes[imageId].h);
                    rcDest.right = rcDest.left + initW;
                    rcDest.bottom = rcDest.top + this.tilesetSizes[imageId].h;
                    dest.drawBitmap(this.tileset, rcSrc, rcDest, null);
                }
            }

            if (initH > 0 && initW > 0) {
                rcSrc.right = rcSrc.left + initW;
                rcSrc.bottom = rcSrc.top + initH;

                rcDest.left = x + (i*this.tilesetSizes[imageId].w);
                rcDest.top = y + (j*this.tilesetSizes[imageId].h);
                rcDest.right = rcDest.left + initW;
                rcDest.bottom = rcDest.top + initH;
                dest.drawBitmap(this.tileset, rcSrc, rcDest, null);
            }

        } else {
            rcSrc.right = rcSrc.left + tilesetSizes[imageId].w;
            rcSrc.bottom = rcSrc.top + tilesetSizes[imageId].h;
            rcDest.left = x;
            rcDest.top = y;
            rcDest.right = rcDest.left + w;
            rcDest.bottom = rcDest.top + h;
            dest.drawBitmap(this.tileset, rcSrc, rcDest, null);
        }
    }

    public Bitmap draw()
    {
        int w =(int) Math.round(Math.abs(this.width * this.xScale));
        int h = (int) Math.round(Math.abs(this.height * this.yScale));
        Bitmap finalRender = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888 );
        Canvas finalCanvas = new Canvas(finalRender);

        // Image 4 center
        if (this.tilesetSizes[4].w > 0 && this.tilesetSizes[4].h > 0) {
            this.drawPart(finalCanvas,
                4,
                this.tilesetSizes[3].w,
                this.tilesetSizes[1].h,
                (w - this.tilesetSizes[3].w - this.tilesetSizes[5].w),
                (h - this.tilesetSizes[1].h - this.tilesetSizes[7].h)
            );
        }

        // Image 1, 7: top bottom border
        if (this.tilesetSizes[1].w > 0 && this.tilesetSizes[1].h > 0) {
            this.drawPart(
                    finalCanvas,
                1,
                this.tilesetSizes[0].w,
                0,
                (w - this.tilesetSizes[0].w - this.tilesetSizes[2].w),
                this.tilesetSizes[1].h
            );
        }

        if (this.tilesetSizes[7].w > 0 && this.tilesetSizes[7].h > 0) {
            this.drawPart(
                    finalCanvas,
                7,
                this.tilesetSizes[6].w,
                (h - this.tilesetSizes[7].h),
                (w - this.tilesetSizes[6].w - this.tilesetSizes[8].w),
                this.tilesetSizes[7].h
            );
        }

        // Images 3, 5: left and right border
        if (this.tilesetSizes[3].w > 0 && this.tilesetSizes[3].h > 0) {
            this.drawPart(
                    finalCanvas,
                3,
                0,
                this.tilesetSizes[0].h,
                this.tilesetSizes[3].w,
                (h - this.tilesetSizes[0].h - this.tilesetSizes[6].h)
            );
        }

        if (this.tilesetSizes[5].w > 0 && this.tilesetSizes[5].h > 0) {
            this.drawPart(
                    finalCanvas,
                5,
                (w - this.tilesetSizes[5].w),
                this.tilesetSizes[0].h,
                this.tilesetSizes[5].w,
                (h - this.tilesetSizes[0].h - this.tilesetSizes[6].h)
            );
        }

        // Images 0, 2, 6, 8: corners
        if (this.tilesetSizes[0].w > 0 && this.tilesetSizes[0].h > 0) {
            this.drawPart(
                    finalCanvas,
                0,
                0,
                0,
                this.tilesetSizes[0].w,
                this.tilesetSizes[0].h
            );
        }

        if (this.tilesetSizes[2].w > 0 && this.tilesetSizes[2].h > 0) {
            this.drawPart(
                    finalCanvas,
                2,
                (w -this.tilesetSizes[2].w),
                0,
                this.tilesetSizes[2].w,
                this.tilesetSizes[2].h
            );
        }

        if (this.tilesetSizes[6].w > 0 && this.tilesetSizes[6].h > 0) {
            this.drawPart(
                    finalCanvas,
                6,
                0,
                (h - this.tilesetSizes[6].h),
                this.tilesetSizes[6].w,
                this.tilesetSizes[6].h
            );
        }

        if (this.tilesetSizes[8].w > 0 && this.tilesetSizes[8].h > 0) {
            this.drawPart(
                    finalCanvas,
                8,
                (w - this.tilesetSizes[8].w),
                (h - this.tilesetSizes[8].h),
                this.tilesetSizes[8].w,
                this.tilesetSizes[8].h
            );
        }

        if (this.xScale < 0.0) {
            finalCanvas.translate(this.width, 0);
            finalCanvas.scale(-1, 1);
        }

        if (this.yScale < 0.0) {
            finalCanvas.translate(0, this.height);
            finalCanvas.scale(1, -1);
        }

        return finalRender;
    }

}