#include "stdafx.h"
#include "Debug.h"
#include "SyncObject.h"
#include "LogFile.h"

static CLogFile *g_pLogFile = NULL;
static int g_nTraceMethod = gnDbgTraceMacro;
static int g_nTraceLevel  = gnDbgLevelDetail;
static int g_nLimitMethod = gnLimitUp;
static BOOL g_nTraceEnable = FALSE;
static BOOL g_bInitialed = FALSE;
static BOOL g_bTimeInfoEnable = FALSE;
static BOOL g_bProcessInfoEnable = FALSE;
CEventEx *g_csDebugTrace = NULL;


BOOL InitDebugTracing(int nLevel, int nMethod, BOOL bEnable, LPCTSTR lpszLogFile)
{
    if(g_bInitialed)
        return TRUE;

    g_nTraceLevel  = nLevel;
    g_nTraceMethod = nMethod;
    g_nTraceEnable = bEnable;
    if(g_nTraceMethod == gnDbgTraceLogFile)
    {
        if(lpszLogFile != NULL)
        {
            WZ_ASSERT(!g_pLogFile);
            char cLogFile[MAX_PATH];
#ifdef UNICODE            
            ::WideCharToMultiByte(CP_ACP,0,lpszLogFile,-1,cLogFile,MAX_PATH,NULL,NULL);
#else
            strcpy(cLogFile,lpszLogFile);
#endif
            g_pLogFile = new CLogFile(cLogFile,g_nTraceEnable);
        }
        else
        {
            g_nTraceMethod = gnDbgTraceMacro;

        }
    }

    g_csDebugTrace = new CEventEx(FALSE, TRUE);
    DebugTracing(gnDbgLevelRun,_T("-------------------------- Output Debug Information --------------------------\n"));
    g_bInitialed = TRUE;
    return TRUE;
}

BOOL UninitDebugTracing()
{
    if(g_nTraceMethod == gnDbgTraceLogFile)
    {
        WZ_ASSERT(g_pLogFile);
        delete g_pLogFile;
        g_pLogFile = NULL;
    }
    else
    {
        WZ_ASSERT(!g_pLogFile);
    }

    delete g_csDebugTrace;
    g_bInitialed = FALSE;
    return TRUE;
}

void EnableTracing(BOOL bEnable)
{
    CEventLock lock(g_csDebugTrace);

    g_nTraceEnable = bEnable;
    if(g_nTraceMethod == gnDbgTraceLogFile)
    {
        WZ_ASSERT(g_pLogFile);
        g_pLogFile->EnableLogMessage(g_nTraceEnable);
    }
}

void SetLimitationMethod(int nLimitMethod)
{
    g_nLimitMethod = nLimitMethod;
}

//现在只有Logfile方式支持时间
void EnableExtraTimeInfo(BOOL bEnable)
{
    CEventLock lock(g_csDebugTrace);

    g_bTimeInfoEnable = bEnable;
    if(g_nTraceMethod == gnDbgTraceLogFile)
    {
        WZ_ASSERT(g_pLogFile);
        g_pLogFile->EnableDateTime(g_bTimeInfoEnable);
    }
}

void EnableExtraProcessInfo(BOOL bEnable)
{
    CEventLock lock(g_csDebugTrace);

    g_bProcessInfoEnable = bEnable;
}

void __cdecl DebugTracing(int nLevel, LPCTSTR fmt, ... )
{
    CEventLock lock(g_csDebugTrace);

    if(g_nTraceEnable)
    {
        bool bLevelEnable = false;
        if((g_nLimitMethod == gnLimitUp) && (nLevel <= g_nTraceLevel))
            bLevelEnable = true;
        else if((g_nLimitMethod == gnLimitDown) && (nLevel >= g_nTraceLevel))
            bLevelEnable = true;
        else
            bLevelEnable = false;

        if(bLevelEnable)
        {
            LPTSTR lpszFinalMsg;
            TCHAR szMessage[512],szTemp[256];
            va_list va;
            va_start(va, fmt);
            wvsprintf(&szTemp[0], fmt, va);
            va_end(va);
            
            lpszFinalMsg = szTemp;
            if(g_bProcessInfoEnable)
            {
                wsprintf(szMessage,_T("(Process = %d, Thread = %d) -- %s"),(int)::GetCurrentProcessId(),(int)::GetCurrentThreadId(),szTemp);
                lpszFinalMsg = szMessage;
            }

            lstrcat(lpszFinalMsg,_T("\n"));

            if(g_nTraceMethod == gnDbgTraceLogFile)
            {
                WZ_ASSERT(g_pLogFile);
#ifdef UNICODE
                char cBuffer[512];
                ::WideCharToMultiByte(CP_ACP,0,lpszFinalMsg,-1,cBuffer,512,NULL,NULL);
                g_pLogFile->LogMessage(cBuffer);
#else
                g_pLogFile->LogMessage(lpszFinalMsg);
#endif
            }
            else if(g_nTraceMethod == gnDbgTraceMacro)
            {
                WZ_TRACE(lpszFinalMsg);
            }
            else
            {
                //nothing
            }
        }
    }
}
