// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8659E6B5_4DF1_4F4C_BF70_FF0A41A38CA8__INCLUDED_)
#define AFX_STDAFX_H__8659E6B5_4DF1_4F4C_BF70_FF0A41A38CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WINVER 0x0500 // for multimonitor support

#include <windows.h>
#include <tchar.h>
//#include <stdio.h>

//#include <assert.h>
//#define ASSERT assert
#include <atlbase.h>
#define TRACE    ATLTRACE
#define ASSERT    ATLASSERT

#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))
#endif
/*
#define INT_TO_STR(x) #x
#define INT_TO_STR2(x) INT_TO_STR((x))
#define WARNING __FILE__  INT_TO_STR2(__LINE__) " : warning: "
*/
//#pragma message (WARNING "Sample Text")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8659E6B5_4DF1_4F4C_BF70_FF0A41A38CA8__INCLUDED_)
