//////////////////////////////////////////////////////////////
// File:        // Localization.h
// File time:    // 12.04.2005    14:31
// Version:     // 1.0
// Author:        // Sergey Kolomenkin <register@tut.by>
// Copyright:    // (c) Sergey Kolomenkin, Minsk, 2005
// Description: //////////////////////////////////////////////
//   Some useful routines...

//////////////////////////////////////////////////////////////
// Version:     // 1.1
// Time:        // 21.11.2007   
// Author:      // Simon Wang <inte2000@163.com>
// Description: // According to the need to adjust some of the code, add some function

#ifndef _LOCALIZATION_H_UID00000019FC5A4D2D
#define _LOCALIZATION_H_UID00000019FC5A4D2D

#define NATIVE_LANGUAGE

void SetThreadNativeLanguage();

void SetLocResourceInstHandle(HINSTANCE hLocResInst);

BOOL    LocLoadString        ( UINT id, LPTSTR pString, IN OUT LPDWORD lpdwSize );

#ifndef TABSIHOST_EXE

HMENU    LocLoadMenu            ( LPCTSTR id );
CString    LocLoadString        ( LPCTSTR id );
HICON    LocLoadIcon            ( LPCTSTR id, int cx = 0, int cy = 0 );
HACCEL  LocLoadAccelerators    ( LPCTSTR id );
LPCDLGTEMPLATE LocLoadDialog( LPCTSTR id );

HMENU    LocLoadMenu            ( UINT id );
CString    LocLoadString        ( UINT id );
HICON    LocLoadIcon            ( UINT id, int cx = 0, int cy = 0 );
HACCEL  LocLoadAccelerators    ( UINT id );
LPCDLGTEMPLATE LocLoadDialog( UINT id );

#endif //ifdef TABSIHOST_EXE

#endif //ifndef _LOCALIZATION_H_UID00000019FC5A4D2D
