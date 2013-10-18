/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * AgentWebService.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: AgentWebService.cpp 5884 2013-01-29 03:21:27Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:21:27  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "TypeDefWebService.h"

#include "AgentWebService.h"
#include "../Logs.h"
#ifdef EXAMPLE_EZCONFIG
#include "../Configs/ConfigWebService.h"
#endif //EXAMPLE_EZCONFIG
#ifndef _DEBUG_THIS
//    #define _DEBUG_THIS
#endif
#ifdef _DEBUG_THIS
	#define DEB(x) x
	#define DBG(x) x
#else
	#define DEB(x)
	#define DBG(x)
#endif

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;

PATTERN_SINGLETON_IMPLEMENT(CAgentWebService);
void CAgentWebService::Run()
{
    static int iFirstRun = 0;

    if (1==iFirstRun)
    {
        m_pHandlerWebService->Select(1,0);
        iFirstRun  = 0;
    }

    if (!m_pHandlerWebService->GetCount())
    {
        sleep(1);
        __fline;
        printf("idle.\n");
        return;
    }

    m_pHandlerWebService->Select(1,0);
    m_pHandlerWebService->Update();
}

CAgentWebService::CAgentWebService():CEZThread("CAgentWebService", TP_DEFAULT, 10)
{
    m_ConfigGeneralAgent.HttpServicePort = 9990; // ��������ã�������Ϊ0

    m_pHandlerWebService = new CHandlerWebService(&m_Log);
    assert(m_pHandlerWebService);

    ///////////////////////////////////////////////
}
CAgentWebService::~CAgentWebService()
{
    m_ConfigGeneralAgent.HttpServicePort = 0;
    // handler�����Թ���
    m_pCHttpServerWebService = NULL;
    
    if (m_pHandlerWebService)
    {
        delete m_pHandlerWebService;
    }
}

CHandlerWebService * CAgentWebService::GetWebServiceHandler()
{
	return m_pHandlerWebService;
}

void CAgentWebService::SetGeneralHttpAgent(unsigned short  port)
{
    m_ConfigGeneralAgent.HttpServicePort = port;
}

int CAgentWebService::Send2Socket(std::string strServerName, const char *pData, size_t Len)
{
    Socket *p = m_pHandlerWebService->GetSocketByName(strServerName);

    if (p)
    {
        p->SendBuf(pData, Len);
    }
    else
    {
        __trip;
        printf("Send2Socket %s, Len:%ul failed\n", strServerName.c_str(), Len);
    }

    return 0;
}


void CAgentWebService::ConnectAll()
{
    if (m_ConfigGeneralAgent.HttpServicePort > 0)
    {
        DBG(
        __fline;
        printf("new m_pCHttpServerWebService: %d\n", m_ConfigGeneralAgent.HttpServicePort);
         );

        m_pCHttpServerWebService = new ListenSocket<CHttpServerWebService>(*m_pHandlerWebService);

        if (m_pCHttpServerWebService)
        {
            if (m_pCHttpServerWebService->Bind(m_ConfigGeneralAgent.HttpServicePort) < 0)
            {
                __trip;
                printf("WebService->Bind :%d error\n", m_ConfigGeneralAgent.HttpServicePort);
            }
            else
            {
                m_pHandlerWebService->Add(m_pCHttpServerWebService);
                // DBG(
                //__fline;
                printf("WebService     Ready:%d\n", m_ConfigGeneralAgent.HttpServicePort);
                // );
                LOG4CPLUS_INFO(LOG_WEBSERVICES, "WebService     Ready:"<<m_ConfigGeneralAgent.HttpServicePort);
            }
        }
    }
}

void CAgentWebService::DisconnectAll()
{

}

BOOL CAgentWebService::Start()
{
    // �Ѿ�����
    if (m_bLoop)
    {
        return TRUE;
    }
    // ���Ե�ʱ��ʹ��Ĭ�ϲ���
    SetDefaultConfig();

    ConnectAll();

    int ret = CreateThread();

    ARG_USED(ret);

    return TRUE;
}

BOOL CAgentWebService::Stop()
{
    return TRUE;
}

void CAgentWebService::SetDefaultConfig()
{
#ifdef USE_EZCONFIG
    CConfigWebService __cfg;
    __cfg.update();

    SetGeneralHttpAgent((unsigned short)(__cfg.getConfig().iWebServicePort));
#else
    SetGeneralHttpAgent((unsigned short)(SMARTHOMETELCOMCTRLSERVER_PORT));

#endif
}

void CAgentWebService::ThreadProc()
{
    while (m_bLoop)
    {
        Run();
    } //while (m_bLoop)
}
