#include "StdAfx.h"
#include "Common.h"
#include <tchar.h>
#include <strsafe.h>



void CFormatMessage::FormatMessage()
{
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		m_nLastError, //���մ�����
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //Ĭ������
		(LPTSTR)&szErrorText, //���մ�����Ϣ
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
	StringCbPrintf(&szParamText[nLen],sizeof(szParamText)-nLen,TEXT("\r\n����ԭ��:(%d)%s"),m_nLastError,szErrorText);
	::OutputDebugString(szParamText);
}