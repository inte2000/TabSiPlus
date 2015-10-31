#ifndef __HELPFUNCTION_H__
#define __HELPFUNCTION_H__

COLORREF DarkenColor(long lScale, COLORREF lColor);
COLORREF MixedColor(COLORREF ColA,COLORREF ColB);
COLORREF MidColor(COLORREF ColA,COLORREF ColB);
COLORREF GrayColor(COLORREF crColor);
COLORREF LightenColor(long lScale, COLORREF lColor);

void InflateRectEx(LPRECT lpRect,int l, int t, int r, int b);
void DeflateRectEx(LPRECT lpRect,int l, int t, int r, int b);

void TransBlt2(HDC hDestDC,int nxdest,int nydest,int nwidth,int nheight,HDC hSrcDC,COLORREF colortransparent,int nxsrc,int nysrc,int nwSrc = 0,int nhSrc = 0,HPALETTE hpal = NULL);

void DrawBitmapObject(HDC hDC,const RECT& rcClient,HDC hBmpDC,const RECT& rcBtnBmp,const RECT& rcBmpMargin,BOOL bTransparent,COLORREF crTransparent);

// There are helper functions for drawing 3D frames
void FillSolidRect(HDC hDC,const RECT& Rc,COLORREF crColor);
void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF crColor);

void Draw3dRect(HDC hDC, int x, int y, int cx, int cy,COLORREF clrTopLeft, COLORREF clrBottomRight);
void Draw3dRect(HDC hDC, const RECT& rect, COLORREF clrTopLeft, COLORREF clrBottomRight);

#endif //__HELPFUNCTION_H__