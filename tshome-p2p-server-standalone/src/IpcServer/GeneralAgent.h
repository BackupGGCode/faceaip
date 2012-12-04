/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgent.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralAgent.cpp 0001 2012-04-06 09:58:43Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _GENERALAGENT_H
#define _GENERALAGENT_H

#include "CommonInclude.h"
#include <time.h>
#include <vector>
#include "GeneralAgentDef.h" /* default def */

#include "EZThread.h"
#include "ListenSocket.h"

// ��Ӧ��
#include "GeneralAgentHandler.h"
#include "../common/DoubleOutLog.h"

// tcp server
#ifdef USE_GENERALAGENTTCPSOCKETSERVER
#include "GeneralAgentTcpSocketServer.h"
#endif
#ifdef USE_GENERALAGENTUDPSOCKETSERVER
#include "GeneralAgentUdpSocket.h"
#endif
// tcp client, auto conn
#ifdef USE_GENERALAGENTTCPSOCKETCONNECTOR
#include "GeneralAgentTcpSocketConnector.h"
#endif
// http client, auto conn when brocken
#ifdef USE_GENERALAGENTHTTPKEEPCONNECTOR
#include "GeneralAgentHttpKeepConnector.h"
#endif

#ifdef USE_SOCKET_STATUS_AGENT
#include "SocketStatusAgent.h"
#endif
#ifdef USE_GENERALCONSOLE_OVERTCP
#include "GeneralConsoleOverTcp.h"
#endif

typedef struct __generalservercfg_t_tsipc
{
    unsigned short IsUse;
    unsigned short Port;
    std::string strHost;
    // will find by name
    std::string strServerName;
}
GENERALSERVERCFG_T_TSIPC;


typedef struct __GeneralAgentCfg_TSIPC
{
    // TCP����˿�
#ifdef USE_GENERALAGENTTCPSOCKETSERVER
    unsigned short ServicePort; // ��������ã�������Ϊ0
#endif
    // UDP����˿�
#ifdef USE_GENERALAGENTUDPSOCKETSERVER

    unsigned short UdpServicePort; // ��������ã�������Ϊ0
#endif
    // ״̬����˿�
#ifdef USE_SOCKET_STATUS_AGENT

    unsigned short StatusServicePort; // ��������ã�������Ϊ0
#endif
    //���������нӿڣ� 0��Ч
#ifdef USE_GENERALCONSOLE_OVERTCP

    unsigned short ConsoleOverTcpPort; // ���������нӿڣ� 0��Ч
#endif
#ifdef USE_GENERALAGENTTCPSOCKETCONNECTOR
    // tcp ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T_TSIPC> Peer;
#endif

#ifdef USE_GENERALAGENTHTTPKEEPCONNECTOR
    // http ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T_TSIPC> PeerHttp;
#endif

}
GENERALAGENTCFG_T_TSIPC;

#define g_GeneralAgentTSIPC (*CGeneralAgent::instance())

class CGeneralAgent : public CEZThread
{
public:
    PATTERN_SINGLETON_DECLARE(CGeneralAgent);
    CGeneralAgent();
    virtual ~CGeneralAgent();
    ////////

    BOOL Start(GENERALSERVERCFG_T_TSIPC *pConfig=NULL);
    BOOL Stop();
    void ThreadProc();
    void SetDefaultConfig();

    // ���÷���
#ifdef USE_GENERALAGENTTCPSOCKETSERVER

    void SetGeneralAgent(unsigned short  port);
#endif
#ifdef USE_GENERALAGENTUDPSOCKETSERVER

    void SetGeneralUdpAgent(unsigned short  port);
#endif

#ifdef USE_GENERALAGENTTCPSOCKETCONNECTOR

    void AddSubHost(std::string strHost, int nPort, std::string strName="");
#endif //USE_GENERALAGENTTCPSOCKETCONNECTOR
#ifdef USE_GENERALAGENTHTTPKEEPCONNECTOR

    void AddSubHttpHost(std::string strHost, int nPort, std::string strServerName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);
#endif

    void ConnectAll();

    // �ڲ���Ҫһ����ʱ�����д���
    // Ϊȷ���ڲ�������ɣ�һ�����֮�� 10s֮���ٵ���Connect��
    void DisconnectAll();

    void Run();

    Socket * GetSocketByName(std::string strServerName);
    int Send2Socket(std::string strSocketName, const char *pData, size_t Len);


private:
    GENERALAGENTCFG_T_TSIPC m_ConfigGeneralAgent;
    CDoubleOutLog m_Log;
    GeneralAgentHandler *m_pHandlerGeneralAgent; 

#ifdef USE_SOCKET_STATUS_AGENT

    ListenSocket<CSocketStatusAgent> *m_pSocketStatusAgent;
#endif // USE_SOCKET_STATUS_AGENT

#ifdef USE_GENERALCONSOLE_OVERTCP

    ListenSocket<CGeneralConsoleOverTcp> *m_pGeneralConsoleOverTcp;
#endif // USE_GENERALCONSOLE_OVERTCP

#ifdef USE_GENERALAGENTTCPSOCKETSERVER

    ListenSocket<CGeneralAgentTcpSocketServer> *m_pGeneralAgentTcpSocketServer;
#endif // USE_GENERALAGENTTCPSOCKETSERVER

#ifdef USE_GENERALAGENTUDPSOCKETSERVER

    CGeneralAgentUdpSocket *m_pGeneralAgentUdpSocketServer;
#endif
}
; //CGeneralAgent

#endif //_GENERALAGENT_H
