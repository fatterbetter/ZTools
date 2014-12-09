#pragma once

//////////////////////////////////////////////////////////////////////////
// ���̼�ͨ��(ICE)
//
// ʹ��ʾ��:
//
// ���񷽵���ZStartPcpService("Լ����")
// ������ʵ��InvokeMethodImpl����
//
// �ͻ����ȵ���ZInitBeforeInvoke("Լ����")
// Ȼ�����ZInvokeMethod("������", "�����б��vector")
//////////////////////////////////////////////////////////////////////////

#include "pcp.h"
#include "csharedmemory.h"

class CIPcpI;

const std::string gc_strPreSmName("SM_ZPCP_");
const int gc_nSmSize = 256;

bool ZStartPcpService(const std::string& strSmName = "", const std::string& strPort = "0");
void ZInitBeforeInvoke(const std::string& strSmName);
std::vector<std::string> ZInvokeMethod(const ::std::string& strMethodName, const ::std::vector<::std::string>& vecParams);
std::string ZInvokeMethod(const ::std::string& strMethodName);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4, const ::std::string& strParam5);
std::string ZInvokeMethod(const ::std::string& strMethodName, const ::std::string& strParam1, const ::std::string& strParam2, const ::std::string& strParam3, const ::std::string& strParam4, const ::std::string& strParam5, const ::std::string& strParam6);

class CZPcp
{
public:
	CZPcp();
	~CZPcp();

public:
	bool StartPcpService(const std::string& strSmName = "", const std::string& strPort = "0");
	void Clear();
	void InitBeforeInvoke(const std::string& strSmName);
	std::vector<std::string> InvokeMethod(const ::std::string& strMethodName, const ::std::vector<::std::string>& vecParams);

private:
	Ice::CommunicatorPtr m_pCommunicator;
	CSharedMemory* m_pSm;
	std::string m_strCallSmAllName;
};

class CIPcpI : public MPcp::IPcp
{
public:
	CIPcpI(void);
	~CIPcpI(void);

	virtual ::MPcp::VectorString InvokeMethod(const ::std::string&, const ::MPcp::VectorString&, const ::Ice::Current& = ::Ice::Current() );

};

extern CZPcp g_ZPcp;