
// testcookie3.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


#include "ZCookie.h"
#include "wininet.h"
#pragma comment(lib,"wininet.lib")

typedef BOOL (__stdcall *Proc_HttpQueryInfoW)(HINTERNET hRequest, DWORD dwInfoLevel,
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

BOOL __stdcall Hook_HttpQueryInfoW(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex);

// Ctestcookie3App:
// 有关此类的实现，请参阅 testcookie3.cpp
//

class Ctestcookie3App : public CWinApp
{
public:
	Ctestcookie3App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern Ctestcookie3App theApp;
extern CZCookies g_cookies;