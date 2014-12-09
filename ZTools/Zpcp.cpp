#include "stdafx.h"

#include "Zpcp.h"
#include <Ice/Ice.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include <cctype>

CZPcp g_ZPcp;

bool ZStartPcpService(const std::string& strSmName/* = ""*/, const std::string& strPort/* = "0"*/)
{
	return g_ZPcp.StartPcpService(strSmName, strPort);
}

void ZInitBeforeInvoke(const std::string& strSmName)
{
	return g_ZPcp.InitBeforeInvoke(strSmName);
}

std::vector<std::string> ZInvokeMethod(const ::std::string& strMethodName, const ::std::vector<::std::string>& vecParams)
{
	return g_ZPcp.InvokeMethod(strMethodName, vecParams);
}

std::string ZInvokeMethod(const ::std::string& strMethodName)
{
	std::vector<std::string> vecResult, vecParams;
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecParams.push_back(strParam2);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecParams.push_back(strParam2);
	vecParams.push_back(strParam3);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecParams.push_back(strParam2);
	vecParams.push_back(strParam3);
	vecParams.push_back(strParam4);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4, const ::std::string& strParam5)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecParams.push_back(strParam2);
	vecParams.push_back(strParam3);
	vecParams.push_back(strParam4);
	vecParams.push_back(strParam5);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4, const ::std::string& strParam5, const ::std::string& strParam6)
{
	std::vector<std::string> vecResult, vecParams;
	vecParams.push_back(strParam1);
	vecParams.push_back(strParam2);
	vecParams.push_back(strParam3);
	vecParams.push_back(strParam4);
	vecParams.push_back(strParam5);
	vecParams.push_back(strParam6);
	vecResult = g_ZPcp.InvokeMethod(strMethodName, vecParams);
	if (vecResult.size() == 0)
	{
		return "";
	}
	else
	{
		return vecResult[0];
	}
}

CZPcp::CZPcp()
{
	m_pCommunicator = NULL;
	m_pSm = NULL;
	m_strCallSmAllName.clear();
}

CZPcp::~CZPcp()
{
	Clear();
}

bool CZPcp::StartPcpService(const std::string& strSmName/* = ""*/, const std::string& strPort/* = "0"*/)
{
	if (m_pSm != NULL)
	{
		return false;
	}

	std::string strSmAllName;

	if (strSmName.empty())
	{
		char a[512] = {0};
		GetModuleBaseName(GetCurrentProcess(), NULL, a, 512);
		strSmAllName = gc_strPreSmName + a;
	}
	else
	{
		strSmAllName = gc_strPreSmName + strSmName;
	}

	std::transform(strSmAllName.begin(), strSmAllName.end(), strSmAllName.begin(), std::toupper);
	m_pSm = new CSharedMemory(strSmAllName.c_str(), gc_nSmSize);
	if (m_pSm->AlreadyExist())
	{
		delete m_pSm;
		m_pSm = NULL;
		return false;
	}

	if (m_pCommunicator == NULL)
	{
		m_pCommunicator = Ice::initialize();
	}

	std::string strEndPoint("");

	try
	{
		Ice::ObjectAdapterPtr adapter = m_pCommunicator->createObjectAdapterWithEndpoints("pcpAdapter", "tcp -h 127.0.0.1 -p " + strPort);
		Ice::ObjectPtr servant = new CIPcpI();
		adapter->add(servant, m_pCommunicator->stringToIdentity("pcp"));
		adapter->activate();
	
		Ice::EndpointSeq es = adapter->getEndpoints();
		strEndPoint = es[0]->toString();
		strEndPoint = "pcp:" + strEndPoint;
		strEndPoint.resize(strEndPoint.size() + 2);
	}
	catch(const Ice::Exception&/* ex*/)
	{
		delete m_pSm;
		m_pSm = NULL;
		return false;
	}

	{
		CSharedMemory::Locker locker(m_pSm);
		memcpy((void*)m_pSm->GetData(), strEndPoint.c_str(), strEndPoint.size());
	}

	return true;
}

void CZPcp::InitBeforeInvoke(const std::string& strSmName)
{
	if (strSmName.empty())
	{
		m_strCallSmAllName.clear();
		return;
	}

	m_strCallSmAllName = gc_strPreSmName + strSmName;
	std::transform(m_strCallSmAllName.begin(), m_strCallSmAllName.end(), m_strCallSmAllName.begin(), std::toupper);
}

std::vector<std::string> CZPcp::InvokeMethod(const ::std::string& strMethodName, const ::std::vector<::std::string>& vecParams)
{
	std::vector<std::string> vecEmpty;

	if (m_pCommunicator == NULL)
	{
		m_pCommunicator = Ice::initialize();
	}

	if (m_strCallSmAllName.empty())
	{
		return vecEmpty;
	}

	std::string strEndPoint;

	CSharedMemory* pSm = new CSharedMemory(m_strCallSmAllName.c_str(), gc_nSmSize);
	if (!pSm->AlreadyExist())
	{
		delete pSm;
		pSm = NULL;
		return vecEmpty;
	}
	else
	{
		CSharedMemory::Locker locker(pSm);
		strEndPoint = (char*)pSm->GetData();
	}
	delete pSm;
	pSm = NULL;

	if (strEndPoint.empty())
	{
		return vecEmpty;
	}

	try
	{
		MPcp::IPcpPrx pcpPrx = MPcp::IPcpPrx::checkedCast(m_pCommunicator->stringToProxy(strEndPoint));
		pcpPrx = pcpPrx->ice_twoway()->ice_timeout(20000)->ice_secure(false);
		vecEmpty = pcpPrx->InvokeMethod(strMethodName, vecParams);
	}
	catch(const Ice::Exception&/* ex*/)
	{
		return vecEmpty;
	}

	return vecEmpty;
}

void CZPcp::Clear()
{
	if (m_pSm)
	{
		delete m_pSm;
		m_pSm = NULL;
	}

	if (m_pCommunicator)
	{
#if !defined(ICE_STATIC_LIBS) || !defined(_DEBUG)
		//静态库且DEBUG模式下，这句代码会有断言错误，所以暂时屏掉，屏掉的情况下会有内存泄露
		m_pCommunicator->destroy();
#endif
		m_pCommunicator = NULL;
	}
}

CIPcpI::CIPcpI(void)
{
}

CIPcpI::~CIPcpI(void)
{
}

extern std::vector<std::string> InvokeMethodImpl(const std::string& strMethodName, const std::vector<std::string>& vecParams);

::MPcp::VectorString CIPcpI::InvokeMethod(const ::std::string& strMethodName, const ::MPcp::VectorString& vecParams, const ::Ice::Current& /*= ::Ice::Current() */)
{
	return InvokeMethodImpl(strMethodName, vecParams);
}
