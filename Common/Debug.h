#ifndef  __GLOBAL_DEBUG_H__
#define __GLOBAL_DEBUG_H__

#ifdef USE_ATLWTL
#define WZ_ASSERT    ATLASSERT
#define WZ_TRACE  ATLTRACE
#else
#define WZ_ASSERT    ASSERT
#define WZ_TRACE  TRACE
#endif

const int gnDbgTraceNone           = 0;
const int gnDbgTraceLogFile        = 1;
const int gnDbgTraceMacro          = 2;

const int gnDbgLevelDetail         = 10;//最高级别，记录全部调试信息
const int gnDbgLevelDebugError     = 7;//记录调试错误信息
const int gnDbgLevelHiDebug        = 6;//记录高级调试信息
const int gnDbgLevelNormalDebug    = 5;//一般调试信息
const int gnDbgLevelRunError       = 3;//运行错误信息
const int gnDbgLevelEnRun          = 2;//加强运行信息
const int gnDbgLevelRun            = 1;//一般运行信息
const int gnDbgLevelNoOutPut       = 0;//一般运行信息

const int gnLimitUp    = 0;
const int gnLimitDown  = 1;


BOOL InitDebugTracing(int nLevel, int nMethod, BOOL bEnable, LPCTSTR lpszLogFile = NULL);
BOOL UninitDebugTracing();
void EnableTracing(BOOL bEnable);
void SetLimitationMethod(int nLimitMethod);
void EnableExtraTimeInfo(BOOL bEnable);
void EnableExtraProcessInfo(BOOL bEnable);

void __cdecl DebugTracing(int nLevel, LPCTSTR fmt, ... );

#endif //__GLOBAL_DEBUG_H__