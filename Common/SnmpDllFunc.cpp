/*********************************************************************************
*  Author         : orbit                                                        *
*  Description    : 这个文件定义CSnmpFunc类，将对SNMP的动态调用封装成类，这个    *
*                   类是Winmsg进程查看软件的一个模块                             *
*  Date           : Fab 19, 2004                                                 *
*  Last-modified  : May 11, 2004                                                 *
*  Contact us     : inte2000@163.com,support@winmsg.com                          *
*  Web Page       : http://www.winmsg.com/cn/orbit.htm                           *
*                   http://blog.csdn.net/orbit/          (关于代码的更新)        *
*********************************************************************************/
#include "stdafx.h"
#include "SnmpDllFunc.h"

LPCTSTR CSnmpFunc::m_lpszSnmpDll = _T("inetmib1.dll");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSnmpFunc::CSnmpFunc()
{
	m_bLoad = FALSE;
	m_hModule = NULL;
	m_pfSnmpExtensionInit = NULL;
	m_pfSnmpExtensionTrap = NULL;
	m_pfSnmpExtensionQuery = NULL;
	m_pfSnmpExtensionInitEx = NULL;
}

CSnmpFunc::~CSnmpFunc()
{
	FreeFunctions();
}

BOOL CSnmpFunc::LoadFunctions()
{
	WZ_ASSERT(m_hModule == NULL);
	m_hModule = ::GetModuleHandle(m_lpszSnmpDll);
	if(m_hModule == NULL)
	{
		m_hModule = ::LoadLibrary(m_lpszSnmpDll);
		m_bLoad = TRUE;
	}

	if(m_hModule == NULL) //still NULL here? Maybe a 9x OS
	{
		m_bLoad = FALSE;
		return FALSE;
	}

	m_pfSnmpExtensionInit = (PFUNC_SnmpExtensionInit)::GetProcAddress(m_hModule,_T("SnmpExtensionInit"));
	m_pfSnmpExtensionTrap = (PFUNC_SnmpExtensionTrap)::GetProcAddress(m_hModule,_T("SnmpExtensionTrap"));
	m_pfSnmpExtensionQuery = (PFUNC_SnmpExtensionQuery)::GetProcAddress(m_hModule,_T("SnmpExtensionQuery"));
	m_pfSnmpExtensionInitEx = (PFUNC_SnmpExtensionInitEx)::GetProcAddress(m_hModule,_T("SnmpExtensionInitEx"));

	if( m_pfSnmpExtensionInit == NULL 
		|| m_pfSnmpExtensionTrap == NULL 
		|| m_pfSnmpExtensionQuery == NULL 
		|| m_pfSnmpExtensionInitEx == NULL )
	{
		FreeFunctions();
		return FALSE;
	}

	return TRUE;
}

BOOL CSnmpFunc::FreeFunctions()
{
	if(m_hModule != NULL)
	{
		if(m_bLoad)
			::FreeLibrary(m_hModule);

		m_hModule = NULL;
	}

	m_pfSnmpExtensionInit = NULL;
	m_pfSnmpExtensionTrap = NULL;
	m_pfSnmpExtensionQuery = NULL;
	m_pfSnmpExtensionInitEx = NULL;

	return TRUE;
}

BOOL CSnmpFunc::SnmpExtensionInit(DWORD dwTimeZeroReference,HANDLE * hPollForTrapEvent,AsnObjectIdentifier * supportedView)
{
	WZ_ASSERT(m_pfSnmpExtensionInit != NULL);
	if(m_pfSnmpExtensionInit != NULL)
		return m_pfSnmpExtensionInit(dwTimeZeroReference,hPollForTrapEvent,supportedView);
	else
		return FALSE;
}

BOOL CSnmpFunc::SnmpExtensionTrap(AsnObjectIdentifier * enterprise,AsnInteger * genericTrap,AsnInteger * specificTrap,AsnTimeticks * timeStamp,RFC1157VarBindList * variableBindings)
{
	WZ_ASSERT(m_pfSnmpExtensionTrap != NULL);
	if(m_pfSnmpExtensionTrap != NULL)
		return m_pfSnmpExtensionTrap(enterprise,genericTrap,specificTrap,timeStamp,variableBindings);
	else
		return FALSE;
}

BOOL CSnmpFunc::SnmpExtensionQuery(BYTE requestType,RFC1157VarBindList * variableBindings,AsnInteger * errorStatus,AsnInteger * errorIndex)
{
	WZ_ASSERT(m_pfSnmpExtensionQuery != NULL);
	if(m_pfSnmpExtensionQuery != NULL)
		return m_pfSnmpExtensionQuery(requestType,variableBindings,errorStatus,errorIndex);
	else
		return FALSE;
}

BOOL CSnmpFunc::SnmpExtensionInitEx(AsnObjectIdentifier * supportedView)
{
	WZ_ASSERT(m_pfSnmpExtensionInitEx != NULL);
	if(m_pfSnmpExtensionInitEx != NULL)
		return m_pfSnmpExtensionInitEx(supportedView);
	else
		return FALSE;
}
