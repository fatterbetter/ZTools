
// testcookie3.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


#include "ZCookie.h"
#include "wininet.h"
#pragma comment(lib,"wininet.lib")

typedef BOOL (__stdcall *Proc_HttpQueryInfoW)(HINTERNET hRequest, DWORD dwInfoLevel,
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

BOOL __stdcall Hook_HttpQueryInfoW(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

// Ctestcookie3App:
// �йش����ʵ�֣������ testcookie3.cpp
//

class Ctestcookie3App : public CWinApp
{
public:
	Ctestcookie3App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctestcookie3App theApp;
extern CZCookies g_cookies;