
// testcookie4.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Ctestcookie4App:
// �йش����ʵ�֣������ testcookie4.cpp
//

class Ctestcookie4App : public CWinApp
{
public:
	Ctestcookie4App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Ctestcookie4App theApp;