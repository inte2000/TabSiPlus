// LogFile.cpp: implementation of the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogFile.h"
#include <share.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile(const char *pszLogFileName, BOOL bLogEnabled) : m_bLogEnabled(bLogEnabled)
{
    m_hMutex = ::CreateMutex(NULL, FALSE, _T("45428D53-A5DB-LOGFILE")); 
    m_bLogDateTime = FALSE;
    strcpy(m_szLogFileName,pszLogFileName);
}

CLogFile::CLogFile(BOOL bLogEnabled) : m_bLogEnabled(bLogEnabled)
{
    m_hMutex = ::CreateMutex(NULL, FALSE, _T("45428D53-A5DB-LOGFILE")); 
    m_bLogDateTime = FALSE;
}

CLogFile::~CLogFile()
{
    ::CloseHandle(m_hMutex);
}

void CLogFile::LogMessage(const char *pszMessage)
{
    ::WaitForSingleObject(m_hMutex, INFINITE);
    __try
    {
        if(m_bLogEnabled)
        {
            FILE* pOutFile;
            char  szmessag[384];
            char  szFlag[2];
            strcpy(szFlag,"a");
            pOutFile = _fsopen(m_szLogFileName, szFlag, _SH_DENYNO);

            if(pOutFile != NULL)
            {
                if(m_bLogDateTime)
                {
                    char szTimeStr[128];
                    time_t ltime;
                    time(&ltime);
                    struct tm *now = localtime(&ltime);
                    strftime(szTimeStr,128,"%#c",now);
                    sprintf(szmessag,"%s --> %s\n",szTimeStr,pszMessage);
                }
                else
                {
                    sprintf(szmessag,"%s\n",pszMessage);
                }
                fseek(pOutFile, 0L, SEEK_END);
                fputs(szmessag, pOutFile);            
                fflush(pOutFile);
                fclose(pOutFile);
            } // if
        } // if
    }
    __finally
    {
        ::ReleaseMutex(m_hMutex);
    }
}
