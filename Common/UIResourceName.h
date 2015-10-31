#ifndef __UIRESOURCENAME_H__
#define __UIRESOURCENAME_H__

typedef struct tagDefaultColorMap
{
    const char *pResName;
    COLORREF Color;
}DefaultColorMap;

extern const char *pResCtrlTextNormal;
extern const char *pResCtrlTextHilight;
extern const char *pResCtrlTextSelected;
extern const char *pResCtrlTextDisabled;

extern const char *pResCtrlBkgndNormal;
extern const char *pResCtrlBkgndHilight;
extern const char *pResCtrlBkgndSelected;
extern const char *pResCtrlBkgndDisabled;

extern const char *pResMenuTextNormal;
extern const char *pResMenuTextHilight;
extern const char *pResMenuTextSelected;
extern const char *pResMenuTextDisabled;

extern const char *pResMenuBkgndNormal;
extern const char *pResMenuBkgndHilight;
extern const char *pResMenuBkgndSelected;
extern const char *pResMenuBkgndDisabled;

extern const char *pResMebuLeftbar;
extern const char *pResCtrlRectFrameNormal;
extern const char *pResCtrlRectFrameHilight;
extern const char *pResCtrlRectFrameDisabled;
extern const char *pResCtrlRectFrameSelected;

extern const char *pRes3DHiLight;
extern const char *pRes3DDarkShadow;
extern const char *pResBtnFace;

extern const char *pResWndBkgndNormal;
extern const char *pResWndBkgndHilight;
extern const char *pResWndBkgndSelected;
extern const char *pResWndBkgndDisabled;

extern const char *pResCheckMarkPen;

extern const char *pResDefaultGuiBoldFont;

extern const char *pResDefaultGuiFont;
extern const char *pResNullBrush;

const int DefaultColorCount = 25;
extern DefaultColorMap g_DefColor[DefaultColorCount];

bool IsSystemUiObject(const char *pszResName);

#endif