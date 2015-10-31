#ifndef __WZTAB_SUBITEM_H__
#define __WZTAB_SUBITEM_H__


class CTabSubItem
{
public:
    CTabSubItem();
    CSize CalcSubItemSize(CDC *pDC, BOOL bShowIcon);
public:
    CString m_strSubItemName;
    LONG m_lSubItemId;
    int m_nIconIdx;
    int m_nWidth;
    int m_nHeight;
    LPARAM lpData;
};

#endif /*__WZTAB_SUBITEM_H__*/
