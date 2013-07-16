/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * HttpServerSmsOverHttp.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: HttpServerSmsOverHttp.h 5884 2013-01-29 03:20:53Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:20:53  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _HttpServerSmsOverHttp_H
#define _HttpServerSmsOverHttp_H

#include <HttpdSocket.h>
#include <SocketHandler.h>
#include <ListenSocket.h>
#include <StdoutLog.h>
#include "../Stack/SmsStack.h"

#define MY_SESSION_ID_LEN 36

class CHttpServerSmsOverHttp : public HttpdSocket
{
public:
    CHttpServerSmsOverHttp(ISocketHandler& h);
    ~CHttpServerSmsOverHttp();

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
    int Do_TestIF( CSmsStack &JsonData) ;
    // WEB��ϵͳ��������ϵͳע��ӿڵ�ַ�Ľӿ�
    int Do_registerURL( CSmsStack &JsonData)						;
    
#if 0
    // WEB��ϵͳ��֤�ӿ�
    int Do_requestAuth( CTSHomeWebServiceIF &JsonData)						;
    // ������֤�ӿ�
    int Do_requestAuthByHost( CTSHomeWebServiceIF &JsonData)						;
    // �����ӿ�
    int Do_heartbeat( CTSHomeWebServiceIF &JsonData)						;
    //ֹͣ������������ӿ�
    int Do_stopHostNetwork( CTSHomeWebServiceIF &JsonData)  ;
    //�������ø��½ӿ�
    int Do_notifyUpdate( CTSHomeWebServiceIF &JsonData)              ;
    //�ϱ��豸״̬�ӿ�
    int Do_reportDeviceStateList( CTSHomeWebServiceIF &JsonData)                   ;
    //��ȡ�豸״̬�ӿ� getDeviceStateList
    int Do_getDeviceStateList( CTSHomeWebServiceIF &JsonData)              ;
    //��������ӿ� controlDevice
    int Do_controlDevice( CTSHomeWebServiceIF &JsonData)                   ;

    int Do_getPubIP4( CTSHomeWebServiceIF &JsonData)                   ;
#endif
};

#endif // _HttpServerSmsOverHttp_H

