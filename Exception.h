#ifndef EXCEPTION_H
#define EXCEPTION_H

//カスタム例外処理

#include <windows.h>
#include <tchar.h>

class Exception
{
public:
	TCHAR m_pErrorStr[1024];

	Exception(HRESULT hr, TCHAR* pMsg)
	{
		_stprintf_s(m_pErrorStr, _T("***HRESULT : 0x%x [ %s ]\n"), hr, pMsg );

#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugString(m_pErrorStr);
#endif
	}
};

#endif