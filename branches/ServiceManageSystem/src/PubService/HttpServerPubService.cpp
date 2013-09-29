/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * HttpServerPubService.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: HttpServerPubService.cpp 5884 2013-01-29 03:21:34Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:21:34  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//#include "CommonInclude.h"
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include "HttpServerPubService.h"
#include "../Logs.h"
#include "../Configs/ConfigPubService.h"
#include "str_opr.h"
#include "StringOpr.h"
#include "HandlerPubService.h"
#ifdef _DEBUG
	#define DEB(x)
	#define DBG(x)
#else
	#define DEB(x)
	#define DBG(x)
#endif

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

#define ARG_USED(x) (void)&x;


CHttpServerPubService::CHttpServerPubService(ISocketHandler& h) : HttpdSocket(h)
{
    m_strBody = "";
    m_requestPrefix = "";
    m_sessionId = "";
}

void CHttpServerPubService::Init()
{
    if (GetParent() -> GetPort() == 443 || GetParent() -> GetPort() == 8443)
    {
        fprintf(stderr, "SSL not available\n");
    }

    LOG4CPLUS_INFO(LOG_WEBSERVICES, "CHttpServerPubService::On:" << GetRemoteAddress() << ":" << GetRemotePort());
}

CHttpServerPubService::~CHttpServerPubService()
{
    LOG4CPLUS_INFO(LOG_WEBSERVICES, "CHttpServerPubService::Destroy:" << GetRemoteAddress() << ":" << GetRemotePort());
}

void CHttpServerPubService::Exec()
{
    CreateHeader();
    GenerateDocument();
}
void CHttpServerPubService::OnHeaderComplete()
{
    m_strBody = "";
}

void CHttpServerPubService::OnUnknowRequest()
{}

void CHttpServerPubService::CreateHeader(size_t ContentLength, const std::string &matchRequest)
{
    SetStatus("200");
    SetStatusText("OK");
    fprintf(stderr, "Uri: '%s'\n", GetUri().c_str());
    {
        size_t x = 0;
        for (size_t i = 0; i < GetUri().size(); i++)
            if (GetUri()[i] == '.')
                x = i;
        std::string ext = GetUri().substr(x + 1);
        if (ext == "gif" || ext == "jpg" || ext == "png")
            AddResponseHeader("Content-type", "image/" + ext);
        else
            AddResponseHeader("Content-type", "text/" + ext);
    }
    //AddResponseHeader("Connection", "close");
    AddResponseHeader("Connection", "Keep-Alive");

    if (m_sessionId.size()==MY_SESSION_ID_LEN)
    {
        AddResponseHeader("Cookie", "JSESSIONID="+m_sessionId);
    }
    else
    {
        // debug only
        m_sessionId = "dbg only";
        AddResponseHeader("Cookie", "JSESSIONID="+m_sessionId);
        m_sessionId="";
    }

    if (ContentLength>0)
    {
        AddResponseHeader( "Content-length", Utility::l2string((long)ContentLength) );
    }

    if (matchRequest != "")
    {
        AddResponseHeader( "Match-request", matchRequest);
    }
    SendResponse();
}

void CHttpServerPubService::GenerateDocument()
{
    std::string strSendBuf = "Hello CHttpServerPubService.\n\r";

    strSendBuf += GetHttpDate()+"\r\n";
    strSendBuf += "Your Body:"+m_strBody+"\n\r";

    strSendBuf = "[{\"result\":\"100\",\"reason\":\"CHttpServerPubService not support url:" + GetUrl() + "\"}]";

    //Send(strSendBuf);
    ReturnHttpMsg(strSendBuf);
}

/** Chunk of http body data recevied. */
void CHttpServerPubService::OnData(const char *p,size_t l)
{
    if (m_strBody.size()>4096 || l>4096)
    {
        LOG4CPLUS_ERROR(LOG_WEBSERVICES, "m_strBody(" << m_strBody.size() << ") or l("<<l<<")"<<"too big");
        std::string strBadboy = "Bad boy.";

        ReturnHttpMsg(strBadboy);

        //Reset(); // prepare for next request
        //SetCloseAndDelete();
        //m_strBody = "";

        return;
    }

    m_strBody += p;
}

void CHttpServerPubService::OnDataComplete()
{
    CConfigPubService __cfg;
    __cfg.update();
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    //LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "m_strBody:" << m_strBody);

    if (m_strBody.size()<strlen("[{}]"))
    {
        LOG4CPLUS_INFO(LOG_WEBSERVICES, "m_strBody(" << m_strBody.size() << ") not json");
    }
    else
    {

        size_t ii;
        for(ii=m_strBody.length()-1;ii>0; ii--)
        {
            if (m_strBody.at(ii)==']')
            {
                break;
            }
        }
        //if (ii !=m_strBody.length()-1)
        if (ii > 3)
        {
            //m_strBody.erase(ii+1,m_strBody.length()-1);
            m_strBody.erase(ii,m_strBody.length()-1);
        }

        for(ii=0;ii<m_strBody.length()-1; ii++)
        {
            if (m_strBody.at(ii)=='[')
            {
                m_strBody.erase(0,ii+1);
                break;
            }
        }

        LOG4CPLUS_INFO(LOG_WEBSERVICES, "Html request:"
                       << "\n\t\t Method: "<<GetMethod()
                       << "\n\t\t URL: "<<GetUrl()
                       << "\n\t\t Http version: "<<GetHttpVersion()
                       << "\n\t\t Body: "<<m_strBody << "\n\t\t\t");


        __CTSHomeWebServiceIF.Parse(m_strBody);
        //CreateHeader();
    }

    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Request URL:" << GetUrl());
    //exa:
    //	curl http://192.168.6.112:60089/PubService/getPubIP4 -d "[{\"reason\":\"OK\",\"result\":\"0\"}]"
    //	curl http://192.168.6.112:60089/PubService/getPubIP4 -d ""
    //	curl http://192.168.6.112:60089/PubService/getPubIP4
    // test interface
    if (GetUrl()==__cfg.getConfig().strURL_getPubIP4)
    {
        Do_getPubIP4(__CTSHomeWebServiceIF);
    }
    //exa:
    //	curl http://192.168.10.125:60088/PubService/TestIF -d "[{\"reason\":\"OK\",\"result\":\"0\"}]"
    // test interface
    else if (GetUrl()=="/PubService/TestIF")
    {
        Do_TestIF(__CTSHomeWebServiceIF);
    }
#if 0
    //��֤�ӿ�
    //	http://218.95.39.172:60088/PubService/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_requestAuth)
    {
        Do_requestAuth(__CTSHomeWebServiceIF);
    }//������֤�ӿ�
    //	http://218.95.39.172:60088/PubService/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_requestAuthByHost)
    {
        Do_requestAuthByHost(__CTSHomeWebServiceIF);
    }
    //5.2.6	�����ӿ� getCtrlDeviceState
    //	http://192.168.10.216:60088/PubService/heartbeat
    else if (GetUrl()==__cfg.getConfig().strURL_heartbeat)
    {
        Do_heartbeat(__CTSHomeWebServiceIF);
    }
    //5.2.8	��ȡ�豸״̬�ӿ� getDeviceStateList
    //	http://218.95.39.172:60088/PubService/getDeviceStateList
    //
    else if (GetUrl()==__cfg.getConfig().strURL_registerURL)
    {
        Do_registerURL(__CTSHomeWebServiceIF);
    }
    //5.2.4	���������ϱ��ӿ�
    //	http://218.95.39.172:60088/PubService/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_stopHostNetwork)
    {
        Do_stopHostNetwork(__CTSHomeWebServiceIF);
    }
    //5.2.6	��ȡ����״̬�ӿ� getCtrlDeviceState
    //	http://218.95.39.172:60088/PubService/getCtrlDeviceState
    else if (GetUrl()==__cfg.getConfig().strURL_notifyUpdate)
    {
        Do_notifyUpdate(__CTSHomeWebServiceIF);
    }
    //5.2.8	��ȡ�豸״̬�ӿ� getDeviceStateList
    //	http://218.95.39.172:60088/PubService/getDeviceStateList
    //
    else if (GetUrl()==__cfg.getConfig().strURL_getDeviceStateList)
    {
        Do_getDeviceStateList(__CTSHomeWebServiceIF);
    }
    //5.2.9	��������ӿ� controlDevice
    //	http://218.95.39.172:60088/PubService/controlDevice
    else if (GetUrl()==__cfg.getConfig().strURL_controlDevice)
    {
        Do_controlDevice(__CTSHomeWebServiceIF);
    }
#endif
    else
    {
        GenerateDocument();
    }

    Reset(); // prepare for next request
    //SetCloseAndDelete();
    m_strBody = "";
}

int CHttpServerPubService::ReturnHttpMsg(const std::string &strMsg, const std::string &matchRequest)
{
    cout << "strMsg.size() :" <<  strMsg.size()  << endl;
    CreateHeader(strMsg.size(), matchRequest);

    cout << "strMsg :" <<  strMsg  << endl;

    Send(strMsg);
    Reset();
    //SetCloseAndDelete();
    m_strBody = "";

    return 0;
}

std::string CHttpServerPubService::generateSessionId()
{
    char uuidBuff[37] = {0};
    uuid_t uuidGenerated;
    uuid_generate_random(uuidGenerated);
    uuid_unparse(uuidGenerated, uuidBuff);

    return uuidBuff;
}

std::string CHttpServerPubService::getMySessionId()
{
    return m_sessionId;
}
// curl http://localhost:60088/PubService/TestIF -d "[{\"reason\":\"I Love You.\",\"result\":\"0\"}]"
int CHttpServerPubService::Do_TestIF( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_TestIF");
    __fline;
    cout << "Do_TestIF, your json:" << JsonData.ToString()<< endl;

    std::string strSendBuf;

    strSendBuf = "[{\"result\":\"0\",\"reason\":\"Do_TestIF, Your URL:" + GetUrl() + "\"}]";

    ReturnHttpMsg(strSendBuf);

    return 0;
}

// WEB��ϵͳ��������ϵͳע��ӿڵ�ַ�Ľӿ�
int CHttpServerPubService::Do_registerURL( CTSHomeWebServiceIF &JsonData)
{
//    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_registerURL");
//    CIPC_registerURL __op;
//    JsonData.Decode(__op);
//    __op.dump();
//    m_sessionId = __op.sessionId;
//
//    // ���췵����Ϣ
//    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
//
//    CIPC_registerURL_ACK Msg;
//
//    if (static_cast<CHandlerPubService&>(Handler()).AuthSession(m_sessionId, EAW_REGISTERURL) ==0)
//    {
//
//        Msg.result = "0";
//        Msg.reason = "Successful";
//        m_requestPrefix = __op.baseURL;//TODO ��Ϊȫ�ֱ����洢��ֵ
//    }
//    else
//    {
//        Msg.result = "1";
//        Msg.reason = "Authentication failed";
//    }
//    __CTSHomeWebServiceIF.Encode(Msg);
//    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";
//
//
//    //Send(strSnd);
//    ReturnHttpMsg(strSnd);
//    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;
}
// WEB��ϵͳ��֤�ӿ�
int CHttpServerPubService::Do_requestAuth( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_requestAuth");
//    CIPC_requestAuth __op;
//    JsonData.Decode(__op);
//    __op.dump();
//
//    // ���췵����Ϣ
//    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
//
//    CAuthResponse Msg;
//
//    if(m_sessionId != "")
//    {
//        Msg.result = "1";
//        Msg.reason = "Don't allow more internal connection";
//        Msg.sessionId = "";
//    }
//    else
//    {
//        if(__op.authUser == "ipcServer" && __op.authPassword == "123456")
//        {
//            Msg.result = "0";
//            Msg.reason = "Successful";
//            m_sessionId = generateSessionId();//"hahahahaha";
//            Msg.sessionId = m_sessionId;
//
//            CSession __session;
//            __session.m_strSession = m_sessionId;
//
//            // Ȩ��ʾ������Ȩ
//            __session.m_vAccess.push_back(EAW_HEARTBEAT);
//            __session.m_vAccess.push_back(EAW_NOTIFYUPDATE);
//            __session.m_vAccess.push_back(EAW_STOPHOSTNET);
//            __session.m_vAccess.push_back(EAW_REGISTERURL);
//            __session.m_vAccess.push_back(EAW_CTRLDEVICE);
//            //__session.m_ttLastRequestTime = time(NULL);
//
//            static_cast<CHandlerPubService&>(Handler()).newSession(__session);
//        }
//        else
//        {
//            Msg.result = "1";
//            Msg.reason = "UserName or password is wrong";
//            Msg.sessionId = "";
//        }
//    }
//    __CTSHomeWebServiceIF.Encode(Msg);
//    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";
//
//
//    //Send(strSnd);
//    ReturnHttpMsg(strSnd);
//    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;

}

// WEB��ϵͳ��֤�ӿ�
#if 1
int CHttpServerPubService::Do_requestAuthByHost( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_requestAuth");
//    CIPC_requestAuth __op;
//    JsonData.Decode(__op);
//    __op.dump();
//
//    // ���췵����Ϣ
//    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
//
//    CAuthResponse Msg;
//
//    if(m_sessionId != "")
//    {
//        Msg.result = "1";
//        Msg.reason = "Don't allow more internal connection";
//        Msg.sessionId = "";
//    }
//    else
//    {
//        if(__op.authUser == "ipcServer" && __op.authPassword == "123456")
//        {
//            Msg.result = "0";
//            Msg.reason = "Successful";
//            m_sessionId = generateSessionId();//"hahahahaha";
//            Msg.sessionId = m_sessionId;
//
//            CSession __session;
//            __session.m_strSession = m_sessionId;
//
//            // Ȩ��ʾ������Ȩ
//            __session.m_vAccess.push_back(EAW_HEARTBEAT);
//            __session.m_vAccess.push_back(EAW_NOTIFYUPDATE);
//            __session.m_vAccess.push_back(EAW_STOPHOSTNET);
//            __session.m_vAccess.push_back(EAW_REGISTERURL);
//            __session.m_vAccess.push_back(EAW_CTRLDEVICE);
//            //__session.m_ttLastRequestTime = time(NULL);
//
//            static_cast<CHandlerPubService&>(Handler()).newSession(__session);
//        }
//        else
//        {
//            Msg.result = "1";
//            Msg.reason = "UserName or password is wrong";
//            Msg.sessionId = "";
//        }
//    }
//    __CTSHomeWebServiceIF.Encode(Msg);
//    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";
//
//
//    //Send(strSnd);
//    ReturnHttpMsg(strSnd);
//    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;

}
#endif
enum EnAuthorityWebService
{
    EAW_HEARTBEAT	=  1,		// �������		_01
    EAW_NOTIFYUPDATE,			// �������		_02
    EAW_STOPHOSTNET,			// �������		_03
    EAW_REGISTERURL,			// �������		_04
    EAW_CTRLDEVICE,				// �������		_05
    EAW_SHUTDOWN,				// �ر�ϵͳ		_06
    EAW_END			= -1,
};
// WEB��ϵͳ�����ӿ�
int CHttpServerPubService::Do_heartbeat( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_heartbeat");
    CIPC_heartbeat __op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;

    cout << "__op.sessionId = " << __op.sessionId << endl;

    // ���췵����Ϣ
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CIPC_heartbeat_ACK Msg;

    if (static_cast<CHandlerPubService&>(Handler()).AuthSession(m_sessionId, EAW_HEARTBEAT) ==0)
    {
        __fline;
        cout << "AuthSession succeeded." << endl;
        Msg.result = "0";
        Msg.reason = "Heartbeat success";
    }
    else
    {
        __trip;
        cout << "AuthSession FAILED!" << endl;
        Msg.result = "1";
        Msg.reason = "ERR_SESSIONID_LOSE_MATCH";
    }

    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";

    //Send(strSnd);
    ReturnHttpMsg(strSnd, GetUrl());
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;

}

//ֹͣ������������ӿ�
int CHttpServerPubService::Do_stopHostNetwork( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_stopHostNetwork");
    CIPC_stopHostNetwork __op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;

    // ���췵����Ϣ
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_stopHostNetwork_ACK Msg;

    if(static_cast<CHandlerPubService&>(Handler()).AuthSession(m_sessionId, EAW_STOPHOSTNET) ==0)
    {

        Msg.result = "0";
        Msg.reason = "OK";
    }
    else
    {
        Msg.result = "1";
        Msg.reason = "ERR_SESSIONID_LOSE_MATCH";
    }

    if(Msg.result != "0")
    {
        __CTSHomeWebServiceIF.Encode(Msg);
        std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";

        //Send(strSnd);
        ReturnHttpMsg(strSnd);

        LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);
    }
    return 0;
}
//�������ø��½ӿ�
int CHttpServerPubService::Do_notifyUpdate( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "NOTIFY_UPDATE");
    CIPC_notifyUpdate __op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;

    // ���췵����Ϣ
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_notifyUpdate_ACK Msg;
    //CHandlerPubService handler = dynamic_cast<CHandlerPubService&>(Handler());

    if(static_cast<CHandlerPubService&>(Handler()).AuthSession(m_sessionId, EAW_NOTIFYUPDATE) ==0)
    {
        Msg.result = "0";
        Msg.reason = "Success";
    }
    else
    {
        Msg.result = "1";
        Msg.reason = "ERR_SESSIONID_LOSE_MATCH";
    }

    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";
    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);
    return 0;

}

int CHttpServerPubService::Do_getDeviceStateList( CTSHomeWebServiceIF &JsonData)
{
#if 0
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_getDeviceStateList");

    CIPC_getDeviceStateList  __op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;
    // ��ʼʹ������

    // ���췵����Ϣ
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CIPC_getDeviceStateList_ACK Msg;
    Msg.result = "0";
    Msg.reason = "OK";

    CIPC_getDeviceStateList_ACK_operate _operate;
    _operate.operate_id      = TEST_OPERATE_ID_SW;
    _operate.operate_ranage  = ""; // ������Ĵ����裬��ؿգ� ������ֵ
    _operate.operate_type    = "0"; //����
    _operate.operate_value   = "2"; // ��
    _operate.operate_explain = "���";
    ezGBKToUTF8(_operate.operate_explain);
    char pBuff[32];
    _operate.value_time       = get_date_time_string(pBuff, NULL);

    Msg.m_operate.push_back(_operate);

    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";


    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

#endif

    return 0;
}


//�����豸��������
int CHttpServerPubService::sendDeviceCtrlMsg(char *ip,
        const int port,
        const char *operate_id,
        const char *value,
        const u_char value_len,
        const int32_t session_id)
{

    return 0;
}

int CHttpServerPubService::Do_controlDevice(CTSHomeWebServiceIF &JsonData)
{
    CIPC_controlDevice   __op;
    CIPC_controlDevice_operate op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;

    /*
      *���췵����Ϣ���˽ӿ�ֻ�����Ϳ��������ĺͻظ�������Ϣ��
      *���ƽ��(��Ӧ��Ϣ)���յ����������Ļظ����ٷ��͸�web�ˡ�
      *���ԣ��ýӿ���Ҫ����һ��webͨ�Ŷ������������Թ�ϵ��
      *���յ������Ļظ���Ϣʱ�����ڲ�ѯ��ص�webͨ�Ŷ���
      */
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_controlDevice_ACK Msg;

    if(static_cast<CHandlerPubService&>(Handler()).AuthSession(m_sessionId, EAW_CTRLDEVICE) ==0)
    {

        Msg.result  = "0" ;
        Msg.reason  = "OK" ;
        return 0;
    }
    else
    {
        Msg.result  = "1" ;
        Msg.reason  = "ERR_SESSIONID_LOSE_MATCH" ;
    }

    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";

    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);
    return 0;
}

int CHttpServerPubService::Do_getPubIP4( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_getPubIP4");

    std::string strSendBuf;
    if (JsonData.isValid())
    {
        __fline;
        cout << "Do_TestIF, your json:" << JsonData.ToString()<< endl;
        strSendBuf = "[{\"result\":\"0\",\"reason\":\"Do_TestIF, Your URL:" + GetUrl() + "\"}]";
    }
    else
    {
        //strSendBuf = 	GetRemoteAddress();
	strSendBuf = 	"PUIP="+GetRemoteAddress()+" ";

    }

    ReturnHttpMsg(strSendBuf);

    SetCloseAndDelete();

    return 0;
}


