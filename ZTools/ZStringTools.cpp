#include "stdafx.h"

#include "ZStringTools.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

//格式化字符串
std::string ZFormatString(const char *fmt, ...) 
{ 
	std::string strResult="";
	if (NULL != fmt)
	{
		va_list marker = NULL;            
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}            
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
}

//格式化字符串
std::string& ZFormatString(std::string& strResult, const char *fmt, ...) 
{ 
	if (NULL != fmt)
	{
		va_list marker = NULL;            
		va_start(marker, fmt);                            //初始化变量参数
		size_t nLength = _vscprintf(fmt, marker) + 1;    //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0');        //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten>0)
		{
			strResult = &vBuffer[0];
		}            
		va_end(marker);                                    //重置变量参数
	}
	return strResult; 
}

//UTF8编码转为多字节编码 
bool ZUTF8ToMB( char*& pmb, const char* pu8 )
{
	// convert an UTF8 string to widechar   
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(std::bad_alloc &/*memExp*/)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, -1, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  

	// convert an widechar string to Multibyte   
	int MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (MBLen <=0)  
	{  
		return false;  
	}	
	pmb = new char[MBLen+1];
	nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, pmb, MBLen, NULL, NULL);
	delete[] lpszW;  

	if(nRtn != MBLen)  
	{		
		delete[] pmb;
		return false;  
	}  
	return true;  
}

//多字节编码转为UTF8编码
bool ZMBToUTF8( char*& pu8, const char* pmb )
{
	// convert an MBCS string to widechar   
	int nLen = MultiByteToWideChar(CP_ACP, 0, pmb, -1, NULL, 0);  

	WCHAR* lpszW = NULL;  
	try  
	{  
		lpszW = new WCHAR[nLen];  
	}  
	catch(std::bad_alloc &/*memExp*/)  
	{  
		return false;  
	}  

	int nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, -1, lpszW, nLen);  

	if(nRtn != nLen)  
	{  
		delete[] lpszW;  
		return false;  
	}  
	// convert an widechar string to utf8  
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);  
	if (utf8Len <= 0)  
	{  
		return false;  
	}  

	pu8 = new char[utf8Len + 1];

	nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, pu8, utf8Len, NULL, NULL);
	pu8[utf8Len] = '\0';
	delete[] lpszW;  

	if (nRtn != utf8Len)  
	{  
		//pu8.clear();  
		delete[] pu8;
		return false;  
	}  
	return true;
}

//UTF8转多字节
std::string& ZUTF8ToMB(std::string& str)
{
	char* sTemp;
	ZUTF8ToMB(sTemp, str.c_str());
	str = sTemp;
	delete[] sTemp;
	sTemp = NULL;
	return str;
}
//多字节转UTF8
std::string& ZMBToUTF8(std::string& str)
{
	char* sTemp;
	ZMBToUTF8(sTemp, str.c_str());
	str = sTemp;
	delete[] sTemp;
	sTemp = NULL;
	return str;
}

void ZSplitString(const std::string& s, std::string delim, std::vector< std::string >& ret)
{
	size_t last = 0;
	size_t index = s.find(delim, last);
	while (index!=std::string::npos)
	{
		ret.push_back(s.substr(last,index-last));
		last=index+delim.length();
		index=s.find(delim,last);
	}
	if (s.length()-last>0)
	{
		ret.push_back(s.substr(last,s.length()-last));
	}
}

void ZTrimString(string& str)
{
   string::size_type pos = str.find_last_not_of(' ');
   if(pos != string::npos) 
   {
      str.erase(pos + 1);
      pos = str.find_first_not_of(' ');
      if(pos != string::npos) str.erase(0, pos);
   }
   else 
	   str.erase(str.begin(), str.end());
}

string ZReplaceString(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos = str.find(src);
    while (pos != string::npos)
    {
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}

//字符串大写
std::string& ZUpperString(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), std::toupper);
	return str;
}

//字符串小写
std::string& ZLowerString(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	return str;
}

bool ZReadFromFile(string& str, const string& strFilePath)
{
	wstring wstrFilePath = Zs2ws(strFilePath);
	ifstream ifs(wstrFilePath, ios::binary);
	if (!ifs)
	{
		return false;
	}

	stringstream buffer;
	buffer << ifs.rdbuf();
	str = buffer.str();

	ifs.close();
	return true;
}

bool ZWriteToFile(const string& str, const string& strFilePath)
{	
	wstring wstrFilePath = Zs2ws(strFilePath);
	ofstream ofs(wstrFilePath);
	if (!ofs)
		return false;

	ofs << str;
	ofs.close();

	return true;
}

std::string Zws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";

	setlocale(LC_ALL, "chs"); 

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	size_t st;
	wcstombs_s(&st,_Dest,_Dsize,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

std::wstring Zs2ws(const std::string& s)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	size_t st;
	mbstowcs_s(&st,_Dest,_Dsize,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

bool ZCompareExtName(std::string strFileName, std::string strExtName)
{
	ZLowerString(strFileName);
	ZLowerString(strExtName);
	std::string::size_type nPos = strFileName.rfind(strExtName);
	if (nPos == std::string::npos)
	{
		return false;
	}
	std::string::size_type nLengthFileName = strFileName.length();
	std::string::size_type nLengthExtName = strExtName.length();
	if (nPos == nLengthFileName - nLengthExtName)
	{
		return true;
	}
	return false;
}
