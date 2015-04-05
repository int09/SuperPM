#include "StdAfx.h"
#include "Common.h"
#include <tchar.h>
#include <strsafe.h>



void CFormatMessage::FormatMessage()
{
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		m_nLastError, //接收错误码
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //默认语言
		(LPTSTR)&szErrorText, //接收错误信息
		0,
		NULL);
}


VOID CFormatMessage::OutputDebugString(LPCTSTR szFormat,...)
{
	va_list vagrc;
	va_start(vagrc,szFormat);
	TCHAR szParamText[2048]={NULL};
	StringCbVPrintf(szParamText,sizeof(szParamText),szFormat,vagrc);
	va_end(vagrc);
	INT nLen = _tcslen(szParamText);
	StringCbPrintf(&szParamText[nLen],sizeof(szParamText)-nLen,TEXT("\r\n错误原因:(%d)%s"),m_nLastError,szErrorText);
	::OutputDebugString(szParamText);
}