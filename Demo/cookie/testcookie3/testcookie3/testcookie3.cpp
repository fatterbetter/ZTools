
// testcookie3.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "testcookie3.h"
#include "testcookie3Dlg.h"
#include "APIHook.h"
#include "Zpcp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CZCookies g_cookies;

CAPIHook sm_HttpQueryInfoW("WININET.DLL", "HttpQueryInfoW", reinterpret_cast<PROC>(Hook_HttpQueryInfoW));

BOOL __stdcall Hook_HttpQueryInfoW(HINTERNET hRequest, DWORD dwInfoLevel, 
	LPVOID lpBuffer, LPDWORD lpdwBufferLength, LPDWORD lpdwIndex)
{
	//��ȡHttpQueryInfoW����ʵ��ַ
	static const Proc_HttpQueryInfoW RealAddr_HttpQueryInfoW = 
		reinterpret_cast<Proc_HttpQueryInfoW>(::GetProcAddress(GetModuleHandle("WININET.DLL"), "HttpQueryInfoW"));
	//������ʵ��HttpQueryInfoW
	BOOL bRet = RealAddr_HttpQueryInfoW(hRequest, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	//�����Ӧͷ������
	if (dwInfoLevel == HTTP_QUERY_RAW_HEADERS_CRLF)	//IE����ͨ���˱�־ȡ��������Ӧͷ���Ӷ���ȡ��Set-Cookie:������
	{
		CString strHeaders(static_cast<wchar_t*>(lpBuffer));
		//�����Set-Cookie:�Ŵ���û�еĻ�˵�������Ӧͷ������Cookie��ֱ�Ӻ���
		if (strHeaders.Find("Set-Cookie:") != -1)
		{
			//������Ӧͷ�����ˣ����������Ӧͷ���Cookie���ĸ���վ�ģ�
			//��������Ҫ��hRequest���������ٴε���HttpQueryInfoW��ѯ�����Ӧͷ��Ӧ������ͷ�����HOST���ݣ���֪�����ĸ���վ��
			//֪����HOST������ΪB���̵���InternetSetCookieExW�������Cookieʱ�ṩ��һ������LPCWSTR lpszUrl
			wchar_t *pwszHost = new wchar_t[500];
			DWORD dwBuffSize = sizeof(char) * 500;
			memset(pwszHost, 0 , dwBuffSize);
			if (RealAddr_HttpQueryInfoW(hRequest, HTTP_QUERY_FLAG_REQUEST_HEADERS | HTTP_QUERY_HOST, pwszHost, &dwBuffSize, NULL))
			{
				CString strUrl(pwszHost);
				strUrl = "http://" + strUrl + "/";
				//��������¼COOKIE��Ϣ
				g_cookies.AddFromHttpHeader((LPCTSTR)strUrl, (LPCTSTR)strHeaders);
			}
			delete [] pwszHost;
			pwszHost = NULL;
		}
	}
	return bRet;
}

std::vector<std::string> InvokeMethodImpl(const std::string& strMethodName, const std::vector<std::string>& vecParams)
{
	std::vector<std::string> vecResult;

	if (strMethodName == "GetCookies")
	{
		vecResult.push_back(g_cookies.ToJson());
	}

	return vecResult;
}

// Ctestcookie3App

BEGIN_MESSAGE_MAP(Ctestcookie3App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Ctestcookie3App ����

Ctestcookie3App::Ctestcookie3App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� Ctestcookie3App ����

Ctestcookie3App theApp;


// Ctestcookie3App ��ʼ��

BOOL Ctestcookie3App::InitInstance()
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

	ZStartPcpService();

	Ctestcookie3Dlg dlg;
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

