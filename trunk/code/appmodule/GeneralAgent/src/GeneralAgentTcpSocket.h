/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocket.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentTcpSocket.h 0001 2012-04-06 09:58:28Z WuJunjie $
 *
 *  Explain:
 *     -tcp ͳһд������ͳһЭ�����-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _GENERALAGENTTCPSOCKET_H
#define _GENERALAGENTTCPSOCKET_H

#include <stdio.h>

#include <TcpSocket.h>
#include <ISocketHandler.h>

#ifdef EXAMPLE_DATAPARSE_EXAMPLE
//!wifi ����Э�飬 ͨ�ò���
typedef struct __wifi_agent_protocol_head
{
    unsigned char u8CmdOrStatus;
    unsigned char u8AddOrCommand;
    unsigned char u8DataLen;
}
WIFI_AGENT_PROTOCOL_HEAD;
#define WIFI_AGENT_PROTOCOL_DATA_TIMEOUT 10 // SECOND
#define WIFI_AGENT_PROTOCOL_DATA_MAX_LEN 64 // ��󻺴�
#endif //EXAMPLE_DATAPARSE_EXAMPLE

class CGeneralAgentTcpSocket : public TcpSocket
{
public:
    CGeneralAgentTcpSocket(ISocketHandler& , std::string strSocketName="GeneralAgentTcpSocket");
    
    // �� TcpSocket::OnRead(); �е���
    virtual void OnRawData(const char *buf,size_t len);
    // ͳһcommand
    // һ��ϵͳ��Э����ϵӦ����һ���ģ� wujunjie 2012-4-9 15:35:31
    virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);

#ifdef EXAMPLE_DATAPARSE_EXAMPLE
    // ͳһread
    // һ��ϵͳ��Э����ϵӦ����һ���ģ� wujunjie 2012-4-9 15:35:31
    virtual void OnRead();
    // ͨ��ͷ pCmd
    // ���� pData�� ���ȼ� pCmd->u8DataLen
    virtual void OnCommand_WifiAgent(WIFI_AGENT_PROTOCOL_HEAD *pCmd, unsigned char *pData);
private:
    //  �����ϴ�δ�����ͷ��
    WIFI_AGENT_PROTOCOL_HEAD m_headLastOn;
#endif

private:
    time_t m_tOnData; ///< last Time in seconds when this socket was on data
};
#endif // _GENERALAGENTTCPSOCKET_H
