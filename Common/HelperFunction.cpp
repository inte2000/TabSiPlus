#include "stdafx.h"
#include "HelperFunction.h"


// Function splits a color into its RGB components and transforms the color using a scale 0..255
COLORREF DarkenColor(long lScale, COLORREF lColor)
{ 
    long R = MulDiv(GetRValue(lColor),(255-lScale),255);
    long G = MulDiv(GetGValue(lColor),(255-lScale),255);
    long B = MulDiv(GetBValue(lColor),(255-lScale),255);

    return RGB(R, G, B); 
}

COLORREF MixedColor(COLORREF ColA,COLORREF ColB)
{
    // ( 86a + 14b ) / 100
    int Red   = MulDiv(86,GetRValue(ColA),100) + MulDiv(14,GetRValue(ColB),100);
    int Green = MulDiv(86,GetGValue(ColA),100) + MulDiv(14,GetGValue(ColB),100);
    int Blue  = MulDiv(86,GetBValue(ColA),100) + MulDiv(14,GetBValue(ColB),100);

    return RGB( Red,Green,Blue);
}

COLORREF MidColor(COLORREF ColA,COLORREF ColB)
{ 
    // (7a + 3b)/10
    int Red   = MulDiv(7,GetRValue(ColA),10) + MulDiv(3,GetRValue(ColB),10);
    int Green = MulDiv(7,GetGValue(ColA),10) + MulDiv(3,GetGValue(ColB),10);
    int Blue  = MulDiv(7,GetBValue(ColA),10) + MulDiv(3,GetBValue(ColB),10);

    return RGB( Red,Green,Blue);
}

COLORREF GrayColor(COLORREF crColor)
{ 
    int Gray  = (((int)GetRValue(crColor)) + GetGValue(crColor) + GetBValue(crColor))/3;
    return RGB( Gray,Gray,Gray);
}

// Function splits a color into its RGB components and transforms the color using a scale 0..255
COLORREF LightenColor(long lScale, COLORREF lColor)
{ 
    long R = MulDiv(255-GetRValue(lColor),lScale,255)+GetRValue(lColor);
    long G = MulDiv(255-GetGValue(lColor),lScale,255)+GetGValue(lColor);
    long B = MulDiv(255-GetBValue(lColor),lScale,255)+GetBValue(lColor);

    return RGB(R, G, B); 
}

void InflateRectEx(LPRECT lpRect,int l, int t, int r, int b)
{
    lpRect->left -= l;
    lpRect->top -= t;
    lpRect->right += r;
    lpRect->bottom += b;
}

void DeflateRectEx(LPRECT lpRect,int l, int t, int r, int b)
{
    lpRect->left += l;
    lpRect->top += t;
    lpRect->right -= r;
    lpRect->bottom -= b;
}

//将hSrcDC 中的某个区域贴到hDestDC中指定的区域，支持拉伸显示，支持透明色
//hSrcDC是hDestDC的兼容DC
void TransBlt2(HDC hDestDC,int nxdest,int nydest,int nwidth,int nheight,HDC hSrcDC,COLORREF colortransparent,int nxsrc,int nysrc,int nwSrc,int nhSrc,HPALETTE hpal)
{
    BOOL bStretch = ((nwSrc != 0) && (nwSrc != nwidth)) || ((nhSrc != 0) && (nhSrc != nheight));
    HDC hMaskDC = CreateCompatibleDC(hDestDC);
    HDC hMemDC = CreateCompatibleDC(hDestDC);
    
    //add these to store return of selectobject() calls
    HGDIOBJ hOldMemBmp = NULL;
    HGDIOBJ hOldMaskBbmp = NULL;
    
    HBITMAP hBmpImage = CreateCompatibleBitmap(hDestDC,nwidth,nheight );
    hOldMemBmp = SelectObject(hMemDC,hBmpImage);
    
    // select and realize the palette
    if(GetDeviceCaps(hDestDC,RASTERCAPS) & RC_PALETTE && hpal )
    {
        SelectPalette(hDestDC,hpal,FALSE);
        RealizePalette(hDestDC);
        
        SelectPalette(hMemDC,hpal,FALSE);
    }
    
    if(bStretch)
        StretchBlt(hMemDC,0,0,nwidth,nheight,hSrcDC,nxsrc,nysrc,nwSrc,nhSrc,SRCCOPY);
    else
        BitBlt(hMemDC,0,0,nwidth,nheight,hSrcDC,nxsrc,nysrc,SRCCOPY);
    
    // create monochrome bitmap for the mask
    HBITMAP hMaskBitmap = CreateBitmap(nwidth,nheight,1,1,NULL);
    hOldMaskBbmp = SelectObject(hMaskDC,hMaskBitmap);
    SetBkColor(hMemDC,colortransparent);
    
    // create the mask from the memory dc
    BitBlt(hMaskDC,0,0,nwidth,nheight,hMemDC,0,0,SRCCOPY);
    
    // set the background../di2001.jpgn memdc to black. using srcpaint with black 
    // and any other color results in the other color, thus making black the transparent color
    SetBkColor(hMemDC,RGB(0,0,0));
    SetTextColor(hMemDC,RGB(255,255,255));
    BitBlt(hMemDC,0,0,nwidth,nheight,hMaskDC,0,0,SRCAND);
    
    // set the foreground to black. see comment above.
    SetBkColor(hDestDC,RGB(255,255,255));
    SetTextColor(hDestDC,RGB(0,0,0));
    BitBlt(hDestDC,nxdest, nydest, nwidth, nheight, hMaskDC, 0, 0, SRCAND);
    
    // combine the foreground with the background../di2001.jpgd
    BitBlt(hDestDC,nxdest, nydest, nwidth, nheight, hMemDC, 0, 0, SRCPAINT);
    
    SelectObject(hMaskDC,hOldMaskBbmp);
    SelectObject(hMemDC,hOldMemBmp);
    
    DeleteDC(hMaskDC);
    DeleteDC(hMemDC);

    DeleteObject(hBmpImage);
    DeleteObject(hMaskBitmap);
}

void DrawBitmapObject(HDC hDC,const RECT& rcClient,HDC hBmpDC,const RECT& rcBtnBmp,const RECT& rcBmpMargin,BOOL bTransparent,COLORREF crTransparent)
{
    if(rcBmpMargin.left == 0 && rcBmpMargin.right == 0 && rcBmpMargin.top == 0 && rcBmpMargin.bottom == 0)//画没有边界地位图，整体一起画
    {
        if(bTransparent)
            TransBlt2(hDC,rcClient.left,rcClient.top,rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,hBmpDC,crTransparent,
                rcBtnBmp.left,rcBtnBmp.top,rcBtnBmp.right - rcBtnBmp.left,rcBtnBmp.bottom - rcBtnBmp.top);
        else
            StretchBlt(hDC,rcClient.left,rcClient.top,rcClient.right - rcClient.left,rcClient.bottom - rcClient.top,hBmpDC,
                rcBtnBmp.left,rcBtnBmp.top,rcBtnBmp.right - rcBtnBmp.left,rcBtnBmp.bottom - rcBtnBmp.top,SRCCOPY);
    }
    else//位图有边界
    {
        RECT rcBtnBmpInner = rcBtnBmp;
        DeflateRectEx(&rcBtnBmpInner,rcBmpMargin.left,rcBmpMargin.top,rcBmpMargin.right,rcBmpMargin.bottom);
        RECT rcClientInner = rcClient;
        DeflateRectEx(&rcClientInner,rcBmpMargin.left,rcBmpMargin.top,rcBmpMargin.right,rcBmpMargin.bottom);

        if(bTransparent)//透明位图
        {
            //左上角
            TransBlt2(hDC,rcClient.left,rcClient.top,rcBmpMargin.left,rcBmpMargin.top,hBmpDC,crTransparent,
                        rcBtnBmp.left,rcBtnBmp.top,rcBmpMargin.left,rcBmpMargin.top);
            //右上角
            TransBlt2(hDC,rcClient.right - rcBmpMargin.right,rcClient.top,rcBmpMargin.right,rcBmpMargin.top,hBmpDC,crTransparent,
                        rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.top,rcBmpMargin.right,rcBmpMargin.top);
            //左下角
            TransBlt2(hDC,rcClient.left,rcClient.bottom - rcBmpMargin.bottom,rcBmpMargin.left,rcBmpMargin.bottom,hBmpDC,crTransparent,
                        rcBtnBmp.left,rcBtnBmp.bottom - rcBmpMargin.bottom,rcBmpMargin.left,rcBmpMargin.bottom);
            //右下角
            TransBlt2(hDC,rcClient.right - rcBmpMargin.right,rcClient.bottom - rcBmpMargin.bottom,rcBmpMargin.right,rcBmpMargin.bottom,hBmpDC,crTransparent,
                        rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.bottom - rcBmpMargin.bottom,rcBmpMargin.right,rcBmpMargin.bottom);

            if(rcClientInner.bottom > rcClientInner.top)//按钮足够高，需要画左右边界
            {
                //左边界
                TransBlt2(hDC,rcClient.left,rcClient.top + rcBmpMargin.top,rcBmpMargin.left,rcClientInner.bottom - rcClientInner.top,hBmpDC,crTransparent,
                            rcBtnBmp.left,rcBtnBmp.top + rcBmpMargin.top,rcBmpMargin.left,rcBtnBmpInner.bottom - rcBtnBmpInner.top);
                //右边界    
                TransBlt2(hDC,rcClient.right - rcBmpMargin.right,rcClient.top + rcBmpMargin.top,rcBmpMargin.right,rcClientInner.bottom - rcClientInner.top,hBmpDC,crTransparent,
                            rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.top + rcBmpMargin.top,rcBmpMargin.right,rcBtnBmpInner.bottom - rcBtnBmpInner.top);
            }
            if(rcClientInner.right > rcClientInner.left)//按钮足够宽，需要画上下边界
            {
                //上边界    
                TransBlt2(hDC,rcClient.left + rcBmpMargin.left,rcClient.top,rcClientInner.right - rcClientInner.left,rcBmpMargin.top,hBmpDC,crTransparent,
                            rcBtnBmp.left + rcBmpMargin.left,rcBtnBmp.top,rcBtnBmpInner.right - rcBtnBmpInner.left,rcBmpMargin.top);
                //下边界
                TransBlt2(hDC,rcClient.left + rcBmpMargin.left,rcClient.bottom - rcBmpMargin.bottom,rcClientInner.right - rcClientInner.left,rcBmpMargin.bottom,hBmpDC,crTransparent,
                            rcBtnBmp.left + rcBmpMargin.left,rcBtnBmp.bottom - rcBmpMargin.bottom,rcBtnBmpInner.right - rcBtnBmpInner.left,rcBmpMargin.bottom);
            }
        }
        else
        {
            //左上角
            BitBlt(hDC,rcClient.left,rcClient.top,rcBmpMargin.left,rcBmpMargin.top,hBmpDC,rcBtnBmp.left,rcBtnBmp.top,SRCCOPY);
            //右上角
            BitBlt(hDC,rcClient.right - rcBmpMargin.right,rcClient.top,rcBmpMargin.right,rcBmpMargin.top,hBmpDC,rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.top,SRCCOPY);
            //左下角
            BitBlt(hDC,rcClient.left,rcClient.bottom - rcBmpMargin.bottom,rcBmpMargin.left,rcBmpMargin.bottom,hBmpDC,rcBtnBmp.left,rcBtnBmp.bottom - rcBmpMargin.bottom,SRCCOPY);
            //右下角
            BitBlt(hDC,rcClient.right - rcBmpMargin.right,rcClient.bottom - rcBmpMargin.bottom,rcBmpMargin.right,rcBmpMargin.bottom,hBmpDC,rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.bottom - rcBmpMargin.bottom,SRCCOPY);

            if(rcClientInner.bottom > rcClientInner.top)//按钮足够高，需要画左右边界
            {
                //左边界
                StretchBlt(hDC,rcClient.left,rcClient.top + rcBmpMargin.top,rcBmpMargin.left,rcClientInner.bottom - rcClientInner.top,hBmpDC,
                            rcBtnBmp.left,rcBtnBmp.top + rcBmpMargin.top,rcBmpMargin.left,rcBtnBmpInner.bottom - rcBtnBmpInner.top,SRCCOPY);
                //右边界    
                StretchBlt(hDC,rcClient.right - rcBmpMargin.right,rcClient.top + rcBmpMargin.top,rcBmpMargin.right,rcClientInner.bottom - rcClientInner.top,hBmpDC,
                            rcBtnBmp.right - rcBmpMargin.right,rcBtnBmp.top + rcBmpMargin.top,rcBmpMargin.right,rcBtnBmpInner.bottom - rcBtnBmpInner.top,SRCCOPY);
            }
            if(rcClientInner.right > rcClientInner.left)//按钮足够宽，需要画上下边界
            {
                //上边界    
                StretchBlt(hDC,rcClient.left + rcBmpMargin.left,rcClient.top,rcClientInner.right - rcClientInner.left,rcBmpMargin.top,hBmpDC,
                            rcBtnBmp.left + rcBmpMargin.left,rcBtnBmp.top,rcBtnBmpInner.right - rcBtnBmpInner.left,rcBmpMargin.top,SRCCOPY);
                //下边界
                StretchBlt(hDC,rcClient.left + rcBmpMargin.left,rcClient.bottom - rcBmpMargin.bottom,rcClientInner.right - rcClientInner.left,rcBmpMargin.bottom,hBmpDC,
                            rcBtnBmp.left + rcBmpMargin.left,rcBtnBmp.bottom - rcBmpMargin.bottom,rcBtnBmpInner.right - rcBtnBmpInner.left,rcBmpMargin.bottom,SRCCOPY);
            }
        }
        //中间
        StretchBlt(hDC,rcClientInner.left,rcClientInner.top,rcClientInner.right - rcClientInner.left,rcClientInner.bottom - rcClientInner.top,hBmpDC,
            rcBtnBmpInner.left,rcBtnBmpInner.top,rcBtnBmpInner.right - rcBtnBmpInner.left,rcBtnBmpInner.bottom - rcBtnBmpInner.top,SRCCOPY);
    }
}

// Helper functions for drawing 3D frames (borrowed from CDC class)
void FillSolidRect(HDC hDC,const RECT& Rc,COLORREF crColor)
{
    SetBkColor(hDC,crColor);
    ExtTextOut(hDC,0,0,ETO_OPAQUE,&Rc,NULL,0,NULL);
}

void FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF crColor)
{
    SetBkColor( hDC, crColor );
    RECT rect;
    SetRect(&rect, x, y, x + cx, y + cy );
    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL );
}

void Draw3dRect( HDC hDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight )
{
   FillSolidRect( hDC, x, y, cx - 1, 1, clrTopLeft );
   FillSolidRect( hDC, x, y, 1, cy - 1, clrTopLeft );
   FillSolidRect( hDC, x + cx, y, -1, cy, clrBottomRight );
   FillSolidRect( hDC, x, y + cy, cx, -1, clrBottomRight );
}

void Draw3dRect( HDC hDC, const RECT& rect, COLORREF clrTopLeft, COLORREF clrBottomRight )
{
   Draw3dRect( hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, clrTopLeft, clrBottomRight );
}
