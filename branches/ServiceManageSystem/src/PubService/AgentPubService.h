/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * AgentPubService.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: AgentPubService.h 5884 2013-01-29 03:21:21Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:21:21  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _AgentPubService_H
#define _AgentPubService_H

//#include "CommonInclude.h"
#include "EZThread.h"

#include <time.h>
#include <vector>
#include <ListenSocket.h>

// ��Ӧ��
#include "HandlerPubService.h"
#include "../DoubleOutLog.h"

// 
#include "HttpServerPubService.h"

typedef struct __AgentPubServiceCfg
{
    unsigned short HttpServicePort; // ��������ã�������Ϊ0
}
AgentPubServiceCfg_T;

#define g_AgentPubService (*CAgentPubService::instance())

class CAgentPubService : public CEZThread
{
public:
    PATTERN_SINGLETON_DECLARE(CAgentPubService);
    CAgentPubService();
    virtual ~CAgentPubService();
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
    	
    CHandlerPubService *GetPubServiceHandler();

private:
    AgentPubServiceCfg_T m_ConfigOfPubService;
    CHandlerPubService *m_pHandlerPubService;
    CDoubleOutLog m_Log;

    ListenSocket<CHttpServerPubService> *m_pCHttpServerPubService;

}
; //CAgentPubService
#endif //_AgentPubService_H
