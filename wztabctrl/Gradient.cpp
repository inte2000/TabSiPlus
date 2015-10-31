// Gradient.cpp: implementation of the CGradient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Gradient.h"
#include "math.h"


CGradient::CGradient()
{
}

CGradient::~CGradient()
{
}

void CGradient::HorizontalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;

    // Draw gradient
    HBRUSH hBrush = NULL;
    double percent;
    unsigned char red, green, blue;
    COLORREF color;
    RECT rect;
    for (int i=0; i<width-1; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i / (double)(width-2);

        // Gradient color
        red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
            green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
            blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
        }
        color = RGB(red, green, blue);

        // Gradient
        rect.left = lpRect->left + i + 1;
        rect.top = lpRect->top + 1;
        rect.right = rect.left + 1;
        rect.bottom = lpRect->bottom - 1;
        hBrush = CreateSolidBrush(color);
        FillRect(hDC, &rect, hBrush);
        DeleteObject(hBrush);
    }
}

void CGradient::VerticalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;

    // Draw gradient
    HBRUSH hBrush = NULL;
    double percent;
    unsigned char red, green, blue;
    COLORREF color;
    RECT rect;
    for (int i=0; i<height-1; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i / (double)(height-2);

        // Gradient color
        red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
            green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
            blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
        }
        color = RGB(red, green, blue);

        // Gradient
        rect.left = lpRect->left + 1;
        rect.top = lpRect->top + i + 1;
        rect.right = lpRect->right - 1;
        rect.bottom = rect.top + 1;
        hBrush = CreateSolidBrush(color);
        FillRect(hDC, &rect, hBrush);
        DeleteObject(hBrush);
    }
}

void CGradient::ForwardDiagonalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;
    double angle = (double)height / (double)width;

    // Draw gradient
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    double percent;
    unsigned char sRed, sGreen, sBlue, eRed, eGreen, eBlue;
    COLORREF colorStart, colorEnd;
    int offsetLeft = lpRect->top + 1;
    int offsetRight = lpRect->bottom - 1;
    int offsetTop = lpRect->left + 1;
    int offsetBottom = lpRect->bottom - 1;
    POINT pointStart, pointEnd;
    POINT sPoint, ePoint;
    int i, j;
    for (i=0; i<=width; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i/(double)(2*width);

        // Start color
        sRed = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        sGreen = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        sBlue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            sRed = (unsigned char)(pow((double)sRed/255.0, gamma) * 255);
            sGreen = (unsigned char)(pow((double)sGreen/255.0, gamma) * 255);
            sBlue = (unsigned char)(pow((double)sBlue/255.0, gamma) * 255);
        }
        colorStart = RGB(sRed, sGreen, sBlue);

        // End color
        eRed = (unsigned char)(GetRValue(eColor)*percent) + (unsigned char)(GetRValue(sColor)*(1-percent));
        eGreen = (unsigned char)(GetGValue(eColor)*percent) + (unsigned char)(GetGValue(sColor)*(1-percent));
        eBlue = (unsigned char)(GetBValue(eColor)*percent) + (unsigned char)(GetBValue(sColor)*(1-percent));
        if (bGamma)
        {
            eRed = (unsigned char)(pow((double)eRed/255.0, gamma) * 255);
            eGreen = (unsigned char)(pow((double)eGreen/255.0, gamma) * 255);
            eBlue = (unsigned char)(pow((double)eBlue/255.0, gamma) * 255);
        }
        colorEnd = RGB(eRed, eGreen, eBlue);

        // Start and end points on the left side
        pointStart.x = lpRect->left + 1;
        pointStart.y = lpRect->top+(int)(angle*i) + 1;
        if (pointStart.y > lpRect->bottom-2)
            pointStart.y = lpRect->bottom-2;
        pointEnd.x = lpRect->left+i+1;
        if (pointEnd.x > lpRect->right-2)
            pointEnd.x = lpRect->right-2;
        pointEnd.y = lpRect->top + 1;

        // Left side
        hPen = CreatePen(PS_SOLID, 1, colorStart);
        hOldPen = (HPEN)SelectObject(hDC, hPen);
        MoveToEx(hDC, pointStart.x, pointStart.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        // Filling holes
        MoveToEx(hDC, offsetTop, pointEnd.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        for (j=offsetLeft; j<=pointStart.y; j++)
        {
            sPoint.x = pointStart.x;
            sPoint.y = j;
            ePoint.x = pointEnd.x;
            ePoint.y = pointEnd.y;

            MoveToEx(hDC, sPoint.x, sPoint.y, NULL);
            LineTo(hDC, ePoint.x, ePoint.y);
        }
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
        offsetLeft = pointStart.y;
        offsetTop = pointEnd.x;

        // Start and end points on the right side
        pointStart.x = lpRect->right-i-2;
        if (pointStart.x < lpRect->left+1)
            pointStart.x = lpRect->left+1;
        pointStart.y = lpRect->bottom-2;
        pointEnd.x = lpRect->right-2;
        pointEnd.y = lpRect->bottom-(int)(angle*i)-2;
        if (pointEnd.y < lpRect->top+1)
            pointEnd.y = lpRect->top+1;

        // Right side
        hPen = CreatePen(PS_SOLID, 1, colorEnd);
        hOldPen = (HPEN)SelectObject(hDC, hPen);
        MoveToEx(hDC, pointStart.x, pointStart.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        // Filling holes
        MoveToEx(hDC, pointEnd.x, offsetBottom, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y-1);
        for (j=offsetRight; j>=pointEnd.y; j--)
        {
            sPoint.x = pointStart.x;
            sPoint.y = pointStart.y;
            ePoint.x = pointEnd.x;
            ePoint.y = j;

            MoveToEx(hDC, sPoint.x, sPoint.y, NULL);
            LineTo(hDC, ePoint.x, ePoint.y);
        }
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
        offsetRight = pointEnd.y;
        offsetBottom = pointEnd.y;
    }
}

void CGradient::BackwardDiagonalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;
    double angle = (double)height / (double)width;

    // Draw gradient
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    double percent;
    unsigned char sRed, sGreen, sBlue, eRed, eGreen, eBlue;
    COLORREF colorStart, colorEnd;
    int offsetLeft = lpRect->bottom - 2;
    int offsetRight = lpRect->top + 1;
    int offsetTop = lpRect->left + 1;
    int offsetBottom = lpRect->bottom - 1;
    POINT pointStart, pointEnd;
    POINT sPoint, ePoint;
    int i, j;
    for (i=0; i<=width; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i/(double)(2*width);

        // Start color
        sRed = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        sGreen = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        sBlue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            sRed = (unsigned char)(pow((double)sRed/255.0, gamma) * 255);
            sGreen = (unsigned char)(pow((double)sGreen/255.0, gamma) * 255);
            sBlue = (unsigned char)(pow((double)sBlue/255.0, gamma) * 255);
        }
        colorStart = RGB(sRed, sGreen, sBlue);

        // End color
        eRed = (unsigned char)(GetRValue(eColor)*percent) + (unsigned char)(GetRValue(sColor)*(1-percent));
        eGreen = (unsigned char)(GetGValue(eColor)*percent) + (unsigned char)(GetGValue(sColor)*(1-percent));
        eBlue = (unsigned char)(GetBValue(eColor)*percent) + (unsigned char)(GetBValue(sColor)*(1-percent));
        if (bGamma)
        {
            eRed = (unsigned char)(pow((double)eRed/255.0, gamma) * 255);
            eGreen = (unsigned char)(pow((double)eGreen/255.0, gamma) * 255);
            eBlue = (unsigned char)(pow((double)eBlue/255.0, gamma) * 255);
        }
        colorEnd = RGB(eRed, eGreen, eBlue);

        // Start and end points on the left side
        pointStart.x = lpRect->left + 1;
        pointStart.y = lpRect->bottom-(int)(angle*i)-2;
        if (pointStart.y < lpRect->top+1)
            pointStart.y = lpRect->top+1;
        pointEnd.x = lpRect->left+i+1;
        if (pointEnd.x >= lpRect->right-2)
            pointEnd.x = lpRect->right-2;
        pointEnd.y = lpRect->bottom-2;

        // Left side
        hPen = CreatePen(PS_SOLID, 1, colorStart);
        hOldPen = (HPEN)SelectObject(hDC, hPen);
        MoveToEx(hDC, pointStart.x, pointStart.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        // Filling holes
        MoveToEx(hDC, offsetTop, pointEnd.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        for (j=pointStart.y; j<=offsetLeft; j++)
        {
            sPoint.x = pointStart.x;
            sPoint.y = j;
            ePoint.x = pointEnd.x;
            ePoint.y = pointEnd.y;

            MoveToEx(hDC, sPoint.x, sPoint.y, NULL);
            LineTo(hDC, ePoint.x, ePoint.y);
        }
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
        offsetLeft = pointStart.y;
        offsetTop = pointEnd.x;

        // Start and end points on the right side
        pointStart.x = lpRect->right-i-2;
        if (pointStart.x < lpRect->left+1)
            pointStart.x = lpRect->left+1;
        pointStart.y = lpRect->top+1;
        pointEnd.x = lpRect->right-2;
        pointEnd.y = lpRect->top+(int)(angle*i)+1;
        if (pointEnd.y > lpRect->bottom-2)
            pointEnd.y = lpRect->bottom-2;

        // Right side
        hPen = CreatePen(PS_SOLID, 1, colorEnd);
        hOldPen = (HPEN)SelectObject(hDC, hPen);
        MoveToEx(hDC, pointStart.x, pointStart.y, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y);
        // Filling holes
        MoveToEx(hDC, pointEnd.x, offsetBottom, NULL);
        LineTo(hDC, pointEnd.x, pointEnd.y+1);
        for (j=pointEnd.y; j>=offsetRight; j--)
        {
            sPoint.x = pointStart.x;
            sPoint.y = pointStart.y;
            ePoint.x = pointEnd.x;
            ePoint.y = j;

            MoveToEx(hDC, sPoint.x, sPoint.y, NULL);
            LineTo(hDC, ePoint.x, ePoint.y);
        }
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
        offsetRight = pointEnd.y;
        offsetBottom = pointEnd.y;
    }
}

void CGradient::RadialGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bCircle, BOOL bGamma, double gamma)
{
    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;
    int size = (width < height) ? width : height;

    // Fill background
    HBRUSH hBrush = CreateSolidBrush(sColor);
    RECT fillRect = {lpRect->left+1, lpRect->top+1, lpRect->right-1, lpRect->bottom-1};
    FillRect(hDC, &fillRect, hBrush);
    DeleteObject(hBrush);

    // Draw gradient
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    double percent;
    unsigned char red, green, blue;
    COLORREF color;
    POINT pointStart, pointEnd;
    for (int i=0; i<size/2; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i / (double)(size/2);

        // Gradient color
        red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
            green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
            blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
        }
        color = RGB(red, green, blue);

        if (bCircle)
        {
            // Start and end points
            pointStart.x = (lpRect->left + lpRect->right)/2 - (size/2-i) + 2;
            pointStart.y = (lpRect->top + lpRect->bottom)/2 - (size/2-i) + 2;
            pointEnd.x = (lpRect->left + lpRect->right)/2 + (size/2-i) - 2;
            pointEnd.y = (lpRect->top + lpRect->bottom)/2 + (size/2-i) - 2;
        }
        else
        {
            // Start and end points
            pointStart.x = (lpRect->left + lpRect->right)/2 - (width/2-i) + 2;
            pointStart.y = (lpRect->top + lpRect->bottom)/2 - (height/2-i) + 2;
            pointEnd.x = (lpRect->left + lpRect->right)/2 + (width/2-i) - 2;
            pointEnd.y = (lpRect->top + lpRect->bottom)/2 + (height/2-i) - 2;
        }

        // Gradient
        hPen = CreatePen(PS_SOLID, 2, color);
        hOldPen = (HPEN)SelectObject(hDC, hPen);
        Ellipse(hDC, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
        SelectObject(hDC, hOldPen);
        DeleteObject(hPen);
    }
}

void CGradient::MulticolorHorizontalGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bGamma, double gamma)
{
    if (numColors < 2)
        return;

    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;

    int offset = width / (numColors-1);
    for (int c=1; c<numColors; c++)
    {
        COLORREF sColor = colors[c-1];
        COLORREF eColor = colors[c];

        // Draw gradient
        HBRUSH hBrush = NULL;
        double percent;
        unsigned char red, green, blue;
        COLORREF color;
        RECT rect;
        for (int i=0; i<=offset; i++)
        {
            // Gradient color percent
            percent = 1 - (double)i / (double)(offset);

            // Gradient color
            red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
            green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
            blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
            if (bGamma)
            {
                red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
                green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
                blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
            }
            color = RGB(red, green, blue);

            // Gradient
            rect.left = lpRect->left + (c-1)*offset + i + 1;
            if (rect.left >= lpRect->right-2)
                rect.left = lpRect->right-2;
            rect.top = lpRect->top + 1;
            rect.right = rect.left + 1;
            rect.bottom = lpRect->bottom - 1;
            hBrush = CreateSolidBrush(color);
            FillRect(hDC, &rect, hBrush);
            DeleteObject(hBrush);
        }

        if (offset*(numColors-1) < width)
        {
            rect.left = lpRect->right - (width-offset*(numColors-1));
            rect.top = lpRect->top + 1;
            rect.right = lpRect->right - 1;
            rect.bottom = lpRect->bottom - 1;
            hBrush = CreateSolidBrush(colors[numColors-1]);
            FillRect(hDC, &rect, hBrush);
            DeleteObject(hBrush);
        }
    }
}

void CGradient::MulticolorVerticalGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bGamma, double gamma)
{
    if (numColors < 2)
        return;

    // Gradient params
    //int width = lpRect->right - lpRect->left - 1; //not used
    int height = lpRect->bottom - lpRect->top + 1;

    int offset = height / (numColors-1);
    for (int c=1; c<numColors; c++)
    {
        COLORREF sColor = colors[c-1];
        COLORREF eColor = colors[c];

        int step = (int)(((double)offset/2)/255);
        if ( step == 0 )
            step = 1;

        // Draw gradient
        HBRUSH hBrush = NULL;
        HPEN hPen = NULL;
        HPEN hOldPen = NULL;
        double percent;
        unsigned char red, green, blue;
        COLORREF color;
        RECT rect;
        for (int i=0; i<=offset; i++)
        {
            // Gradient color percent
            percent = 1 - (double)i / (double)(offset);

            // Gradient color
            red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
            green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
            blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
            if (bGamma)
            {
                red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
                green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
                blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
            }
            color = RGB(red, green, blue);

            // Gradient
            rect.left = lpRect->left;
            rect.top = lpRect->top + (c-1)*offset + i;
            if (rect.top >= lpRect->bottom - 1)
                rect.top = lpRect->bottom - 1;
            rect.right = lpRect->right;
            rect.bottom = rect.top + 1;
            hBrush = CreateSolidBrush(color);
            FillRect(hDC, &rect, hBrush);
            DeleteObject(hBrush);
        }

        if (offset*(numColors-1) < height)
        {
            rect.left = lpRect->left;
            rect.top = lpRect->bottom - (height-offset*(numColors-1));
            rect.right = lpRect->right;
            rect.bottom = lpRect->bottom;
            hBrush = CreateSolidBrush(colors[numColors-1]);
            FillRect(hDC, &rect, hBrush);
            DeleteObject(hBrush);
        }
    }
}

void CGradient::MulticolorRadialGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bCircle, BOOL bGamma, double gamma)
{
    if (numColors < 2)
        return;
    else if (numColors == 2)
    {
        RadialGradient(hDC, lpRect, colors[0], colors[1], bCircle, bGamma);
        return;
    }

    // Gradient params
    int width = lpRect->right - lpRect->left - 1;
    int height = lpRect->bottom - lpRect->top - 1;
    int size = (width < height) ? width : height;

    // Fill background
    HBRUSH hBrush = CreateSolidBrush(colors[0]);
    RECT fillRect = {lpRect->left+1, lpRect->top+1, lpRect->right-1, lpRect->bottom-1};
    FillRect(hDC, &fillRect, hBrush);
    DeleteObject(hBrush);

    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, (HBRUSH)GetStockObject(HOLLOW_BRUSH));

    int offset = size / numColors;
    int offset1 = width / numColors;
    int offset2 = height / numColors;
    POINT pointStart, pointEnd;
    HPEN hPen = NULL;
    HPEN hOldPen = NULL;
    for (int c=numColors-1; c>0; c--)
    {
        COLORREF sColor = colors[numColors-c];
        COLORREF eColor = colors[numColors-c-1];

        if (!bCircle)
        {
            // Start and end points
            pointStart.x = (lpRect->left + lpRect->right - c*offset1)/2 + 1;
            pointStart.y = (lpRect->top + lpRect->bottom - c*offset2)/2 + 1;
            pointEnd.x = (lpRect->left + lpRect->right + c*offset1)/2 - 1;
            pointEnd.y = (lpRect->top + lpRect->bottom + c*offset2)/2 - 1;
            hOldPen = (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 1, sColor));
            hOldBrush = (HBRUSH)SelectObject(hDC, CreateSolidBrush(sColor));
            Ellipse(hDC, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
            SelectObject(hDC, hOldBrush);
            SelectObject(hDC, hOldPen);
        }

        // Draw gradient
        double percent;
        unsigned char red, green, blue;
        COLORREF color;
        for (int i=0; i<=offset/2; i++)
        {
            // Gradient color percent
            percent = 1 - (double)i / (double)(offset/2);

            // Gradient color
            red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
            green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
            blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
            if (bGamma)
            {
                red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
                green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
                blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
            }
            color = RGB(red, green, blue);

            if (bCircle)
            {
                // Start and end points
                pointStart.x = (lpRect->left + lpRect->right - (c-1)*offset)/2 - i + 2;
                pointStart.y = (lpRect->top + lpRect->bottom - (c-1)*offset)/2 - i + 2;
                pointEnd.x = (lpRect->left + lpRect->right + (c-1)*offset)/2 + i - 2;
                pointEnd.y = (lpRect->top + lpRect->bottom + (c-1)*offset)/2 + i - 2;
            }
            else
            {
                // Start and end points
                pointStart.x = (lpRect->left + lpRect->right - c*offset1)/2 - i + 2;
                pointStart.y = (lpRect->top + lpRect->bottom - c*offset2)/2 - i + 2;
                pointEnd.x = (lpRect->left + lpRect->right + c*offset1)/2 + i - 2;
                pointEnd.y = (lpRect->top + lpRect->bottom + c*offset2)/2 + i - 2;
            }

            // Gradient
            hPen = CreatePen(PS_SOLID, 2, color);
            hOldPen = (HPEN)SelectObject(hDC, hPen);
            Ellipse(hDC, pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
            SelectObject(hDC, hOldPen);
            DeleteObject(hPen);
        }
    }

    SelectObject(hDC, hOldBrush);
}

void CGradient::HorizontalGradient(HDC hDC, HRGN hRgn, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    RECT rgnRect;
    GetRgnBox(hRgn, &rgnRect);

    // Gradient params
    int width = rgnRect.right - rgnRect.left - 1;
    int height = rgnRect.bottom - rgnRect.top - 1;

    // Draw gradient
    HBRUSH hBrush = NULL;
    double percent;
    unsigned char red, green, blue;
    COLORREF color;
    int startY, endY;
    RECT rect;
    bool foundStart, foundEnd;
    for (int i=0; i<width; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i / (double)(width);

        // Gradient color
        red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
            green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
            blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
        }
        color = RGB(red, green, blue);

        endY = rgnRect.top;
        while (endY<=rgnRect.bottom)
        {
            foundStart = foundEnd = false;

            // Top offset
            startY = endY;
            while (startY<=rgnRect.bottom)
            {
                if ( PtInRegion(hRgn, rgnRect.left+i, startY) )
                {
                    foundStart = true;
                    break;
                }
                else
                    startY++;
            }

            // Bottom offset
            endY = startY;
            while (endY<=rgnRect.bottom)
            {
                if ( !PtInRegion(hRgn, rgnRect.left+i, endY) )
                {
                    foundEnd = true;
                    break;
                }
                else
                    endY++;
            }

            if (foundStart && foundEnd)
            {
                // Gradient rectangle
                rect.left = rgnRect.left + i;
                rect.top = startY;
                rect.right = rect.left + 1;
                rect.bottom = endY;
                hBrush = CreateSolidBrush(color);
                FillRect(hDC, &rect, hBrush);
                DeleteObject(hBrush);
            }
        }
    }
}

void CGradient::VerticalGradient(HDC hDC, HRGN hRgn, COLORREF sColor, COLORREF eColor, BOOL bGamma, double gamma)
{
    RECT rgnRect;
    GetRgnBox(hRgn, &rgnRect);

    // Gradient params
    int width = rgnRect.right - rgnRect.left - 1;
    int height = rgnRect.bottom - rgnRect.top - 1;

    // Draw gradient
    HBRUSH hBrush = NULL;
    double percent;
    unsigned char red, green, blue;
    COLORREF color;
    int startX, endX;
    RECT rect;
    bool foundStart, foundEnd;
    for (int i=0; i<height; i++)
    {
        // Gradient color percent
        percent = 1 - (double)i / (double)(height);

        // Gradient color
        red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
        green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
        blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
        if (bGamma)
        {
            red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
            green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
            blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
        }
        color = RGB(red, green, blue);

        endX = rgnRect.left;
        while (endX<=rgnRect.right)
        {
            foundStart = foundEnd = false;

            // Left offset
            startX = endX;
            while (startX<=rgnRect.right)
            {
                if ( PtInRegion(hRgn, startX, rgnRect.top+i) )
                {
                    foundStart = true;
                    break;
                }
                
                startX++;
            }

            // Right offset
            endX = startX;
            while (endX<=rgnRect.right)
            {
                if ( !PtInRegion(hRgn, endX, rgnRect.top+i) )
                {
                    foundEnd = true;
                    break;
                }
                
                endX++;
            }

            if (foundStart && foundEnd)
            {
                // Gradient rectangle
                rect.left = startX;
                rect.top = rgnRect.top + i;
                rect.right = endX;
                rect.bottom = rect.top + 1;
                hBrush = CreateSolidBrush(color);
                FillRect(hDC, &rect, hBrush);
                DeleteObject(hBrush);
            }
        }
    }
}

void CGradient::MulticolorHorizontalGradient(HDC hDC, HRGN hRgn, COLORREF colors[], int numColors, BOOL bGamma, double gamma)
{
    if (numColors < 2)
        return;

    RECT rgnRect;
    GetRgnBox(hRgn, &rgnRect);

    // Gradient params
    int width = rgnRect.right - rgnRect.left - 1;
    int height = rgnRect.bottom - rgnRect.top - 1;

    int offset = width / (numColors-1);
    for (int c=1; c<numColors; c++)
    {
        COLORREF sColor = colors[c-1];
        COLORREF eColor = colors[c];

        // Draw gradient
        HBRUSH hBrush = NULL;
        double percent;
        unsigned char red, green, blue;
        COLORREF color;
        int startY, endY;
        RECT rect;
        bool foundStart, foundEnd;
        for (int i=0; i<=offset; i++)
        {
            // Gradient color percent
            percent = 1 - (double)i / (double)(offset);

            // Gradient color
            red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
            green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
            blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
            if (bGamma)
            {
                red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
                green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
                blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
            }
            color = RGB(red, green, blue);

            endY = rgnRect.top;
            while (endY<=rgnRect.bottom)
            {
                foundStart = foundEnd = false;

                // Top offset
                startY = endY;
                while (startY<=rgnRect.bottom)
                {
                    if ( PtInRegion(hRgn, rgnRect.left+(c-1)*offset+i, startY) )
                    {
                        foundStart = true;
                        break;
                    }

                    startY++;
                }

                // Bottom offset
                endY = startY;
                while (endY<=rgnRect.bottom)
                {
                    if ( !PtInRegion(hRgn, rgnRect.left+(c-1)*offset+i, endY) )
                    {
                        foundEnd = true;
                        break;
                    }

                    endY++;
                }

                if (foundStart && foundEnd)
                {
                    // Gradient rectangle
                    rect.left = rgnRect.left + (c-1)*offset + i;
                    rect.top = startY;
                    rect.right = rect.left + 1;
                    rect.bottom = endY;
                    hBrush = CreateSolidBrush(color);
                    FillRect(hDC, &rect, hBrush);
                    DeleteObject(hBrush);
                }
            }
        }
    }
}

void CGradient::MulticolorVerticalGradient(HDC hDC, HRGN hRgn, COLORREF colors[], int numColors, BOOL bGamma, double gamma)
{
    if (numColors < 2)
        return;

    RECT rgnRect;
    GetRgnBox(hRgn, &rgnRect);

    // Gradient params
    int width = rgnRect.right - rgnRect.left - 1;
    int height = rgnRect.bottom - rgnRect.top - 1;

    int offset = height / (numColors-1);
    for (int c=1; c<numColors; c++)
    {
        COLORREF sColor = colors[c-1];
        COLORREF eColor = colors[c];

        int step = (int)(((double)offset/2)/255);
        if ( step == 0 )
            step = 1;

        // Draw gradient
        HBRUSH hBrush = NULL;
        HPEN hPen = NULL;
        HPEN hOldPen = NULL;
        double percent;
        unsigned char red, green, blue;
        COLORREF color;
        int startX, endX;
        RECT rect;
        bool foundStart, foundEnd;
        for (int i=0; i<offset; i++)
        {
            // Gradient color percent
            percent = 1 - (double)i / (double)(offset);

            // Gradient color
            red = (unsigned char)(GetRValue(sColor)*percent) + (unsigned char)(GetRValue(eColor)*(1-percent));
            green = (unsigned char)(GetGValue(sColor)*percent) + (unsigned char)(GetGValue(eColor)*(1-percent));
            blue = (unsigned char)(GetBValue(sColor)*percent) + (unsigned char)(GetBValue(eColor)*(1-percent));
            if (bGamma)
            {
                red = (unsigned char)(pow((double)red/255.0, gamma) * 255);
                green = (unsigned char)(pow((double)green/255.0, gamma) * 255);
                blue = (unsigned char)(pow((double)blue/255.0, gamma) * 255);
            }
            color = RGB(red, green, blue);

            endX = rgnRect.left;
            while (endX<=rgnRect.right)
            {
                foundStart = foundEnd = false;

                // Left offset
                startX = endX;
                while (startX<=rgnRect.right)
                {
                    if ( PtInRegion(hRgn, startX, rgnRect.top+(c-1)*offset+i) )
                    {
                        foundStart = true;
                        break;
                    }

                    startX++;
                }

                // Right offset
                endX = startX;
                while (endX<=rgnRect.right)
                {
                    if ( !PtInRegion(hRgn, endX, rgnRect.top+(c-1)*offset+i) )
                    {
                        foundEnd = true;
                        break;
                    }

                    endX++;
                }

                if (foundStart && foundEnd)
                {
                    // Gradient rectangle
                    rect.left = startX;
                    rect.top = rgnRect.top + (c-1)*offset + i;
                    rect.right = endX;
                    rect.bottom = rect.top + 1;
                    hBrush = CreateSolidBrush(color);
                    FillRect(hDC, &rect, hBrush);
                    DeleteObject(hBrush);
                }
            }
        }
    }
}
