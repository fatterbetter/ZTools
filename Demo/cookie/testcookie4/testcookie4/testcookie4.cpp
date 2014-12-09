
// testcookie4.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "testcookie4.h"
#include "testcookie4Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <vector>
#include <string>
#include "Zpcp.h"

#include "wininet.h"
#pragma comment(lib,"wininet.lib")

std::vector<std::string> InvokeMethodImpl(const std::string& strMethodName, const std::vector<std::string>& vecParams)
{
	std::vector<std::string> v;
	return v;
}

// Ctestcookie4App

BEGIN_MESSAGE_MAP(Ctestcookie4App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ctestcookie4App ����

Ctestcookie4App::Ctestcookie4App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Ctestcookie4App ����

Ctestcookie4App theApp;


// Ctestcookie4App ��ʼ��

BOOL Ctestcookie4App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	ZInitBeforeInvoke("testcookie3.exe");
	std::string sCookies = ZInvokeMethod("GetCookies");

	Json::Value jCookies;
	Json::Reader jReader;
	if (jReader.parse(sCookies, jCookies))
	{
		if (jCookies.isArray())
		{
			for (UINT i = 0; i < jCookies.size(); i++)
			{
				Json::Value jCookie = jCookies[Json::Value::ArrayIndex(i)];
				std::string strUrl = jCookie["url"].asString();
				std::string strData = jCookie["data"].asString();
				DWORD dwFlag = jCookie["flag"].asUInt();
				InternetSetCookieEx(strUrl.c_str(), NULL, strData.c_str(), dwFlag, NULL);
			}
		}
	}

	Ctestcookie4Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

