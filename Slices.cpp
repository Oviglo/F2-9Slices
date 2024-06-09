#include "Slices.h"
#include "Surface.h"

BOOL Slices::Blit(LPSURFACE src, LPSURFACE dest, int dX, int dY, int dW, int dH, int sX, int sY, int sW, int sH, POINT pCenter, float fAngle) {
	if (sW == 0 || sH == 0)
		return 0;

#ifdef HWABETA
	if (this->m_isHwa) {
		float xScale = dW / sW;
		float yScale = dH / sH;
		
		return src->BlitEx(*dest, dX, dY, 2, 2, sX, sY, sW, sH, &pCenter, fAngle);
	}
#endif

	if (dW != sW || dH != sH) {
		
		return src->Stretch(*dest, dX, dY, dW, dH, sX, sY, sW, sH, BMODE_OPAQUE, BOP_COPY, 0, STRF_COPYALPHA);
	}

	return src->Blit(*dest, dX, dY, sX, sY, sW, sH, BMODE_OPAQUE, BOP_COPY, 0, BLTF_COPYALPHA);
}

bool Slices::generateTileset(WORD images[], LPSURFACE surf, void* appId) {

	// Draw new tileset
	Img imageInfos[9];
	for (int i = 0; i < 9; i++) {
		if (images[i] == 0) {
			this->m_minWidth = 32;
			this->m_minHeight = 32;
			this->updateSize();
			return false;
		}
			
		GetImageInfos(appId, images[i], &imageInfos[i]);
	}
	short w = imageInfos[0].imgWidth + imageInfos[1].imgWidth + imageInfos[2].imgWidth;
	short h = imageInfos[0].imgHeight + imageInfos[3].imgHeight + imageInfos[6].imgHeight;

	// set min size
	this->m_minWidth = max(32, imageInfos[0].imgWidth + imageInfos[2].imgWidth);
	this->m_minHeight = max(32, imageInfos[0].imgHeight + imageInfos[6].imgHeight);

	cSurface* pTilesetProto;
	int nType = ST_HWA_ROMTEXTURE;
	int nDrv = SD_DIB;
	int depth = 24;
	if (surf != NULL) {
		depth = surf->GetDepth();
		nDrv = surf->GetDriver();
		this->m_isHwa = nDrv >= SD_3DFX;
		if (nDrv == SD_DDRAW)
			nDrv = SD_DIB;
		if (nDrv != SD_DIB) {
			nType = 0;
		}
	}
	
	GetSurfacePrototype(&pTilesetProto, depth, nType, nDrv);

	if (w <= 0 || h <= 0) {

		return false;
	}

	this->sTileset.Create(w, h, pTilesetProto);

	int xOffset = 0;
	int yOffset = 0;
	cSurface imgSurf;
	
	for (int i = 0; i < 3; i++) {
		xOffset = 0;
		for (int j = 0; j < 3; j++) {
			LockImageSurface(appId, images[(i*3)+j], imgSurf, LOCKIMAGE_READBLITONLY);
			imgSurf.Blit(this->sTileset, xOffset, yOffset, BMODE_OPAQUE,
				BOP_COPY,
				0,
				BLTF_COPYALPHA);
			UnlockImageSurface(imgSurf);

			tilesetSizes[(i * 3) + j].w = imageInfos[(i * 3) + j].imgWidth;
			tilesetSizes[(i * 3) + j].h = imageInfos[(i * 3) + j].imgHeight;
			tilesetSizes[(i * 3) + j].x = xOffset;
			tilesetSizes[(i * 3) + j].y = yOffset;

			xOffset += imageInfos[(i * 3) + j].imgWidth;
		}

		yOffset += imageInfos[i*3].imgHeight;
	}

	return true;

	//this->updateSize();
}

void Slices::setWidth(int width)
{
	m_width = max(width, m_minWidth);
	this->updateSize();
}

void Slices::setHeight(int height)
{
	m_height = max(height, m_minHeight);
	this->updateSize();
}

void Slices::setMinWidth(int value)
{
	m_minWidth = value;
}

void Slices::setMinHeight(int value)
{
	m_minHeight = value;
}

int Slices::getWidth()
{
	return m_width;
}

int Slices::getHeight()
{
	return m_height;
}

void Slices::setXScale(float value)
{
	xScale = value;
}

void Slices::setYScale(float value)
{
	yScale = value;
}

float Slices::getXScale()
{
	return xScale;
}

float Slices::getYScale()
{
	return yScale;
}

void Slices::setIntegral(bool isIntegral)
{
	this->m_isIntegral = isIntegral;
	this->updateSize();
}

bool Slices::getIntegral()
{
	return this->m_isIntegral;
}

int Slices::getMinWidth()
{
	return m_minWidth;
}

int Slices::getMinHeight()
{
	return m_minHeight;
}

unsigned short* Slices::getImages()
{
	return m_images;
}

void Slices::setDisplayMode(int mode)
{
	this->m_displayMode = mode;
}

int Slices::getDisplayMode()
{
	return this->m_displayMode;
}

cSurface& Slices::getTileset()
{
	return this->sTileset;
}

void Slices::drawPart(LPSURFACE source, LPSURFACE dest, int imageId, int x, int y, int w, int h)
{
	if (0 == m_displayMode) { // Tile display mode
		float i = 0;
		float j = 0;
		int initW = w;
		int initH = h;
		int repeatX = floor(w / tilesetSizes[imageId].w);
		int repeatY = floor(h / tilesetSizes[imageId].h);
		if (!dest->HasAlpha() && source->HasAlpha())
			dest->CreateAlpha();

		for (i = 0; i < repeatX; i++) {
			initH = h;
			for (j = 0; j < repeatY; j++) {
				source->Blit(
					*dest,
					x + (i * tilesetSizes[imageId].w),
					y + (j * tilesetSizes[imageId].h),
					tilesetSizes[imageId].x, 
					tilesetSizes[imageId].y,
					tilesetSizes[imageId].w,
					tilesetSizes[imageId].h,
					BMODE_OPAQUE,
					BOP_COPY,
					0,
					BLTF_COPYALPHA
				);
				initH -= tilesetSizes[imageId].h;
			}

			// Draw verticaly complete part
			if (initH > 0) {
				source->Blit(
					*dest,
					x + (i * tilesetSizes[imageId].w),
					y + (j * tilesetSizes[imageId].h),
					tilesetSizes[imageId].x,
					tilesetSizes[imageId].y,
					tilesetSizes[imageId].w,
					initH,
					BMODE_OPAQUE,
					BOP_COPY,
					0,
					BLTF_COPYALPHA
				);
			}

			initW -= tilesetSizes[imageId].w;
		}

		// Draw horizontaly complete part
		if (initW > 0) {
			for (j = 0; j < repeatY; j++) {
				source->Blit(
					*dest,
					x + (i * tilesetSizes[imageId].w),
					y + (j * tilesetSizes[imageId].h),
					tilesetSizes[imageId].x,
					tilesetSizes[imageId].y,
					initW,
					tilesetSizes[imageId].h,
					BMODE_OPAQUE,
					BOP_COPY,
					0,
					BLTF_COPYALPHA
				);
			}
		}

		if (initH > 0 && initW > 0) {
			
			source->Blit(
				*dest,
				x + (i * tilesetSizes[imageId].w),
				y + (j * tilesetSizes[imageId].h),
				tilesetSizes[imageId].x,
				tilesetSizes[imageId].y,
				initW,
				initH,
				BMODE_OPAQUE,
				BOP_COPY,
				0,
				BLTF_COPYALPHA
			);
		}
	}
	else { // Stretch display mode (opacity issue)
		source->Stretch(
			*dest,
			x,
			y,
			w,
			h,
			tilesetSizes[imageId].x,
			tilesetSizes[imageId].y,
			tilesetSizes[imageId].w,
			tilesetSizes[imageId].h,
			BMODE_OPAQUE,
			BOP_COPY,
			0,
			STRF_COPYALPHA
		);
	}
}

void Slices::updateSize()
{
	if (m_isIntegral && m_displayMode == 0) {
		if (tilesetSizes[4].w > 0 && tilesetSizes[4].h > 0) {
			int middleXSize = ceil((m_width - (tilesetSizes[0].w + tilesetSizes[2].w)) / tilesetSizes[4].w);
			int middleYSize = ceil((m_height - (tilesetSizes[0].h + tilesetSizes[6].h)) / tilesetSizes[4].h);

			m_width = (tilesetSizes[0].w + tilesetSizes[2].w) + (middleXSize * tilesetSizes[4].w);
			m_height = (tilesetSizes[0].h + tilesetSizes[6].h) + (middleYSize * tilesetSizes[4].h);
		}
		else {
			m_width = m_minWidth;
			m_height = m_minHeight;
		}
		
	}
	else {
		m_width = max(m_minWidth, m_width);
		m_height = max(m_minHeight, m_height);
	}
}

void Slices::draw(LPSURFACE surf, cSurface& sFinal)
{
	/*
	 -------------
	 | 0 | 1 | 2 |
	 -------------
	 | 3 | 4 | 5 |
	 -------------
	 | 6 | 7 | 8 |
	 -------------
	*/

	int w = abs((int)(m_width * xScale));
	int h = abs((int)(m_height * yScale));
	
	cSurface* pFinalProto;
	int nType = ST_MEMORYWITHDC;
	int nDrv = surf->GetDriver();
	if (nDrv == SD_DDRAW)
		nDrv = SD_DIB;
	if (nDrv != SD_DIB) {
		nType = 0;
	}
	GetSurfacePrototype(&pFinalProto, surf->GetDepth(), nType, nDrv);

	sFinal.Create(w, h, pFinalProto);

	//-----------------------
	// Image 1, 7
	//top bottom border
	//-----------------------
	if (tilesetSizes[1].w > 0 && tilesetSizes[1].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			1,
			tilesetSizes[0].w,
			0,
			(w - tilesetSizes[0].w - tilesetSizes[2].w),
			tilesetSizes[1].h
		);

	if (tilesetSizes[7].w > 0 && tilesetSizes[7].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			7,
			tilesetSizes[6].w,
			(h - tilesetSizes[7].h),
			(w - tilesetSizes[6].w - tilesetSizes[8].w),
			tilesetSizes[7].h
		);

	//-----------------------
	// Image 4
	// Center image
	//-----------------------
	if (tilesetSizes[4].w > 0 && tilesetSizes[4].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			4,
			tilesetSizes[3].w,
			tilesetSizes[1].h,
			(w - tilesetSizes[3].w - tilesetSizes[5].w),
			(h - tilesetSizes[1].h - tilesetSizes[7].h)
		);
	//-----------------------
	// Image 3, 5
	// left right border
	//-----------------------
	if (tilesetSizes[3].w > 0 && tilesetSizes[3].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			3,
			0,
			tilesetSizes[0].h,
			tilesetSizes[3].w,
			(h - tilesetSizes[0].h - tilesetSizes[6].h)
		);

	if (tilesetSizes[5].w > 0 && tilesetSizes[5].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			5,
			(w - tilesetSizes[5].w),
			tilesetSizes[0].h,
			tilesetSizes[5].w,
			(h - tilesetSizes[0].h - tilesetSizes[6].h)
		);
	//-----------------------
	// Image 0, 2, 6, 8
	// no repeat images
	//-----------------------
	if (tilesetSizes[0].w > 0 && tilesetSizes[0].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			0,
			0,
			0,
			tilesetSizes[0].w,
			tilesetSizes[0].h
		);

	if (tilesetSizes[2].w > 0 && tilesetSizes[2].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			2, // imgId
			(w - tilesetSizes[2].w), // x
			0, // y
			tilesetSizes[2].w,
			tilesetSizes[2].h
		);

	if (tilesetSizes[6].w > 0 && tilesetSizes[6].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			6, // imgId
			0, // x
			(h - tilesetSizes[6].h), // y
			tilesetSizes[6].w,
			tilesetSizes[6].h
		);

	if (tilesetSizes[8].w > 0 && tilesetSizes[8].h > 0)
		drawPart(
			&sTileset,
			&sFinal,
			8, // imgId
			(w - tilesetSizes[8].w), // x
			(h - tilesetSizes[8].h), // y
			tilesetSizes[8].w,
			tilesetSizes[8].h
		);

	if (xScale < 0.0)
		sFinal.ReverseX();

	if (yScale < 0.0)
		sFinal.ReverseY();
}
