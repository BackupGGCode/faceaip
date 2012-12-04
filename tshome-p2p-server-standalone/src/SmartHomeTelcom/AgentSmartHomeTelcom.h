/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * AgentSmartHomeTelcom.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: AgentSmartHomeTelcom.h 5884 2012-09-06 04:23:58Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-06 04:23:58  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _AgentSmartHomeTelcom_H
#define _AgentSmartHomeTelcom_H

#include "CommonInclude.h"
#include "EZThread.h"
#include "IniFile.h"

#include <time.h>
#include <vector>
#include <ListenSocket.h>

// ��Ӧ��
#include "HandlerSmartHomeTelcom.h"
#include "../common/DoubleOutLog.h"

// http client, auto conn when brocken
#include "HttpKeepConnectorSmartHomeTelcom.h"

//typedef struct __generalservercfg_t
//{
//    unsigned short IsUse;
//    unsigned short Port;
//    char strIP[16];
//    // will find by name
//    std::string strServerName;
//}
//GENERALSERVERCFG_T; 
#include "SmartHomeTelcomDef.h" /* port etc */


typedef struct __AgentSmartHomeTelcomCfg
{
    // ��������˿�
    unsigned short ServicePort; // ��������ã�������Ϊ0
    // ����״̬����˿�
    unsigned short StatusServicePort; // ��������ã�������Ϊ0
    //���������нӿڣ� 0��Ч
    unsigned short ConsoleOverTcpPort; // ���������нӿڣ� 0��Ч
#ifdef USE_GENERALAGENTTCPSOCKETCONNECTOR
    // tcp ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T> Peer;
#endif

#ifdef USE_HTTPKEEPCONNECTORSMARTHOME
    // http ��λ��
    // ����Ϊ������ 2012-7-10 16:58:48
    std::vector<GENERALSERVERCFG_T> PeerHttp;
#endif

}
AGENT_SMARTHOME_JSTELCOM_CFG_T;

#define g_SmartHomeAgentTelcom (*CAgentSmartHomeTelcom::instance())

class CAgentSmartHomeTelcom : public CEZThread
{
public:
    PATTERN_SINGLETON_DECLARE(CAgentSmartHomeTelcom);
    CAgentSmartHomeTelcom();
    virtual ~CAgentSmartHomeTelcom();
    ////////

    BOOL Start();
    BOOL Stop();
    void ThreadProc();
    void SetDefaultConfig();

    // ���÷���
    void SetGeneralAgent(unsigned short  port);

#ifdef USE_HTTPKEEPCONNECTORSMARTHOME

    void AddSubHttpHost(char* szIP, int nPort, std::string strServerName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);
#endif

    void ConnectAll();

    void ConnectAllSync();

    // �ڲ���Ҫһ����ʱ�����д���
    // ����֮�� 10s֮���ٵ���Connect��
    void DisconnectAll();

    void Run();

    Socket * GetSocketByName(std::string strServerName);
    int Send2Socket(std::string strSocketName, const char *pData, size_t Len);

    CHttpKeepConnectorSmartHomeTelcom* GetHttpKeepConnector(std::string strSocketName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);

private:
    AGENT_SMARTHOME_JSTELCOM_CFG_T m_ConfigGeneralAgent;
    CHandlerSmartHomeTelcom *m_pHandlerSmartHomeTelcom;
    CDoubleOutLog m_Log;

#ifdef USE_HTTPKEEPCONNECTORSMARTHOME
    //std::vector<CHttpKeepConnectorSmartHomeTelcom> m_HttpConnector;
#endif // USE_HTTPKEEPCONNECTORSMARTHOME
}
; //CAgentSmartHomeTelcom
#endif //_AgentSmartHomeTelcom_H
