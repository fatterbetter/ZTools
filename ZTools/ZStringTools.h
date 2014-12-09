#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

//��ʽ���ַ���
std::string ZFormatString(const char *fmt, ...);
//��ʽ���ַ���
std::string& ZFormatString(std::string& strResult, const char *fmt, ...);

//�ַ�����д
std::string& ZUpperString(std::string& str);
//�ַ���Сд
std::string& ZLowerString(std::string& str);

void ZSplitString(const std::string& s, std::string delim, std::vector< std::string >& ret);
void ZTrimString(string& str);
string ZReplaceString(const string& str, const string& src, const string& dest);

bool ZReadFromFile(string& str, const string& strFilePath);
bool ZWriteToFile(const string& str, const string& strFilePath);

std::wstring Zs2ws(const std::string& s);
std::string Zws2s(const std::wstring& ws);

//UTF8ת���ֽڣ��������ֶ��ͷ�pmb
bool ZUTF8ToMB(char*& pmb, const char* pu8);
//���ֽ�תUTF8���������ֶ��ͷ�pu8
bool ZMBToUTF8(char*& pu8, const char* pmb);

//UTF8ת���ֽ�
std::string& ZUTF8ToMB(std::string& str);
//���ֽ�תUTF8
std::string& ZMBToUTF8(std::string& str);

bool ZCompareExtName(std::string strFileName, std::string strExtName);

//����ת�ַ���
template <class T> 
std::string ZConvertToString(T value) 
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

//�ַ���ת����
template <class T> 
void ZConvertFromString(T &value, const std::string &s)
{
	std::stringstream ss(s);
	ss >> value;
}
