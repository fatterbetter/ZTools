#pragma once

#include <string>
#include <map>
using namespace std;

class CZCookie;

class CZCookies;

class CZCookies
{
public:
	CZCookies();
	~CZCookies();

	map<string, CZCookie*> m_mapCookies;

	void Clear();
	string ToJson();
	
	//ʹ��ץ����httpͷ����Ϣ������
	void AddFromHttpHeader(string strHost, string strHeaders);
	//ʹ��documet.cookie��ȡ�õ�����Ϣ������
	void AddFromDocumentCookie(string strUrl, string strDocumentCookie);

	void AddData(string strUrl, string strData);
	void AddKeyValue(string strUrl, string strKey, string strValue);
	void AddCookie(CZCookie* pCookie);
};

class CZCookie
{
public:
	CZCookie(){m_dwFlag = 0;};
	CZCookie(string strUrl, string strData);
	CZCookie(string strUrl, string strKey, string strValue, bool bHttpOnly = false);
	~CZCookie(){};

	string m_strUrl;
	//���ݣ���ʽΪa=b;path=/;domain=.a.com
	string m_strData;
	//��Ҫ������ʶhttponly��cookie
	DWORD m_dwFlag;

	string m_strName;
	string m_strPath;
	string m_strDomain;

	//��ʶ����ʽΪurl;name;path;domain
	string GetId();
};