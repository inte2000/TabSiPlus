// Gradient.h: interface for the CGradient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GRADIENT_H)
#define GRADIENT_H


#define PI		3.1415926535897932384626433832795
#define GAMMA	0.4


class CGradient  
{
public:
	// Public methods
	CGradient();
	virtual ~CGradient();
	void HorizontalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void HorizontalGradient(HDC hDC, HRGN hRgn, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void VerticalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void VerticalGradient(HDC hDC, HRGN hRgn, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void ForwardDiagonalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void BackwardDiagonalGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bGamma=FALSE, double gamma=GAMMA);
	void RadialGradient(HDC hDC, LPRECT lpRect, COLORREF sColor, COLORREF eColor, BOOL bCircle, BOOL bGamma=FALSE, double gamma=GAMMA);
	void MulticolorHorizontalGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bGamma=FALSE, double gamma=GAMMA);
	void MulticolorHorizontalGradient(HDC hDC, HRGN hRgn, COLORREF colors[], int numColors, BOOL bGamma=FALSE, double gamma=GAMMA);
	void MulticolorVerticalGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bGamma=FALSE, double gamma=GAMMA);
	void MulticolorVerticalGradient(HDC hDC, HRGN hRgn, COLORREF colors[], int numColors, BOOL bGamma=FALSE, double gamma=GAMMA);
	void MulticolorRadialGradient(HDC hDC, LPRECT lpRect, COLORREF colors[], int numColors, BOOL bCircle, BOOL bGamma=FALSE, double gamma=GAMMA);
};

#endif // !defined(GRADIENT_H)
