/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * SmsMsg.h - _explain_
 *
 * Copyright (C) 2012 ezlibs.com, All Rights Reserved.
 *
 * $Id: SmsMsg.h 0001 2012-5-8 14:38:29Z wu_junjie $
 *
 *  Explain:
 *     SmsMsg -- Telecom eHome Msg
 *
 *  Update:
 *     2012-5-8 14:38:33 WuJunjie 549 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _SmsMsg_H
#define _SmsMsg_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//
// �򵥷�����Ϣ
//
class CAckMsgSimple
{
public:
    std::string result;
    std::string reason;

    CAckMsgSimple()
    {
        result = "";
        reason = "";
    }

    void dump()
    {
        std::cout << "result:" << result << std::endl;
        std::cout << "reason:" << reason << std::endl;
    }
};

//
// ���ش���session�ļ���Ϣ
//
class CAckMsgSession : public CAckMsgSimple
{
public:
    std::string SessionId;
    CAckMsgSession()
    {
        SessionId = "";
    }
    void dump()
    {
        std::cout << "SessionId:" << result << std::endl;
        CAckMsgSimple::dump();
    }
};

//
// ����״̬
//
class CSmsMsg_LocalStatus
{
public:
    std::string LocalIp 		    ;// socket local address GetSockAddress
    std::string LocalDateTime		;// 2013-07-08 11:31:11

    CSmsMsg_LocalStatus()
    {
        LocalIp 	 = "";
        LocalDateTime  = "2013-07-08 11:31:11";
    }

    void dump()
    {
        std::cout << "LocalIp   :" << LocalIp  		<< std::endl;
        std::cout << "LocalDateTime :" 	 << LocalDateTime  	<< std::endl;
    }
};

//
// ע����Ϣ��
//
class CSmsMsg_RegisterReq
{
public:
    std::string ProductID 		    ;// �豸���
    std::string AuthCode 			;// ������Ȩ�� ͨ�����+��Ȩ��ֱ�ӷ���
    std::string AuthName 		    ;// ��Ȩ�û���
    std::string Password			;// AuthName + Password �ķ�ʽ����������Ȧ���� ���Ի�����֣�Ȧ�ڿͻ��˿���ͨ����ŷ���

    CSmsMsg_LocalStatus m_LocalStatus;

    CSmsMsg_RegisterReq()
    {
        ProductID 	 = "";
        AuthCode  = "";
        AuthName 	 = "";
        Password = "";
    }

    void dump()
    {
        std::cout << "ProductID   :" << ProductID  		<< std::endl;
        std::cout << "AuthCode :" 	 << AuthCode  	<< std::endl;
        std::cout << "AuthName    :" << AuthName   		<< std::endl;
        std::cout << "Password:" 	 << Password 	<< std::endl;

        std::cout << "m_LocalStatus:" 	<< std::endl;
        m_LocalStatus.dump();
    }
};

class CSmsMsg_ServiceAgent
{
public:
    std::string ServiceName 		;// 
    std::string URL					;// ip:port(192.168.6.112:10000) or http://pubservice.ezlibs.com:60089/PubService/getPubIP4

    CSmsMsg_LocalStatus()
    {
        LocalIp 	 = "";
        LocalDateTime  = "2013-07-08 11:31:11";
    }

    void dump()
    {
        std::cout << "LocalIp   :" << LocalIp  		<< std::endl;
        std::cout << "LocalDateTime :" 	 << LocalDateTime  	<< std::endl;
    }
};

//
// ע�᷵����Ϣ��
//
class CSmsMsg_RegisterAck
{
public:
    std::string LocalDateTime		;
    std::string LocalDateTime		;

    CSmsMsg_RegisterAck()
    {
        ProductID 	 = "";
        AuthCode  = "";
        AuthName 	 = "";
        Password = "";
    }

    void dump()
    {
        std::cout << "ProductID   :" << ProductID  		<< std::endl;
        std::cout << "AuthCode :" 	 << AuthCode  	<< std::endl;
        std::cout << "AuthName    :" << AuthName   		<< std::endl;
        std::cout << "Password:" 	 << Password 	<< std::endl;

        std::cout << "m_LocalStatus:" 	<< std::endl;
        m_LocalStatus.dump();
    }
};

#endif /* _SmsMsg_H */
