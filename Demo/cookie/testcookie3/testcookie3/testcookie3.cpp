
// testcookie3.cpp : 定义应用程序的类行为。
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
	//获取HttpQueryInfoW的真实地址
	static const Proc_HttpQueryInfoW RealAddr_HttpQueryInfoW = 
		reinterpret_cast<Proc_HttpQueryInfoW>(::GetProcAddress(GetModuleHandle("WININET.DLL"), "HttpQueryInfoW"));
	//调用真实的HttpQueryInfoW
	BOOL bRet = RealAddr_HttpQueryInfoW(hRequest, dwInfoLevel, lpBuffer, lpdwBufferLength, lpdwIndex);
	//检查响应头的内容
	if (dwInfoLevel == HTTP_QUERY_RAW_HEADERS_CRLF)	//IE总是通过此标志取得整个响应头，从而提取里Set-Cookie:的内容
	{
		CString strHeaders(static_cast<wchar_t*>(lpBuffer));
		//如果有Set-Cookie:才处理，没有的话说明这个响应头不设置Cookie，直接忽略
		if (strHeaders.Find("Set-Cookie:") != -1)
		{
			//现在响应头是有了，但是这个响应头里的Cookie是哪个网站的？
			//所以我们要用hRequest这个句柄，再次调用HttpQueryInfoW查询这个响应头对应的请求头里面的HOST内容，才知道是哪个网站的
			//知道了HOST，才能为B进程调用InternetSetCookieExW设置这个Cookie时提供第一个参数LPCWSTR lpszUrl
			wchar_t *pwszHost = new wchar_t[500];
			DWORD dwBuffSize = sizeof(char) * 500;
			memset(pwszHost, 0 , dwBuffSize);
			if (RealAddr_HttpQueryInfoW(hRequest, HTTP_QUERY_FLAG_REQUEST_HEADERS | HTTP_QUERY_HOST, pwszHost, &dwBuffSize, NULL))
			{
				CString strUrl(pwszHost);
				strUrl = "http://" + strUrl + "/";
				//分析并记录COOKIE信息
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


// Ctestcookie3App 构造

Ctestcookie3App::Ctestcookie3App()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Ctestcookie3App 对象

Ctestcookie3App theApp;


// Ctestcookie3App 初始化

BOOL Ctestcookie3App::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	ZStartPcpService();

	Ctestcookie3Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

