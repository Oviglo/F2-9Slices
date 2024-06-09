//----------------------------------------------------------------------------------
// 9SlicesObject.js
// Clickteam Fusion 2.5 HTML5 9 Slices Object
// By Loic OVIGNE
//----------------------------------------------------------------------------------
/* Copyright (c) 2020 - Oviglo
*/

// Slices helping object
function OvigloSlices() {
    this.width = 32;
    this.height = 32;
    this.minWidth = 32;
    this.minHeight = 32;
    this.isIntegral = true;
    this.xScale = 1.0;
    this.yScale = 1.0;
    this.tileset = null;
    this.tilesetSizes = [];
    this.displayMode = 0;
};

function OvigloSliceTilesetSize() {
    this.w = 0;
    this.h = 0;
    this.x = 0;
    this.y = 0;
};

OvigloSlices.prototype = {
    drawPart(dest, imageId, x, y, w, h) {
        if (this.displayMode == 0) {
            var i = 0;
            var j = 0;
            var initW = w;
            var initH = h;
            var repeatX = Math.floor(w / this.tilesetSizes[imageId].w);
            var repeatY = Math.floor(h / this.tilesetSizes[imageId].h);

            for (i =0; i < repeatX; i++) {
                initH = h;
                for (j = 0; j < repeatY; j++) {
                    dest["drawImage"](
                        this.tileset.canvas,
                        this.tilesetSizes[imageId].x,
                        this.tilesetSizes[imageId].y,
                        this.tilesetSizes[imageId].w,
                        this.tilesetSizes[imageId].h,
                        x + (i * this.tilesetSizes[imageId].w),
                        y + (j * this.tilesetSizes[imageId].h),
                        this.tilesetSizes[imageId].w,
                        this.tilesetSizes[imageId].h

                    );

                    initH -= this.tilesetSizes[imageId].h;
                }

                // Draw verticaly complete part
                if (initH > 0) {
                    
                    dest["drawImage"](
                        this.tileset.canvas,
                        this.tilesetSizes[imageId].x,
                        this.tilesetSizes[imageId].y,
                        this.tilesetSizes[imageId].w,
                        initH,
                        x + (i * this.tilesetSizes[imageId].w),
                        y + (j * this.tilesetSizes[imageId].h),
                        this.tilesetSizes[imageId].w,
                        initH
                    );
                }

                initW -= this.tilesetSizes[imageId].w;
            }

            // Draw horizontaly complete part
            if (initW > 0) {
                for (j = 0; j < repeatY; j++) {
                    dest["drawImage"](
                        this.tileset.canvas,
                        this.tilesetSizes[imageId].x,
                        this.tilesetSizes[imageId].y,
                        initW,
                        this.tilesetSizes[imageId].h,
                        x + (i * this.tilesetSizes[imageId].w),
                        y + (j * this.tilesetSizes[imageId].h),
                        initW,
                        this.tilesetSizes[imageId].h
                    );
                }
            }

            // Draw complete center part (bottom left)
            if (initH > 0 && initW > 0) {
                dest["drawImage"](
                    this.tileset.canvas,
                    this.tilesetSizes[imageId].x,
                    this.tilesetSizes[imageId].y,
                    initW,
                    initH,
                    x + (i * this.tilesetSizes[imageId].w),
                    y + (j * this.tilesetSizes[imageId].h),
                    initW,
                    initH
                );
            }
        } else {
            dest["drawImage"](
                this.tileset.canvas,
                this.tilesetSizes[imageId].x,
                this.tilesetSizes[imageId].y,
                this.tilesetSizes[imageId].w,
                this.tilesetSizes[imageId].h,
                x,
                y,
                w,
                h

            );
        }
    },

    draw() {
        var w = Math.round(Math.abs(this.width * this.xScale));
        var h = Math.round(Math.abs(this.height * this.yScale));
        var finalCtx = document.createElement("canvas").getContext("2d");
        finalCtx.canvas.width = w;
        finalCtx.canvas.height = h;

        // Image 4 center
        if (this.tilesetSizes[4].w > 0 && this.tilesetSizes[4].h > 0) {
            this.drawPart(
                finalCtx, 
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
                finalCtx, 
                1,
                this.tilesetSizes[0].w,
                0,
                (w - this.tilesetSizes[0].w - this.tilesetSizes[2].w),
                this.tilesetSizes[1].h
            );
        }

        if (this.tilesetSizes[7].w > 0 && this.tilesetSizes[7].h > 0) {
            this.drawPart(
                finalCtx, 
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
                finalCtx, 
                3,
                0,
                this.tilesetSizes[0].h,
                this.tilesetSizes[3].w,
                (h - this.tilesetSizes[0].h - this.tilesetSizes[6].h)
            );
        }

        if (this.tilesetSizes[5].w > 0 && this.tilesetSizes[5].h > 0) {
            this.drawPart(
                finalCtx, 
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
                finalCtx, 
                0,
                0,
                0,
                this.tilesetSizes[0].w,
                this.tilesetSizes[0].h
            );
        }

        if (this.tilesetSizes[2].w > 0 && this.tilesetSizes[2].h > 0) {
            this.drawPart(
                finalCtx, 
                2,
                (w -this.tilesetSizes[2].w),
                0,
                this.tilesetSizes[2].w,
                this.tilesetSizes[2].h
            );
        }

        if (this.tilesetSizes[6].w > 0 && this.tilesetSizes[6].h > 0) {
            this.drawPart(
                finalCtx, 
                6,
                0,
                (h - this.tilesetSizes[6].h),
                this.tilesetSizes[6].w,
                this.tilesetSizes[6].h
            );
        }

        if (this.tilesetSizes[8].w > 0 && this.tilesetSizes[8].h > 0) {
            this.drawPart(
                finalCtx, 
                8,
                (w - this.tilesetSizes[8].w),
                (h - this.tilesetSizes[8].h),
                this.tilesetSizes[8].w,
                this.tilesetSizes[8].h
            );
        }

        if (this.xScale < 0.0) {
            finalCtx.translate(this.width, 0);
            finalCtx.scale(-1, 1);
        }

        if (this.yScale < 0.0) {
            finalCtx.translate(0, this.height);
            finalCtx.scale(1, -1);
        }

        return finalCtx;
    },

    updateSize() {
        if (this.isIntegral && this.displayMode == 0) {
            if (this.tilesetSizes[4].w > 0 && this.tilesetSizes[4].h > 0) {
                var middleXSize = Math.ceil((this.width - (this.tilesetSizes[0].w + this.tilesetSizes[2].w)) / this.tilesetSizes[4].w);
                var middleYSize = Math.ceil((this.height - (this.tilesetSizes[0].h + this.tilesetSizes[6].h)) / this.tilesetSizes[4].h);
    
                this.width = (this.tilesetSizes[0].w + this.tilesetSizes[2].w) + (middleXSize * this.tilesetSizes[4].w);
                this.height = (this.tilesetSizes[0].h + this.tilesetSizes[6].h) + (middleYSize * this.tilesetSizes[4].h);
            } else {
                this.width = Math.max(this.minWidth, this.tilesetSizes[0].w + this.tilesetSizes[2].w);
			    this.height = Math.max(this.minHeight, this.tilesetSizes[0].h + this.tilesetSizes[6].h);
            }
        } else {
            this.width = Math.max(this.minWidth, this.width);
            this.height = Math.max(this.minHeight, this.height);
        }
    },

    setWidth(value) {
        this.width = Math.max(this.minWidth, value);

        this.updateSize();
    },

    setHeight(value) {
        this.height = Math.max(this.minHeight, value);

        this.updateSize();
    },

    setIntegral(is) {
        this.isIntegral = is;
        this.updateSize();
    },

    generateTileset(images) {
        images.forEach((image) => {
            var size = new OvigloSliceTilesetSize();
            size.w = image.width;
            size.h = image.height;
            this.tilesetSizes.push(size);
        });

        var w = this.tilesetSizes[0].w + this.tilesetSizes[1].w + this.tilesetSizes[2].w;
        var h = this.tilesetSizes[0].h + this.tilesetSizes[3].h + this.tilesetSizes[6].h;

        // Set min size
        this.minWidth = this.tilesetSizes[0].w + this.tilesetSizes[2].w;
        this.minHeight = this.tilesetSizes[0].h + this.tilesetSizes[6].h;

        this.tileset = document.createElement("canvas").getContext("2d");
        this.tileset.canvas.width = w;
        this.tileset.canvas.height = h;

        var xOffset = 0;
        var yOffset = 0;

        for (var i = 0; i < 3; i++) {
            xOffset = 0;
            for (var j = 0; j < 3; j++) {
                this.tileset["drawImage"](images[(i*3)+j], xOffset, yOffset);

                this.tilesetSizes[(i*3)+j].x = xOffset;
                this.tilesetSizes[(i*3)+j].y = yOffset;

                xOffset += this.tilesetSizes[(i*3)+j].w;
            }

            yOffset += this.tilesetSizes[i*3].h;
        }

        console.log(this.tilesetSizes);
    }
};

function CRun9SlicesObject()
{
    this.oSlices = new OvigloSlices();
    this.xHotSpot = 0;
    this.yHotSpot = 0;
    this.centerHotSpot = false;
    this.images = new Array(9);
    this.oldXHotSpot = 0;
    this.oldYHotSpot = 0;
    this.tilesetGenerated = false;
};

// Prototype definition
// -----------------------------------------------------------------
CRun9SlicesObject.prototype = CServices.extend(new CRunExtension(),
{
    loadBankImage(handle) {
        
        var image = this.rh.rhApp.imageBank.getImageFromHandle(handle);
        var tempCtx = document.createElement("canvas").getContext("2d");
        tempCtx.canvas.width = image.width;
        tempCtx.canvas.height = image.height;
        if (image != null) {
            if (image.mosaic == 0 && image.img != null) {
                tempCtx["drawImage"](image.img, 0, 0);
            } else {
                tempCtx["drawImage"](image.app.imageBank.mosaics[image.mosaic],
                    image.mosaicX, image.mosaicY,
                    image.width, image.height,
                    0, 0,
                    image.width, image.height
                );

            }
            return tempCtx.canvas;
        }

        return null;
    },

    getNumberOfConditions:function()
    {
        return 2;
    },

    // Creation of the object
    // --------------------------------------------------------------------
    // - file : a CFile object, pointing to the object's data zone
    // - cob : a CCreateObjectInfo containing infos about the created object
    // - version : the version number of the object, as defined in the C code
    createRunObject:function(file, cob, version)
    {
        this.oSlices.width = file.readAShort();
        this.oSlices.height = file.readAShort();
        this.ho.hoImgWidth = this.oSlices.width;
        this.ho.hoImgHeight = this.oSlices.height;
        var i = 0;
        for (i = 0; i < 9; i++) {
            this.images[i] = file.readAShort();
        }
        this.ho.loadImageList(this.images);

        this.oSlices.isIntegral = file.readAShort() == 1;
        this.xHotSpot = file.readAInt();
        this.yHotSpot = file.readAInt();

        this.centerHotSpot = file.readAByte() == 1;
        file.skipBytes(11);
        this.oSlices.displayMode = file.readAByte();

        this.oldXHotSpot = 0;
        this.oldYHotSpot = 0;

        return false;
    },

        // Handling of the object
    // ---------------------------------------------------------------------
    // This function is called at every loop of the game. You have to perform 
    // in it all the tasks necessary for your object to function.
    // Return values:
    //    - 0 : this function will be called during the next loop
    //    - CRunExtension.REFLAG_ONESHOT : this function will not be called anymore
    //      In this case, call the this.reHandle(); function of the base class 
    //      to have it called again.
    handleRunObject()
    {
        var images = new Array(9);
        for (var i = 0; i < 9; i++) {
            images[i] = this.loadBankImage(this.images[i]);
        }
        this.oSlices.generateTileset(images);
        this.tilesetGenerated = true;
        if (this.centerHotSpot) {
            this.xHotSpot = Math.abs(this.oSlices.width * this.oSlices.xScale) / 2;
            this.yHotSpot = Math.abs(this.oSlices.height * this.oSlices.yScale) / 2;
        }

        this.ho.hoImgWidth = Math.abs(this.oSlices.width * this.oSlices.xScale);
        this.ho.hoImgHeight = Math.abs(this.oSlices.height * this.oSlices.yScale);
        this.ho.hoX -= (this.xHotSpot - this.oldXHotSpot);
        this.ho.hoY -= (this.yHotSpot - this.oldYHotSpot);

        this.oldXHotSpot = this.xHotSpot;
        this.oldYHotSpot = this.yHotSpot;

        return CRunExtension.REFLAG_ONESHOT;
    },

    // Destruction of the object
    // ---------------------------------------------------------------
    // Called when the object is actually destroyed. This will always be
    // after the main game loop, and out of the actions processing : the 
    // destroy process is queued until the very end of the game loop.
    destroyRunObject(bFast)
    {
        this.oSlices = null;
    },

    // Displays the object
    // ----------------------------------------------------------------
    // Called when the object needs to be displayed in the browser.
    //    - renderer : the Renderer object which will draw the object
    //    - xDraw : an offset to add to every X coordinate for display
    //    - yDraw : an offset to add to every Y coordinate for display
    // This function will only be called if the object's flags in the C code
    // indicate that this is a displayable object (OEFLAG_SPRITE)
    displayRunObject(renderer, xDraw, yDraw)
    {
        if (this.oSlices.xScale == 0 || this.oSlices.yScale == 0) {
            return;
        }

        if (!this.tilesetGenerated) {
            this.ho.reHandle();
            return;
        }
        var x = this.ho.hoX - this.rh.rhWindowX + this.ho.pLayer.x + xDraw;
        var y = this.ho.hoY - this.rh.rhWindowY + this.ho.pLayer.y + yDraw;

        var finalCtx = this.oSlices.draw();

        renderer.renderSimpleImage(finalCtx.canvas, x, y, finalCtx.canvas.width, finalCtx.canvas.height, 0, 0);
    },

    condition(num, cnd)
    {
        switch(num) {
            case 0:
                return this.centerHotSpot;
            case 1:
                return this.oSlices.isIntegral;
        }

        return false;
    },

    action(num, act)
    {  
        switch(num) {
            case 0:
                var val = act.getParamExpression(this.rh, 0);
                this.oSlices.setWidth(val);
                this.ho.reHandle();
                break;
            case 1:
                var val = act.getParamExpression(this.rh, 0);
                this.oSlices.setHeight(val);
                this.ho.reHandle();
                break;
            case 2:
                var val = act.getParamExpression(this.rh, 0);
                this.oSlices.xScale = val;
                this.ho.reHandle();
                break;
            case 3:
                var val = act.getParamExpression(this.rh, 0);
                this.oSlices.yScale = val;
                this.ho.reHandle();
                break;
            case 4:
                var val = act.getParamExpression(this.rh, 0);
                this.xHotSpot = val;
                this.ho.reHandle();
                break;
            case 5:
                var val = act.getParamExpression(this.rh, 0);
                this.yHotSpot = val;
                this.ho.reHandle();
                break;
            case 6:
                this.centerHotSpot = true;
                this.ho.reHandle();
                break;
            case 7:
                this.centerHotSpot = false;
                this.ho.reHandle();
                break;
            case 8:
                this.oSlices.setIntegral(true);
                this.ho.reHandle();
                break;
            case 9:
                this.oSlices.setIntegral(false);
                this.ho.reHandle();
                break;
            case 10:
                var val = act.getParamExpression(this.rh, 0);
                this.oSlices.displayMode = val;
                this.ho.reHandle();
                break;
        }
    },

    expression(num)
    {
        switch(num) {
            case 0:
                return this.oSlices.xScale;
            case 1:
                return this.oSlices.yScale;
            case 2:
                return this.xHotSpot;
            case 3:
                return this.yHotSpot;
            case 4:
                return this.oSlices.displayMode;
        }
        return 0;
    }
});
