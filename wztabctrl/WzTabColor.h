#ifndef __WZTABCOLOR_H__
#define __WZTABCOLOR_H__

class CWzTabColor
{
public:
	CWzTabColor();
	~CWzTabColor();
	
	void DrawFrameBoder(CDC *pDC, CRect& rcBorder, BOOL bHover, BOOL bActive);
	void SetFrameBorderColor(COLORREF crNormal, COLORREF crHover, COLORREF crActive);
	void SetBkgndColor(COLORREF crNormal, COLORREF crHover, COLORREF crActive, COLORREF crLastActive);
	void SetBkgndColor(COLORREF crNormal, COLORREF crHover, COLORREF crActive);

public:
	COLORREF m_crFrmBoderNormalLight;
	COLORREF m_crFrmBoderNormalDark;
	COLORREF m_crFrmBoderHoverLight;
	COLORREF m_crFrmBoderHoverDark;
	COLORREF m_crFrmBoderActiveLight;
	COLORREF m_crFrmBoderActiveDark;
	
	COLORREF m_crBkgndHover;
	COLORREF m_crBkgndAvtive;
	COLORREF m_crBkgndLastAvtive;
	COLORREF m_crBkgndNormal;

	COLORREF m_crTextNormal;
	COLORREF m_crTextHover;
	COLORREF m_crTextActive;
	COLORREF m_crTextLastActive;
	COLORREF m_crTextActiveHover;
};

#endif /*__WZTABCOLOR_H__*/