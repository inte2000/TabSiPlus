#include "stdafx.h"
#include "UIResourceName.h"

const char *pResCtrlTextNormal = "CtrlTextNormal";
const char *pResCtrlTextHilight = "CtrlTextHilight";
const char *pResCtrlTextSelected = "CtrlTextSelected";
const char *pResCtrlTextDisabled = "CtrlTextDisabled";

const char *pResCtrlBkgndNormal = "CtrlBkgndNormal";
const char *pResCtrlBkgndHilight = "CtrlBkgndHilight";
const char *pResCtrlBkgndSelected = "CtrlBkgndSelected";
const char *pResCtrlBkgndDisabled = "CtrlBkgndDisabled";

const char *pResMenuTextNormal = "MenuTextNormal";
const char *pResMenuTextHilight = "MenuTextHilight";
const char *pResMenuTextSelected = "MenuTextSelected";
const char *pResMenuTextDisabled = "MenuTextDisabled";

const char *pResMenuBkgndNormal = "MenuBkgndNormal";
const char *pResMenuBkgndHilight = "MenuBkgndHilight";
const char *pResMenuBkgndSelected = "MenuBkgndSelected";
const char *pResMenuBkgndDisabled = "MenuBkgndDisabled";

const char *pResMebuLeftbar = "MebuLeftbar";
const char *pResCtrlRectFrameNormal = "CtrlRectFrameNormal";
const char *pResCtrlRectFrameSelected = "CtrlRectFrameSelected";
const char *pResCtrlRectFrameDisabled = "CtrlRectFrameDisabled";
const char *pResCtrlRectFrameHilight = "CtrlRectFrameHilight";

const char *pRes3DHiLight = "3DHiLight";
const char *pRes3DDarkShadow = "3DDarkShadow";
const char *pResBtnFace = "BtnFace";

const char *pResWndBkgndNormal = "WindowBkgndNormal";
const char *pResWndBkgndHilight = "WindowBkgndHilight";
const char *pResWndBkgndSelected = "WindowBkgndSelected";
const char *pResWndBkgndDisabled = "WindowBkgndDisabled";

const char *pResCheckMarkPen = "CheckMarkPen";

const char *pResDefaultGuiBoldFont = "DefaultGuiBoldFont";


const char *pResDefaultGuiFont = "DefaultGuiFont";
const char *pResNullBrush = "NullBrush";

DefaultColorMap g_DefColor[DefaultColorCount] = {
    { pResCtrlTextNormal, RGB(0,0,255) },
    { pResCtrlTextHilight, RGB(128,64,255) },
    { pResCtrlTextSelected, RGB(255,64,128) },
    { pResCtrlTextDisabled, RGB(128,128,128) },

    { pResCtrlBkgndNormal, RGB(255,255,220) },
    { pResCtrlBkgndHilight, RGB(255,238,194) },
    { pResCtrlBkgndSelected, RGB(255,238,194) },
    { pResCtrlBkgndDisabled, RGB(192,192,192) },

    { pResMenuTextNormal, RGB(0,0,255) },
    { pResMenuTextHilight, RGB(128,64,255) },
    { pResMenuTextSelected, RGB(255,64,128) },
    { pResMenuTextDisabled, RGB(128,128,128) },

    { pResMenuBkgndNormal, RGB(255,255,220) },
    { pResMenuBkgndHilight, RGB(255,238,194) },
    { pResMenuBkgndSelected, RGB(255,238,194) },
    { pResMenuBkgndDisabled, RGB(192,192,192) },

    { pResMebuLeftbar, RGB(215,241,215) },

    { pResCtrlRectFrameNormal, RGB(255,0,128) },
    { pResCtrlRectFrameHilight, RGB(255,0,128) },
    { pResCtrlRectFrameSelected, RGB(255,0,128) },
    { pResCtrlRectFrameDisabled, RGB(200,200,200) },

    { pResWndBkgndNormal, RGB(215,241,215) },
    { pResWndBkgndHilight, RGB(255,238,194) },
    { pResWndBkgndSelected, RGB(255,238,194) },
    { pResWndBkgndDisabled, RGB(192,192,192) }

};

const int SysNameCount = 2;
static const char *g_SystemUiObject[SysNameCount] = {
    pResDefaultGuiFont,pResNullBrush
};

bool IsSystemUiObject(const char *pszResName)
{
    bool bIsSys = false;
    for(int i = 0; i < SysNameCount; i++)
    {
        if(strcmp(g_SystemUiObject[i],pszResName) == 0)
        {
            bIsSys = true;
            break;
        }
    }

    return bIsSys;
}
