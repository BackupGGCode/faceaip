/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentTcpSocket.h - _explain_
 *
 * Copyright (C) 2011 WuJunjie(Joy.Woo@hotmail.com), All Rights Reserved.
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
#include "CommonInclude.h"
#include <iostream>
#include <string.h>
#include "Def_ODIP.h"
#include "base64.h"
#include "ezBuffer.h"
#include "des.h"
#include "md5.h"

#ifdef INTERNAL_ODIP_DATA_PARSE
#define ODIP_PROTOCOL_DATA_TIMEOUT 10 // SECOND
#define ODIP_PROTOCOL_DATA_MAX_LEN 1024*9 // ��� ��
#endif //INTERNAL_ODIP_DATA_PARSE

class CGeneralAgentTcpSocket : public TcpSocket
{
public:
    CGeneralAgentTcpSocket(ISocketHandler& , std::string strSocketName="GeneralAgentTcpSocket");
    
    // �� TcpSocket::OnRead(); �е���
    virtual void OnRawData(const char *buf,size_t len);
    // ͳһcommand
    // һ��ϵͳ��Э����ϵӦ����һ���ģ� wujunjie 2012-4-9 15:35:31
    virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);

#ifdef INTERNAL_ODIP_DATA_PARSE
    // ͳһread
    // һ��ϵͳ��Э����ϵӦ����һ���ģ� wujunjie 2012-4-9 15:35:31
    virtual void OnRead();
    // ͨ��ͷ pCmd
    // ���� pData�� ���ȼ� pCmd->u8DataLen
    virtual void OnCommand_ODIP(const ODIP_HEAD_T *pCmd, const unsigned char *pData);
private:
    //  �����ϴ�δ�����ͷ��
    //WIFI_AGENT_PROTOCOL_HEAD m_headLastOn;
	
    ODIP_HEAD_T m_headLastOn;
    // ����ĸ���ʵ�ʶ�������
    unsigned char m_OdipDataBuffer[4096*2];
#endif

private:
    time_t m_tOnData; ///< last Time in seconds when this socket was on data



CezBuffer g_cabRecvBuff;
// �ϴ�����
//CezBuffer g_cabLastCMD;

//ODIP_HEAD_T g_Dvrip;

};
#endif // _GENERALAGENTTCPSOCKET_H
