/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgent.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
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

#include <time.h>
#include <vector>
#include "GeneralAgentDef.h" /* default def */

#include "ListenSocket.h"

#include "EZThread.h"
#include "EZTimer.h"

// ��Ӧ��
#include "GeneralAgentHandler.h"
// tcp server
#ifdef EXAMPLE_GENERALAGENTTCPSOCKETSERVER
#include "GeneralAgentTcpSocketServer.h"
#endif
#ifdef EXAMPLE_GENERALAGENTHTTPSERVER
#include "GeneralAgentHttpServer.h"
#endif
#ifdef EXAMPLE_GENERALAGENTUDPSOCKETSERVER
#include "GeneralAgentUdpSocket.h"
#endif
// tcp client, auto conn
#ifdef EXAMPLE_GENERALAGENTTCPSOCKETCONNECTOR
#include "GeneralAgentTcpSocketConnector.h"
#endif
// http client, auto conn when brocken
#ifdef EXAMPLE_GENERALAGENTHTTPKEEPCONNECTOR
#include "GeneralAgentHttpKeepConnector.h"
#endif

#ifdef EXAMPLE_SOCKET_STATUS_AGENT
#include "SocketStatusAgent.h"
#endif
#ifdef EXAMPLE_GENERALCONSOLE_OVERTCP
#include "GeneralConsoleOverTcp.h"
#endif

typedef struct __generalservercfg_t
{
    unsigned short IsUse;
    unsigned short Port;
    std::string strHost;
    // will find by name
    std::string strServerName;

    int iConnTimeOut;//��ʱʱ��
    int iConnTimes; //���ӳ��Դ�����������

    int iInterval; //[in] �����������λ���룬����Ĭ��ֵΪ30��
    int bEnableRecon ;//[in] �Ƿ�������0-��������1-����������Ĭ��ֵΪ1

    std::string strUName;
    std::string strUPwd;

    __generalservercfg_t()
    {
        IsUse = 0;
        Port = 0;
        strHost = "";
        // will find by name
        strServerName="";

        iConnTimeOut = 0;//��ʱʱ��
        iConnTimes = 0; //���ӳ��Դ�����������

        iInterval = 0; //[in] �����������λ���룬����Ĭ��ֵΪ30��
        bEnableRecon  = 0;//[in] �Ƿ�������0-��������1-����������Ĭ��ֵΪ1

        strUName = "";
        strUPwd = "";
    }
}GENERALSERVERCFG_T;


typedef struct __GeneralAgentCfg
{
    // TCP����˿�
#ifdef EXAMPLE_GENERALAGENTTCPSOCKETSERVER
    unsigned short PortTcpServer; // ��������ã�������Ϊ0
#endif
#ifdef EXAMPLE_GENERALAGENTHTTPSERVER
    unsigned short PortHttpServer; // ��������ã�������Ϊ0
#endif
    // UDP����˿�
#ifdef EXAMPLE_GENERALAGENTUDPSOCKETSERVER
    unsigned short UdpServerPort; // ��������ã�������Ϊ0
#endif
    // ״̬����˿�
#ifdef EXAMPLE_SOCKET_STATUS_AGENT
    unsigned short StatusServicePort; // ��������ã�������Ϊ0
#endif
    //���������нӿڣ� 0��Ч
#ifdef EXAMPLE_GENERALCONSOLE_OVERTCP
    unsigned short ConsoleOverTcpPort; // ���������нӿڣ� 0��Ч
#endif
#ifdef EXAMPLE_GENERALAGENTTCPSOCKETCONNECTOR
    // tcp ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T> TcpHost;
#endif

#ifdef EXAMPLE_GENERALAGENTHTTPKEEPCONNECTOR
    // http ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T> HttpHost;
#endif

}
GENERALAGENTCFG_T;

// Socket *pSocket, void *pDat, int len
//typedef TSignal3<Socket *, void *, int> SIG_GeneralAgentHandler_DATA;

#define g_GeneralAgent (*CGeneralAgent::instance())

class CGeneralAgent : public CEZThread
{
public:
    PATTERN_SINGLETON_DECLARE(CGeneralAgent);
    CGeneralAgent();
    virtual ~CGeneralAgent();
    ////////
    void Initialize(int argc, char * argv[]);
	
	// �������ò���
    void SetConfig(GENERALAGENTCFG_T *pConfig=NULL);

#ifdef EXAMPLE_SIGNAL_ONDATA
    BOOL Start(CEZObject * pObj=NULL, SIG_GeneralAgentHandler_DATA::SigProc pProc=NULL);
    BOOL Stop(CEZObject * pObj=NULL, SIG_GeneralAgentHandler_DATA::SigProc pProc=NULL);
#else
    BOOL Start();
    BOOL Stop();
#endif //EXAMPLE_SIGNAL_ONDATA
    void ThreadProc();

    // ���÷���
#ifdef EXAMPLE_GENERALAGENTTCPSOCKETSERVER
// port =0 disable, others use it
    void SetTcpServer(unsigned short  port);
#endif
#ifdef EXAMPLE_GENERALAGENTHTTPSERVER
    void SetHttpServer(unsigned short  port);
#endif
#ifdef EXAMPLE_GENERALAGENTUDPSOCKETSERVER
    void SetUdpServer(unsigned short  port);
#endif

#ifdef EXAMPLE_GENERALAGENTTCPSOCKETCONNECTOR

    void AddTcpHost(std::string strHost, int nPort, std::string strName="");
#endif //EXAMPLE_GENERALAGENTTCPSOCKETCONNECTOR
#ifdef EXAMPLE_GENERALAGENTHTTPKEEPCONNECTOR

    void AddHttpHost(std::string strHost, int nPort, std::string strServerName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);
#endif

    int ConnectorCreate();

    // �ڲ���Ҫһ����ʱ�����д���
    // Ϊȷ���ڲ�������ɣ�һ�����֮�� 10s֮���ٵ���Connect��
    int ConnectorDestroy();

    void Run();

    int Send2Socket(std::string strSocketName, const char *pData, size_t Len);
    Socket * GetSocketByName(std::string strServerName);


    void TimerProcMaintain();
    const std::string &GetVersion();
    unsigned int GetRunPeriod();
    time_t GetStartTime();

#ifdef EXAMPLE_SIGNAL_ONDATA
    BOOL OnData(Socket *pSocket, const char *pdat, size_t len);
#endif //EXAMPLE_SIGNAL_ONDATA

private:
    GENERALAGENTCFG_T m_ConfigGeneralAgent;
    CGeneralAgentLog m_Log;
    GeneralAgentHandler *m_pHandlerGeneralAgent;

#ifdef EXAMPLE_SOCKET_STATUS_AGENT

    ListenSocket<CSocketStatusAgent> *m_pSocketStatusAgent;
#endif // EXAMPLE_SOCKET_STATUS_AGENT

#ifdef EXAMPLE_GENERALCONSOLE_OVERTCP

    ListenSocket<CGeneralConsoleOverTcp> *m_pGeneralConsoleOverTcp;
#endif // EXAMPLE_GENERALCONSOLE_OVERTCP

#ifdef EXAMPLE_GENERALAGENTTCPSOCKETSERVER

    ListenSocket<CGeneralAgentTcpSocketServer> *m_pGeneralAgentTcpSocketServer;
#endif // EXAMPLE_GENERALAGENTTCPSOCKETSERVER
#ifdef EXAMPLE_GENERALAGENTHTTPSERVER

    ListenSocket<CGeneralAgentHttpServer> *m_pGeneralAgentHttpServer;
#endif // EXAMPLE_GENERALAGENTHTTPSERVER


#ifdef EXAMPLE_GENERALAGENTUDPSOCKETSERVER
	CGeneralAgentUdpSocket *m_pGeneralAgentUdpSocketServer;
#endif

    int InitializeLogs();
#ifdef ATTATCH_FUNC_OF_GENERAL_AGENT
    CEZTimer          m_TimerMaintain;          //����ܶ�ʱ��
    // ����ʱ��
    unsigned int m_iRunPeriod;
    time_t m_ttStartTime;
#endif //#ifdef ATTATCH_FUNC_OF_GENERAL_AGENT

#ifdef EXAMPLE_SIGNAL_ONDATA
    CEZMutex m_MutexSig;
    SIG_GeneralAgentHandler_DATA m_sigData;
#endif //EXAMPLE_SIGNAL_ONDATA
}
; //CGeneralAgent

#endif //_GENERALAGENT_H
