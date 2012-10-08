/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * HttpServerSmartHomeTelcomCtrlServer.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: HttpServerSmartHomeTelcomCtrlServer.h 5884 2012-09-06 01:31:28Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-06 01:31:28  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _HttpServerSmartHomeTelcomCtrlServer_H
#define _HttpServerSmartHomeTelcomCtrlServer_H

#include <HttpdSocket.h>
#include <SocketHandler.h>
#include <ListenSocket.h>
#include <StdoutLog.h>
#include "../TeHomeIF.h"

class CHttpServerSmartHomeTelcomCtrlServer : public HttpdSocket
{
public:
    CHttpServerSmartHomeTelcomCtrlServer(ISocketHandler& h);
    ~CHttpServerSmartHomeTelcomCtrlServer();

    void Init();

    void Exec();

    void CreateHeader();

    void GenerateDocument();

    virtual void OnData(const char *,size_t);
    virtual void OnDataComplete();
    virtual void OnHeaderComplete();

private:
    std::string m_strBody;
    int ReturnHttpMsg(const std::string &strMsg);
    int sendDeviceCtrlMsg(char *ip,
                       const int port,
                       const char *operate_id,
                       const char *value,
                       const u_char value_len,
                       const int32_t session_id);
	
    // 5.2.2	ע���û����ؽӿ�
    int Do_register( CTeHomeIF &JsonData)						;
    // 0.0.0	����豸�ӿ�
    int Do_addDevice( CTeHomeIF &JsonData)						;
    //5.2.4	���������ϱ��ӿ�
    int Do_requestReportDeviceConfigParam( CTeHomeIF &JsonData)  ;
    //5.2.6	��ȡ����״̬�ӿ� getCtrlDeviceState
    int Do_getCtrlDeviceState( CTeHomeIF &JsonData)              ;
    //5.2.8	��ȡ�豸״̬�ӿ� getDeviceStateList
    int Do_getDeviceStateList( CTeHomeIF &JsonData)              ;
    //5.2.9	��������ӿ� controlDevice
    int Do_controlDevice( CTeHomeIF &JsonData)                   ;

    // ����Ϊ����WebService�ӿ�
    // �˺�
    int Do_AddAccount( CTeHomeIF &JsonData)  ;
    int Do_DelAccount( CTeHomeIF &JsonData)  ;
    int Do_ModAccount( CTeHomeIF &JsonData)  ;
    int Do_QryAccount( CTeHomeIF &JsonData)  ;

	// ���͡������
    int Do_AddMap( CTeHomeIF &JsonData)   ;
    int Do_DelMap( CTeHomeIF &JsonData)   ;
    int Do_ModMap( CTeHomeIF &JsonData)   ;
    int Do_QryMap( CTeHomeIF &JsonData)   ;

	// �豸, ���а�����������
    int Do_AddDevice( CTeHomeIF &JsonData)   ;
    int Do_DelDevice( CTeHomeIF &JsonData)   ;
    int Do_ModDevice( CTeHomeIF &JsonData)   ;
    int Do_QryDevice( CTeHomeIF &JsonData)   ;
    int Do_ControlDevice( CTeHomeIF &JsonData)   ;

	// ��ͥ����λ ά���� ʵ�ʽ��˺š����ͽ��й���
    int Do_AddFamily( CTeHomeIF &JsonData)   ;
    int Do_DelFamily( CTeHomeIF &JsonData)   ;
    int Do_ModFamily( CTeHomeIF &JsonData)   ;
    int Do_QryFamily( CTeHomeIF &JsonData)   ;
};

#endif // _HttpServerSmartHomeTelcomCtrlServer_H

