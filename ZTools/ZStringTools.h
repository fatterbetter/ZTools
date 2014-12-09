#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

//格式化字符串
std::string ZFormatString(const char *fmt, ...);
//格式化字符串
std::string& ZFormatString(std::string& strResult, const char *fmt, ...);

//字符串大写
std::string& ZUpperString(std::string& str);
//字符串小写
std::string& ZLowerString(std::string& str);

void ZSplitString(const std::string& s, std::string delim, std::vector< std::string >& ret);
void ZTrimString(string& str);
string ZReplaceString(const string& str, const string& src, const string& dest);

bool ZReadFromFile(string& str, const string& strFilePath);
bool ZWriteToFile(const string& str, const string& strFilePath);

std::wstring Zs2ws(const std::string& s);
std::string Zws2s(const std::wstring& ws);

//UTF8转多字节，用完须手动释放pmb
bool ZUTF8ToMB(char*& pmb, const char* pu8);
//多字节转UTF8，用完须手动释放pu8
bool ZMBToUTF8(char*& pu8, const char* pmb);

//UTF8转多字节
std::string& ZUTF8ToMB(std::string& str);
//多字节转UTF8
std::string& ZMBToUTF8(std::string& str);

bool ZCompareExtName(std::string strFileName, std::string strExtName);

//数字转字符串
template <class T> 
std::string ZConvertToString(T value) 
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

//字符串转数字
template <class T> 
void ZConvertFromString(T &value, const std::string &s)
{
	std::stringstream ss(s);
	ss >> value;
}
