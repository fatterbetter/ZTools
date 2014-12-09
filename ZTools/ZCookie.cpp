#include "stdafx.h"

#include "ZCookie.h"
#include "ZStringTools.h"

CZCookies::CZCookies()
{

}

CZCookies::~CZCookies()
{
	Clear();
}

void CZCookies::Clear()
{
	for (map<string, CZCookie*>::iterator it = m_mapCookies.begin(); it != m_mapCookies.end(); ++it)
	{
		delete it->second;
		it->second = NULL;
	}
	m_mapCookies.clear();
}

string CZCookies::ToJson()
{
	string result;
	result.append("[");

	for (map<string, CZCookie*>::iterator it = m_mapCookies.begin(); it != m_mapCookies.end(); ++it)
	{		
		if (it != m_mapCookies.begin())
			result.append(",");

		result.append("{");
		result.append("\"url\":\"");
		for (string::iterator its = it->second->m_strUrl.begin(); its != it->second->m_strUrl.end(); ++its)
		{
			if (*its == '\\')
				result.append("\\\\");
			else if (*its == '\"')
				result.append("\\\"");
			else
				result.append(1, *its);
		}
		result.append("\",\"data\":\"");		
		for (string::iterator its = it->second->m_strData.begin(); its != it->second->m_strData.end(); ++its)
		{
			if (*its == '\\')
				result.append("\\\\");
			else if (*its == '\"')
				result.append("\\\"");
			else
				result.append(1, *its);
		}
		result.append("\",\"flag\":");
		result.append(ZConvertToString<DWORD>(it->second->m_dwFlag));
		
		result.append("}");
	}

	result.append("]");
	return result;
}

void CZCookies::AddFromHttpHeader(string strHost, string strHeaders)
{
	vector<string> vStrHeaders;
	ZSplitString(strHeaders, "\r\n", vStrHeaders);
	for (vector<string>::iterator it = vStrHeaders.begin(); it != vStrHeaders.end(); ++it)
	{		
		string::size_type pos = (*it).find(':');
		if (pos == string::npos)
		{
			continue;
		}
		else
		{
			string strKey = (*it).substr(0, pos);
			ZTrimString(strKey);
			ZLowerString(strKey);
			if (strKey != "set-cookie")
			{
				continue;
			}

			string strValue = (*it).substr(pos + 1);
			ZTrimString(strValue);

			AddData(strHost, strValue);
		}
	}
}

void CZCookies::AddFromDocumentCookie(string strUrl, string strDocumentCookie)
{
	vector<string> vStrPairs;
	ZSplitString(strDocumentCookie, ";", vStrPairs);
	for (vector<string>::iterator it = vStrPairs.begin(); it != vStrPairs.end(); ++it)
	{
		AddData(strUrl, *it);
	}
}

void CZCookies::AddData(string strUrl, string strData)
{
	CZCookie* pCookie = new CZCookie(strUrl, strData);
	AddCookie(pCookie);
}

void CZCookies::AddKeyValue(string strUrl, string strKey, string strValue)
{
	CZCookie* pCookie = new CZCookie(strUrl, strKey, strValue);
	AddCookie(pCookie);
}

void CZCookies::AddCookie(CZCookie* pCookie)
{
	string strId = pCookie->GetId();
	CZCookie* pCookieOld = m_mapCookies[strId];
	if (pCookieOld)
	{
		delete pCookieOld;
		pCookieOld = NULL;
	}
	m_mapCookies[strId] = pCookie;
}

CZCookie::CZCookie(string strUrl, string strData)
{
	m_dwFlag = 0;
	m_strUrl = strUrl;
	m_strData = strData;

	//data需进一步处理
	vector<string> vStrPairs;
	ZSplitString(strData, ";", vStrPairs);
	for (vector<string>::iterator it = vStrPairs.begin(); it != vStrPairs.end(); ++it)
	{
		string::size_type pos = (*it).find('=');
		if (pos == string::npos)
		{
			string strHttp(*it);
			ZTrimString(strHttp);
			ZLowerString(strHttp);
			if (strHttp == "httponly")
			{
				m_dwFlag = 0x00002000;
			}
			continue;
		}
		else
		{
			string strKey = (*it).substr(0, pos);			
			string strValue = (*it).substr(pos + 1);
			ZTrimString(strKey);
			ZTrimString(strValue);
			string strKeyLower = strKey;
			ZLowerString(strKeyLower);
			if (strKeyLower == "path")
			{
				m_strPath = strValue;
			}
			else if (strKeyLower == "domain")
			{
				m_strDomain = strValue;
			}
			else if (strKeyLower == "expires")
			{
				continue;
			}
			else if (strKeyLower == "")
			{
				continue;
			}
			else
			{
				if (m_strName == "")
				{
					m_strName = strKey;
				}
			}
		}
	}		
}

CZCookie::CZCookie(string strUrl, string strKey, string strValue, bool bHttpOnly/* = false*/)
{
	m_dwFlag = 0;
	m_strUrl = strUrl;
	m_strName = strKey;
	ZFormatString(m_strData, "%s=%s; Path=/", strKey.c_str(), strValue.c_str());
	m_strPath = "/";
	
	//#define INTERNET_COOKIE_HTTPONLY        0x00002000
	if (bHttpOnly)
	{
		m_dwFlag = 0x00002000;
	}
	else
	{
		m_dwFlag = 0x00000000;
	}
}

string CZCookie::GetId()
{
	return ZFormatString("%s;%s;%s;%s", 
		m_strUrl.c_str(),
		m_strName.c_str(),
		m_strPath.c_str(),
		m_strDomain.c_str());
}