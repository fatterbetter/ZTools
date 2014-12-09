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
	
	//使用抓到的http头中信息来设置
	void AddFromHttpHeader(string strHost, string strHeaders);
	//使用documet.cookie中取得到的信息来设置
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
	//数据，格式为a=b;path=/;domain=.a.com
	string m_strData;
	//主要用来标识httponly的cookie
	DWORD m_dwFlag;

	string m_strName;
	string m_strPath;
	string m_strDomain;

	//标识，格式为url;name;path;domain
	string GetId();
};