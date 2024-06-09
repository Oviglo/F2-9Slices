#pragma once
#include "ccxhdr.h"
#include "Surface.h"

struct TilesetSize {
	int w=0;
	int h=0;
	int x=0;
	int y=0;
};

class Slices
{
private:
	unsigned short m_images[9];
	int m_width = 32;
	int m_height = 32;
	int m_minWidth = 32;
	int m_minHeight = 32;
	int m_displayMode = 0;
	bool m_isIntegral = true;
	float xScale = 1.0;
	float yScale = 1.0;
	cSurface sTileset;
	TilesetSize tilesetSizes[9];
	bool m_isHwa = false;

	void drawPart(LPSURFACE source, LPSURFACE dest, int imageId, int x =0, int y=0, int w = 32, int h = 32);
	void updateSize();
	BOOL Blit(LPSURFACE src, LPSURFACE dest, int dX, int dY, int dW, int dH, int sX, int sY, int sW, int sH, POINT pCenter, float fAngle);

public:
	bool generateTileset(WORD images[], LPSURFACE surf, void* appId);
	void setWidth(int width);
	void setHeight(int height);
	void setMinWidth(int value);
	void setMinHeight(int value);
	int getMinWidth();
	int getMinHeight();
	int getWidth();
	int getHeight();
	void setXScale(float value);
	void setYScale(float value);
	float getXScale();
	float getYScale();
	void setIntegral(bool isIntegral);
	bool getIntegral();
	unsigned short* getImages();
	void setDisplayMode(int mode);
	int getDisplayMode();

	cSurface& getTileset();

	void draw(LPSURFACE surf, cSurface& sFinal);

};

