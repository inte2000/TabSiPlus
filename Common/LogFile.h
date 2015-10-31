// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGFILE_H__2259D14C_2509_4091_9AF1_2E7E64668877__INCLUDED_)
#define AFX_LOGFILE_H__2259D14C_2509_4091_9AF1_2E7E64668877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLogFile
{
public:
    CLogFile(const char *pszLogFileName, BOOL bLogEnabled);
    CLogFile(BOOL bLogEnabled);
    virtual ~CLogFile();

    void InitializeFileName(const char *pszLogFileName);
    void EnableLogMessage(BOOL bEnable);
    void EnableDateTime(BOOL bEnable);

    void LogMessage(const char *pszMessage);
private:
    BOOL    m_bLogDateTime;
    BOOL    m_bLogEnabled;
    char    m_szLogFileName[MAX_PATH];
    HANDLE  m_hMutex;
};

inline void CLogFile::InitializeFileName(const char *pszLogFileName)
{ strcpy(m_szLogFileName,pszLogFileName); }

inline void CLogFile::EnableLogMessage(BOOL bEnable)
{ m_bLogEnabled = bEnable; }

inline void CLogFile::EnableDateTime(BOOL bEnable)
{ m_bLogDateTime = bEnable; }

#endif // !defined(AFX_LOGFILE_H__2259D14C_2509_4091_9AF1_2E7E64668877__INCLUDED_)
