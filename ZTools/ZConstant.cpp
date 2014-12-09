#include "stdafx.h"
#include "ZConstant.h"

std::string ZGetCurrentModulePath(std::string strModule /*= "" */)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char exeFullPath[MAX_PATH]; // MAX_PATH
	HMODULE hMod = NULL;
	if (!strModule.empty())
	{
		hMod = GetModuleHandle(strModule.c_str());
	}
	GetModuleFileName(hMod,exeFullPath,MAX_PATH);//得到程序模块名称，全路径
	_splitpath_s( exeFullPath, drive, dir, fname, ext );
	std::string strPath(drive);
	strPath.append(dir);
	return strPath;
}

std::string ZGetCurrentTimeString()
{
	//得到当前时间
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	std::string strTm;
	ZFormatString(strTm
		, "%04d-%02d-%02d %02d:%02d:%02d.%03d"
		, systime.wYear
		, systime.wMonth
		, systime.wDay
		, systime.wHour
		, systime.wMinute
		, systime.wSecond
		, systime.wMilliseconds);
	return strTm;
}

std::string ZGetCurrentTimeString16()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	std::string strTm;
	ZFormatString(strTm
		, "%X%X%X%X%X%X%X"
		, systime.wYear
		, systime.wMonth
		, systime.wDay
		, systime.wHour
		, systime.wMinute
		, systime.wSecond
		, systime.wMilliseconds);
	return strTm;
}

std::string ZGetNewGuidString()
{
	GUID guid;
	CoCreateGuid(&guid);

	std::string strResult;
	ZFormatString(strResult,
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return strResult;
}

std::string ZGetRandomString8()
{
	GUID guid;
	CoCreateGuid(&guid);

	std::string strResult;
	ZFormatString(strResult,
		"%08X",
		guid.Data1);

	return strResult;
}

std::string ZGetLastErrorMessageString(DWORD dwError /*= 0*/)
{
	if (dwError == 0)
	{
		dwError = GetLastError();
	}

	LPTSTR pszErrMSG;
	// Use the default system locale since we look for Windows messages.
	// Note: this MAKELANGID combination has 0 as value
	DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
	// Get the error code's textual description
	BOOL fOk = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_ALLOCATE_BUFFER, 
		NULL, dwError, systemLocale, 
		(LPTSTR) &pszErrMSG, 0, NULL);

	if (!fOk)
	{
		// Is it a network-related error?
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL)
		{
			fOk = FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				hDll, dwError, systemLocale,
				(LPTSTR) &pszErrMSG, 0, NULL);
			FreeLibrary(hDll);
		}
	}

	std::string strMessage(pszErrMSG);
	LocalFree(pszErrMSG);  
	return strMessage;
}
