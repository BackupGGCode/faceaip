/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * AgentWebService.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: AgentWebService.h 5884 2013-01-29 03:21:21Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:21:21  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _AgentWebService_H
#define _AgentWebService_H

#include "EZThread.h"

#include <time.h>
#include <vector>
#include <ListenSocket.h>

// ��Ӧ��
#include "HandlerWebService.h"
#include "../GeneralAgentLog.h"

// 
#include "HttpServerWebService.h"

typedef struct __AgentWebServiceCfg
{
    unsigned short HttpServicePort; // ��������ã�������Ϊ0
}
AgentWebServiceCfg_T;

#define g_AgentWebService (*CAgentWebService::instance())

class CAgentWebService : public CEZThread
{
public:
    PATTERN_SINGLETON_DECLARE(CAgentWebService);
    CAgentWebService();
    virtual ~CAgentWebService();
    ////////

    BOOL Start();
    BOOL Stop();
    void ThreadProc();
    void SetDefaultConfig();

    // ���÷���
    void SetGeneralHttpAgent(unsigned short  port);
    void ConnectAll();

    void ConnectAllSync();

    // �ڲ���Ҫһ����ʱ�����д���
    // ����֮�� 10s֮���ٵ���Connect��
    void DisconnectAll();

    void Run();

    int Send2Socket(std::string strSocketName, const char *pData, size_t Len);
    	
    CHandlerWebService *GetWebServiceHandler();

private:
    AgentWebServiceCfg_T m_ConfigGeneralAgent;
    CHandlerWebService *m_pHandlerWebService;
    CGeneralAgentLog m_Log;

    ListenSocket<CHttpServerWebService> *m_pCHttpServerWebService;

}
; //CAgentWebService
#endif //_AgentWebService_H
