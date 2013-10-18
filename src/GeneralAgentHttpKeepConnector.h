/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentHttpKeepConnector.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentHttpKeepConnector.h 5884 2012-07-10 04:10:20Z WuJunjie $
 *
 *  Explain:
 *     -
 *      http ��������
 *      �������ӣ��Ͽ����Զ��ٴ�����
 *     -
 *
 *  Update:
 *     2012-07-10 04:10:20  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef _GeneralAgentHttpKeepConnector_H
#define _GeneralAgentHttpKeepConnector_H

#include "GeneralAgentTcpSocket.h"
#include <ISocketHandler.h>
/*ezutil/http_parser.h*/
#include <http_parser.h>

#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"

// �߳�����
//#define CGENERALAGENTHTTPKEEPCONNECTOR_THREAD_ATTR "CGeneralAgentHttpKeepConnector", TP_DEFAULT, 100

#define CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME "GeneralAgentHttpKeepConnector"

class CGeneralAgentHttpKeepConnector : public TcpSocket//,public CEZThread
{
public:
    CGeneralAgentHttpKeepConnector(ISocketHandler&, std::string strSocketName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);
    ~CGeneralAgentHttpKeepConnector();

    void SetCloseAndDelete();
    void OnConnectFailed();
    void OnDelete();

    // �� TcpSocket::OnRead(); �е���
    virtual void OnRawData(const char *buf,size_t len);

    /////////////////////////////
    // �����������
    virtual void OnConnect();
    virtual void OnReconnect();
    virtual void OnDisconnect();
    virtual void OnConnectTimeout();
    virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);
    /////////////////////////////

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    void ThreadProc();
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CGeneralAgentHttpKeepConnector(const CGeneralAgentHttpKeepConnector& s) : TcpSocket(s)//,CEZThread(CGENERALAGENTHTTPKEEPCONNECTOR_THREAD_ATTR)
    {} // copy constructor
    CGeneralAgentHttpKeepConnector& operator=(const CGeneralAgentHttpKeepConnector& )
    {
        return *this;
    } // assignment operator
    /** Create a new instance and reconnect */
    CGeneralAgentHttpKeepConnector *Reconnect();
    bool m_b_connected;
    time_t m_tOnData; ///< last Time in seconds when this socket was on data

    /////////////
    http_parser *m_pHttpParserCli;
    http_parser_settings *m_pHttpParserSettings;
};

#endif // _GeneralAgentHttpKeepConnector_H
