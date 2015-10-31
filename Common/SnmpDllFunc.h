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
#if !defined(AFX_SNMPFUNC_H__9AF63BB3_8A14_4DFE_B2B0_5E8C8159B98E__INCLUDED_)
#define AFX_SNMPFUNC_H__9AF63BB3_8A14_4DFE_B2B0_5E8C8159B98E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <snmp.h>


typedef BOOL (WINAPI * PFUNC_SnmpExtensionInit)(DWORD dwTimeZeroReference,HANDLE * hPollForTrapEvent,AsnObjectIdentifier * supportedView); 
typedef BOOL (WINAPI * PFUNC_SnmpExtensionTrap)(AsnObjectIdentifier * enterprise,AsnInteger * genericTrap,AsnInteger * specificTrap,AsnTimeticks * timeStamp,RFC1157VarBindList * variableBindings); 
typedef BOOL (WINAPI * PFUNC_SnmpExtensionQuery)(BYTE requestType,RFC1157VarBindList * variableBindings,AsnInteger * errorStatus,AsnInteger * errorIndex); 
typedef BOOL (WINAPI * PFUNC_SnmpExtensionInitEx)(AsnObjectIdentifier * supportedView); 

class CSnmpFunc  
{
public:
	CSnmpFunc();
	virtual ~CSnmpFunc();
	
	BOOL LoadFunctions();
	BOOL FreeFunctions();

	BOOL SnmpExtensionInit(DWORD dwTimeZeroReference,HANDLE * hPollForTrapEvent,AsnObjectIdentifier * supportedView); 
	BOOL SnmpExtensionTrap(AsnObjectIdentifier * enterprise,AsnInteger * genericTrap,AsnInteger * specificTrap,AsnTimeticks * timeStamp,RFC1157VarBindList * variableBindings); 
	BOOL SnmpExtensionQuery(BYTE requestType,RFC1157VarBindList * variableBindings,AsnInteger * errorStatus,AsnInteger * errorIndex); 
	BOOL SnmpExtensionInitEx(AsnObjectIdentifier * supportedView); 

public:
	static LPCTSTR m_lpszSnmpDll;
protected:
	PFUNC_SnmpExtensionInit m_pfSnmpExtensionInit;
	PFUNC_SnmpExtensionTrap m_pfSnmpExtensionTrap;
	PFUNC_SnmpExtensionQuery m_pfSnmpExtensionQuery;
	PFUNC_SnmpExtensionInitEx m_pfSnmpExtensionInitEx;


	BOOL m_bLoad;
	HMODULE m_hModule;
};

#endif // !defined(AFX_SNMPFUNC_H__9AF63BB3_8A14_4DFE_B2B0_5E8C8159B98E__INCLUDED_)
