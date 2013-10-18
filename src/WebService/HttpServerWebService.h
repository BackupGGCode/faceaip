/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * HttpServerWebService.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: HttpServerWebService.h 5884 2013-01-29 03:20:53Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:20:53  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _HttpServerWebService_H
#define _HttpServerWebService_H

#include <HttpdSocket.h>
#include <SocketHandler.h>
#include <ListenSocket.h>
#include <StdoutLog.h>
#include "WebServiceStack.h"

#define MY_SESSION_ID_LEN 36

class CHttpServerWebService : public HttpdSocket
{
public:
    CHttpServerWebService(ISocketHandler& h);
    ~CHttpServerWebService();

    void Init();

    void Exec();

	/**
	  * matchRequest��һ���Զ���ͷ���ԣ���ʾ����Ӧƥ���ĸ�����ġ�
	  * ���ڳ������첽������Ӧʱ����ȷ�������ȷ��
	  */
    void CreateHeader(size_t ContentLength=0, const std::string &matchRequest="");

    void GenerateDocument();

    virtual void OnData(const char *,size_t);
    virtual void OnDataComplete();
    virtual void OnHeaderComplete();
    virtual void OnUnknowRequest();
    std::string getMySessionId();

private:
    std::string m_strBody;
	std::string m_requestPrefix;
	std::string m_sessionId;
	int reportId;
	
    int ReturnHttpMsg(const std::string &strMsg, const std::string &matchRequest="");
    int sendDeviceCtrlMsg(char *ip,
                       const int port,
                       const char *operate_id,
                       const char *value,
                       const u_char value_len,
                       const int32_t session_id);
    // get a new
    std::string generateSessionId();
	
	//���Խӿ�
    int Do_TestIF( CWebServiceMsg &JsonData) ;
    // WEB��ϵͳ��������ϵͳע��ӿڵ�ַ�Ľӿ�
    int Do_registerURL( CWebServiceMsg &JsonData)						;
    // WEB��ϵͳ��֤�ӿ�
    int Do_requestAuth( CWebServiceMsg &JsonData)						;
    // ������֤�ӿ�
    int Do_requestAuthByHost( CWebServiceMsg &JsonData)						;
    // �����ӿ�
    int Do_heartbeat( CWebServiceMsg &JsonData)						;
    //ֹͣ������������ӿ�
    int Do_stopHostNetwork( CWebServiceMsg &JsonData)  ;
    //�������ø��½ӿ�
    int Do_notifyUpdate( CWebServiceMsg &JsonData)              ;
    //�ϱ��豸״̬�ӿ�
    int Do_reportDeviceStateList( CWebServiceMsg &JsonData)                   ;
    //��ȡ�豸״̬�ӿ� getDeviceStateList
    int Do_getDeviceStateList( CWebServiceMsg &JsonData)              ;
    //��������ӿ� controlDevice
    int Do_controlDevice( CWebServiceMsg &JsonData)                   ;

};

#endif // _HttpServerWebService_H

