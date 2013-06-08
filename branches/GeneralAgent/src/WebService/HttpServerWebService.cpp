/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * HttpServerWebService.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: HttpServerWebService.cpp 5884 2013-01-29 03:21:34Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:21:34  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "CommonInclude.h"
#include <string.h>
#include <stdio.h>
#include <uuid/uuid.h>
#include "HttpServerWebService.h"
#include "../Logs.h"
#include "../Configs/ConfigWebService.h"
#include "str_opr.h"
#include "StringOpr.h"
//#include "app/server/EasyMysqlConn.h"
//#include "app/common/md5_tools.h"
//#include "app/server/types.h"
//#include "tshome_typedef.h"
//#include "app/datagram/remote_def.h"
//#include "app/datagram/report_mgr.h"
//#include "app/common/config.h"
//#include "StringOpr.h"

//#include "../IpcServer/GeneralSocketProcessor.h"
#include "HandlerWebService.h"

//extern ts_udb_mgr g_udb_mgr;
//extern pj_bool_t g_run;
//extern int pj_runnable;
//#define SEND_BUFFER_SIZE 512
//extern pj_thread_t *httpSendThread;

/*pjsip data*/
//typedef struct send_to_client_data_t
//{
//    //pj_turn_srv *srv;                                       //pjsip server, real data sender
//    char snd_data[SEND_BUFFER_SIZE];                        //data
//    int snd_len;                                            //data length
//    char ip[48];                                               //destination ip
//    int port;                                               //destination port
//}
//send_to_client_data;
//
//CEZMutex g_mutexFor_send_data_buffer;
//std::vector<send_to_client_data> send_data_buffer;
//sem_t sem_sendto_client;


CHttpServerWebService::CHttpServerWebService(ISocketHandler& h) : HttpdSocket(h)
{
    m_strBody = "";
    m_requestPrefix = "";
    m_sessionId = "";
	//m_pPJ_TURN_SRV = NULL;
	//int ini2 = sem_init(&sem_sendto_client, 0, 0);
//    if(ini2 != 0)
//    {
//        LOG4CPLUS_ERROR(LOG_WEBSERVICES, "sem init failed");
//        exit(1);
//    }
}

void CHttpServerWebService::Init()
{
    if (GetParent() -> GetPort() == 443 || GetParent() -> GetPort() == 8443)
    {
        fprintf(stderr, "SSL not available\n");
    }

    LOG4CPLUS_INFO(LOG_WEBSERVICES, "CHttpServerWebService::On:" << GetRemoteAddress() << ":" << GetRemotePort());
}

CHttpServerWebService::~CHttpServerWebService()
{
    LOG4CPLUS_INFO(LOG_WEBSERVICES, "CHttpServerWebService::Destroy:" << GetRemoteAddress() << ":" << GetRemotePort());
}

void CHttpServerWebService::Exec()
{
    CreateHeader();
    GenerateDocument();
}
void CHttpServerWebService::OnHeaderComplete()
{
    m_strBody = "";
}

void CHttpServerWebService::OnUnknowRequest()
{
    //LOG4CPLUS_ERROR(LOG_WEBSERVICES, "CHttpServerWebService::OnUnknowRequest(), not a http req");

    //Reset(); // prepare for next request
    //SetCloseAndDelete();
}

void CHttpServerWebService::CreateHeader(size_t ContentLength, const std::string &matchRequest)
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

void CHttpServerWebService::GenerateDocument()
{
    std::string strSendBuf = "Hello CHttpServerWebService.\n\r";

    strSendBuf += GetHttpDate()+"\r\n";
    strSendBuf += "Your Body:"+m_strBody+"\n\r";

    strSendBuf = "[{\"result\":\"100\",\"reason\":\"CHttpServerWebService not support url:" + GetUrl() + "\"}]";

    //Send(strSendBuf);
    ReturnHttpMsg(strSendBuf);
}

/** Chunk of http body data recevied. */
void CHttpServerWebService::OnData(const char *p,size_t l)
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

void CHttpServerWebService::OnDataComplete()
{
    CConfigWebService __cfg;
    __cfg.update();

    //LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "m_strBody:" << m_strBody);

    if (m_strBody.size()<strlen("[{}]"))
    {
        LOG4CPLUS_ERROR(LOG_WEBSERVICES, "m_strBody(" << m_strBody.size() << ") should 3 or bigger");

        m_strBody =  "[{\"result\":\"101\",\"reason\":\"m_strBody.size() < 3\"}]";

        ReturnHttpMsg(m_strBody);

        //m_strBody = "";

        return;
    }

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

    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    __CTSHomeWebServiceIF.Parse(m_strBody);

    //CreateHeader();

    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Request URL:" << GetUrl());
	
	//在调用接口处理函数之前，先将公用资源初始化。
	//m_pPJ_TURN_SRV = static_cast<CHandlerWebService&>(Handler()).GetPJ_server();

    //exa:
    //	curl http://192.168.10.125:60088/WebService/TestIF -d "[{\"reason\":\"OK\",\"result\":\"0\"}]"
    // test interface
    if (GetUrl()=="/WebService/TestIF")
    {
        Do_TestIF(__CTSHomeWebServiceIF);
    }
    //认证接口
    //	http://218.95.39.172:60088/SmartHomeTelcom/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_requestAuth)
    {
        Do_requestAuth(__CTSHomeWebServiceIF);
    }//主机认证接口
    //	http://218.95.39.172:60088/SmartHomeTelcom/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_requestAuthByHost)
    {
        Do_requestAuthByHost(__CTSHomeWebServiceIF);
    }
    //5.2.6	心跳接口 getCtrlDeviceState
    //	http://192.168.10.216:60088/WebService/heartbeat
    else if (GetUrl()==__cfg.getConfig().strURL_heartbeat)
    {
        Do_heartbeat(__CTSHomeWebServiceIF);
    }
    //5.2.8	获取设备状态接口 getDeviceStateList
    //	http://218.95.39.172:60088/SmartHomeTelcom/getDeviceStateList
    //
    else if (GetUrl()==__cfg.getConfig().strURL_registerURL)
    {
        Do_registerURL(__CTSHomeWebServiceIF);
    }
    //5.2.4	请求配置上报接口
    //	http://218.95.39.172:60088/SmartHomeTelcom/requestReportDeviceConfigParam
    else if (GetUrl()==__cfg.getConfig().strURL_stopHostNetwork)
    {
        Do_stopHostNetwork(__CTSHomeWebServiceIF);
    }
    //5.2.6	获取网关状态接口 getCtrlDeviceState
    //	http://218.95.39.172:60088/SmartHomeTelcom/getCtrlDeviceState
    else if (GetUrl()==__cfg.getConfig().strURL_notifyUpdate)
    {
        Do_notifyUpdate(__CTSHomeWebServiceIF);
    }
    //5.2.8	获取设备状态接口 getDeviceStateList
    //	http://218.95.39.172:60088/SmartHomeTelcom/getDeviceStateList
    //
    else if (GetUrl()==__cfg.getConfig().strURL_getDeviceStateList)
    {
        Do_getDeviceStateList(__CTSHomeWebServiceIF);
    }
    //5.2.9	控制命令接口 controlDevice
    //	http://218.95.39.172:60088/SmartHomeTelcom/controlDevice
    else if (GetUrl()==__cfg.getConfig().strURL_controlDevice)
    {
        Do_controlDevice(__CTSHomeWebServiceIF);
    }
    else
    {
        GenerateDocument();
    }

    Reset(); // prepare for next request
    //SetCloseAndDelete();
    m_strBody = "";
}

int CHttpServerWebService::ReturnHttpMsg(const std::string &strMsg, const std::string &matchRequest)
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

std::string CHttpServerWebService::generateSessionId()
{
    char uuidBuff[37] = {0};
    uuid_t uuidGenerated;
    uuid_generate_random(uuidGenerated);
    uuid_unparse(uuidGenerated, uuidBuff);

    return uuidBuff;
}

std::string CHttpServerWebService::getMySessionId()
{
    return m_sessionId;
}

int CHttpServerWebService::Do_TestIF( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_TestIF");

    std::string strSendBuf;

    strSendBuf = "[{\"result\":\"0\",\"reason\":\"Do_TestIF, Your URL:" + GetUrl() + "\"}]";

    ReturnHttpMsg(strSendBuf);

    return 0;
}


//static int add_send_data(send_to_client_data &data)
//{
//    CEZLock guard(g_mutexFor_send_data_buffer);
//
//    send_data_buffer.push_back(data);
//    return TS_SUCCESS;
//}
//
//static int pop_first_send_data(send_to_client_data &data)
//{
//    CEZLock guard(g_mutexFor_send_data_buffer);
//    if(send_data_buffer.size() <= 0)
//    {
//        return TS_FAILED;
//    }
//    data = send_data_buffer[0];
//    send_data_buffer.erase(send_data_buffer.begin());
//    return TS_SUCCESS;
//}
//
//int sendto_client(void *data)
//{
//    //__fline;
//    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "start sendto_client");
//
//    while(pj_runnable)
//    {
//        sem_wait(&sem_sendto_client);
//
//        send_to_client_data send_data = {0};
//        int result = pop_first_send_data(send_data);
//        if(result == TS_SUCCESS)
//        {
//            //call the pjsip API to send data to client.
//            pj_status_t res = send_data_to_client(send_data.srv,
//                                                  (unsigned char *)send_data.snd_data,
//                                                  send_data.snd_len,
//                                                  //PJ_FALSE,
//                                                  send_data.ip,
//                                                  send_data.port);
//            if(res == PJ_SUCCESS)
//            {
//                LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "send_data_to_client "<<send_data.ip << ":"<<send_data.port);
//            }
//            else
//            {
//                LOG4CPLUS_ERROR(LOG_WEBSERVICES, "Send data error. send_data_to_client "<<send_data.ip << ":"<<send_data.port);
//            }
//        }
//        else
//        {
//            // impossble
//            LOG4CPLUS_ERROR(LOG_WEBSERVICES, "pop_first_send_data failed.");
//            usleep(50*1000);
//        }
//    }
//    pj_thread_join(httpSendThread);
//    pj_thread_destroy(httpSendThread);
//    return 1;
//}

//void CHttpServerWebService::SetPJ_turn_srv(pj_turn_srv *ppj_turn_srv)
//{
//    m_pPJ_TURN_SRV = ppj_turn_srv;
//    return;
//}
//
//pj_turn_srv * CHttpServerWebService::GetPJ_server()
//{
//    assert(m_pPJ_TURN_SRV);
//
//    return m_pPJ_TURN_SRV;
//}

// WEB子系统向连辅子系统注册接口地址的接口
int CHttpServerWebService::Do_registerURL( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_registerURL");
    CIPC_registerURL __op;
    JsonData.Decode(__op);
    __op.dump();
	m_sessionId = __op.sessionId;

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CIPC_registerURL_ACK Msg;

    if (static_cast<CHandlerWebService&>(Handler()).AuthSession(m_sessionId, EAW_REGISTERURL) ==0)
    {

        Msg.result = "0";
        Msg.reason = "Successful";
        m_requestPrefix = __op.baseURL;//TODO 作为全局变量存储该值
    }
    else
    {
        Msg.result = "1";
        Msg.reason = "Authentication failed";
    }
    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";


    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;
}
// WEB子系统认证接口
int CHttpServerWebService::Do_requestAuth( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_requestAuth");
    CIPC_requestAuth __op;
    JsonData.Decode(__op);
    __op.dump();

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CAuthResponse Msg;

    if(m_sessionId != "")
    {
        Msg.result = "1";
        Msg.reason = "Don't allow more internal connection";
        Msg.sessionId = "";
    }
    else
    {
        if(__op.authUser == "ipcServer" && __op.authPassword == "123456")
        {
            Msg.result = "0";
            Msg.reason = "Successful";
            m_sessionId = generateSessionId();//"hahahahaha";
            Msg.sessionId = m_sessionId;

            CSession __session;
            __session.m_strSession = m_sessionId;

            // 权限示例，赋权
            __session.m_vAccess.push_back(EAW_HEARTBEAT);
            __session.m_vAccess.push_back(EAW_NOTIFYUPDATE);
            __session.m_vAccess.push_back(EAW_STOPHOSTNET);
            __session.m_vAccess.push_back(EAW_REGISTERURL);
            __session.m_vAccess.push_back(EAW_CTRLDEVICE);
            //__session.m_ttLastRequestTime = time(NULL);

            static_cast<CHandlerWebService&>(Handler()).newSession(__session);
        }
        else
        {
            Msg.result = "1";
            Msg.reason = "UserName or password is wrong";
            Msg.sessionId = "";
        }
    }
    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";


    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;

}

// WEB子系统认证接口
#if 1
int CHttpServerWebService::Do_requestAuthByHost( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_requestAuth");
    CIPC_requestAuth __op;
    JsonData.Decode(__op);
    __op.dump();

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CAuthResponse Msg;

    if(m_sessionId != "")
    {
        Msg.result = "1";
        Msg.reason = "Don't allow more internal connection";
        Msg.sessionId = "";
    }
    else
    {
        if(__op.authUser == "ipcServer" && __op.authPassword == "123456")
        {
            Msg.result = "0";
            Msg.reason = "Successful";
            m_sessionId = generateSessionId();//"hahahahaha";
            Msg.sessionId = m_sessionId;

            CSession __session;
            __session.m_strSession = m_sessionId;

            // 权限示例，赋权
            __session.m_vAccess.push_back(EAW_HEARTBEAT);
            __session.m_vAccess.push_back(EAW_NOTIFYUPDATE);
            __session.m_vAccess.push_back(EAW_STOPHOSTNET);
            __session.m_vAccess.push_back(EAW_REGISTERURL);
            __session.m_vAccess.push_back(EAW_CTRLDEVICE);
            //__session.m_ttLastRequestTime = time(NULL);

            static_cast<CHandlerWebService&>(Handler()).newSession(__session);
        }
        else
        {
            Msg.result = "1";
            Msg.reason = "UserName or password is wrong";
            Msg.sessionId = "";
        }
    }
    __CTSHomeWebServiceIF.Encode(Msg);
    std::string strSnd = "["+__CTSHomeWebServiceIF.ToString()+"]";


    //Send(strSnd);
    ReturnHttpMsg(strSnd);
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "SendBack:" << strSnd);

    return 0;

}
#endif
// WEB子系统心跳接口
int CHttpServerWebService::Do_heartbeat( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_heartbeat");
    CIPC_heartbeat __op;
    JsonData.Decode(__op);
    __op.dump();
    m_sessionId = __op.sessionId;
	
	cout << "__op.sessionId = " << __op.sessionId << endl;

	// 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CIPC_heartbeat_ACK Msg;

    if (static_cast<CHandlerWebService&>(Handler()).AuthSession(m_sessionId, EAW_HEARTBEAT) ==0)
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

//停止主机外网服务接口
int CHttpServerWebService::Do_stopHostNetwork( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_stopHostNetwork");
    CIPC_stopHostNetwork __op;
    JsonData.Decode(__op);
    __op.dump();
	m_sessionId = __op.sessionId;

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_stopHostNetwork_ACK Msg;

    if(static_cast<CHandlerWebService&>(Handler()).AuthSession(m_sessionId, EAW_STOPHOSTNET) ==0)
    {
        
        Msg.result = "0";
        Msg.reason = "OK";
        //m_pPJ_TURN_SRV = static_cast<CHandlerWebService&>(Handler()).GetPJ_server();
        //assert(m_pPJ_TURN_SRV != NULL);
//        if(m_pPJ_TURN_SRV->core.is_running)
//        {
//            //int reportid = host_req->header.searilNum;
//            int homeid = atoi(__op.homeIdStr.c_str());
//            LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "homeid=%d"<<homeid);
//            ts_user *host_user = NULL;
//            //search the owner host user.
//            std::vector<ts_user> host_list;
//            int result = g_udb_mgr.home.get_hosts(homeid, host_list);
//            if(result == 0 && host_list.size() > 0)
//            {
//                // get the only one host user in this list.
//                host_user = &(host_list[0]);
//            }
//            if(NULL != host_user)
//            {
//                std::string username = __op.username;
//                //begin new a to_host_deny_connect_req_t object
//                host_deny_connect_req_t to_host;
//                memset(&to_host, 0, sizeof(host_deny_connect_req_t));
//                int body_len = sizeof(host_deny_connect_req_t)-sizeof(remote_header_t);
//                init_common_header(&to_host.header, 0, TSHOME_PROTOCOL_VERSION, body_len, STOP_CONNECTION, REQUEST, SERVER);
//
//                to_host.session_id = host_user->session_id;
//                to_host.report_id = 0;//需要有函数来为每个请求生成一个id标记，用于服务器收到主机响应时查找websocket
//                //end
//                u_char snd_buff[SEND_BUFFER_SIZE] = {0};
//                fill_stophostnet_req_to_host(snd_buff, &to_host);
//                //get a SDP object of the host user.
//                std::vector<user_sdp> sdps = host_user->sdpVec;
//
//                if(sdps.size() > 0)
//                {
//                    user_sdp sdp = sdps[0];
//
//                    //begin new a snd_to_client_data object
//                    send_to_client_data tc;
//                    memset(&tc, 0, sizeof(send_to_client_data));
//                    tc.srv = m_pPJ_TURN_SRV;
//                    tc.snd_len = sizeof(host_deny_connect_req_t);
//                    memcpy(tc.snd_data,&snd_buff, tc.snd_len);
//                    //tc.ip = sdp.addr;
//                    strcpy(tc.ip, sdp.addr);
//                    tc.port = sdp.port;
//                    //end
//
//                    add_send_data(tc);
//
//                    sem_post(&sem_sendto_client);
//                }
//                else
//                {
//                    Msg.result = "1";
//                    Msg.reason = "ERR_HOST(SDP)_LOSE";
//                }
//
//
//            }
//            else
//            {
//                //no host user, then responsed an ERR_HOST_UNLOGIN
//                Msg.result = "1";
//                Msg.reason = "ERR_HOST_UNLOGIN";
//            }
//        }
//        else
//        {//if the pjsip server stoped, this request will be unreachable.
//            Msg.result = "1";
//            Msg.reason = "The pjsip server stoped";
//        }


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
//推送配置更新接口
int CHttpServerWebService::Do_notifyUpdate( CTSHomeWebServiceIF &JsonData)
{
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "NOTIFY_UPDATE");
    CIPC_notifyUpdate __op;
    JsonData.Decode(__op);
    __op.dump();
	m_sessionId = __op.sessionId;

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_notifyUpdate_ACK Msg;
	//CHandlerWebService handler = dynamic_cast<CHandlerWebService&>(Handler());

    if(static_cast<CHandlerWebService&>(Handler()).AuthSession(m_sessionId, EAW_NOTIFYUPDATE) ==0)
    {
    	            Msg.result = "0";
            Msg.reason = "Success";

//        do
//        {
//            //m_pPJ_TURN_SRV = static_cast<CHandlerWebService&>(Handler()).GetPJ_server();
//            assert(m_pPJ_TURN_SRV != NULL);
//            if(!m_pPJ_TURN_SRV->core.is_running)
//            {
//                Msg.result = "1";
//                Msg.reason = "the pjsip server stoped, this request will be unreachable.";
//                LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "the pjsip server stoped, this request will be unreachable.");
//                break;
//            }
//    		LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "homeid = " << __op.homeIdStr);
//            int homeid = atoi(__op.homeIdStr.c_str());
//            //begin new an update_push_update_t object
//            update_issue_req_t push_update;
//            memset(&push_update, 0, sizeof(update_issue_req_t));
//            int body_len = sizeof(update_issue_req_t) - sizeof(remote_header_t);
//            init_common_header(&push_update.header, 0, TSHOME_PROTOCOL_VERSION, body_len, UPDATE_ISSUE,
//                               REQUEST, SERVER);
//            std::vector<ts_user> host_list;
//            int result = g_udb_mgr.home.get_hosts(homeid, host_list);
//            u_char snd_buff[SEND_BUFFER_SIZE] = {0};
//            if(result == 0 && host_list.size() > 0)
//            {
//                //iterator each host user.
//                size_t i = 0;
//                for(; i < host_list.size(); i++)
//                {
//                    push_update.session_id = host_list[i].session_id;
//                    fill_push_update(snd_buff, &push_update);
//                    //end fill datagram.
//
//
//                    std::vector<user_sdp> sdps = host_list[i].sdpVec;
//
//                    if(sdps.size() <= 0)
//                    {
//                        continue;
//                    }
//                    user_sdp sdp = sdps[0];
//
//                    //new a request to host client
//                    send_to_client_data tc;
//                    memset(&tc, 0, sizeof(send_to_client_data));
//                    tc.srv = m_pPJ_TURN_SRV;
//                    tc.snd_len = sizeof(update_issue_req_t);
//                    memcpy(tc.snd_data,snd_buff, tc.snd_len);
//                    strcpy(tc.ip, sdp.addr);
//                    tc.port = sdp.port;
//
//                    add_send_data(tc);
//
//                    sem_post(&sem_sendto_client);
//                    memset(snd_buff, 0, sizeof(snd_buff));
//
//                }
//
//            }
//            else
//            {
//                LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "no host user");
//            }
//
//            std::vector<ts_user> mobile_list;
//            result = g_udb_mgr.home.get_mobiles(homeid, mobile_list);
//            //iterator each mobile user.
//            if(result == 0 && mobile_list.size() > 0)
//            {
//                size_t i = 0;
//                for(; i < mobile_list.size(); i++)
//                {
//                    push_update.session_id = mobile_list[i].session_id;
//                    fill_push_update(snd_buff, &push_update);
//
//                    std::vector<user_sdp> sdps = mobile_list[i].sdpVec;
//
//                    if(sdps.size() <= 0)
//                    {
//                        continue;
//                    }
//                    user_sdp sdp = sdps[0];
//					
//                    send_to_client_data tc;
//                    memset(&tc, 0, sizeof(send_to_client_data));
//                    tc.srv = m_pPJ_TURN_SRV;
//                    tc.snd_len = sizeof(update_issue_req_t);
//                    memcpy(tc.snd_data,snd_buff, tc.snd_len);
//                    strcpy(tc.ip, sdp.addr);
//                    tc.port = sdp.port;
//
//                    add_send_data(tc);
//
//                    sem_post(&sem_sendto_client);
//					
//                    memset(snd_buff, 0, sizeof(snd_buff));
//                }
//            }
//            else
//            {
//                LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "no mobile user");
//            }
//            Msg.result = "0";
//            Msg.reason = "Success";
//        }
//        while(0);

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

int CHttpServerWebService::Do_getDeviceStateList( CTSHomeWebServiceIF &JsonData)
{
#if 0
    LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "Do_getDeviceStateList");

    CIPC_getDeviceStateList  __op;
    JsonData.Decode(__op);
    __op.dump();
	m_sessionId = __op.sessionId;
    // 开始使用数据

    // 构造返回消息
    CTSHomeWebServiceIF __CTSHomeWebServiceIF;

    CIPC_getDeviceStateList_ACK Msg;
    Msg.result = "0";
    Msg.reason = "OK";

    CIPC_getDeviceStateList_ACK_operate _operate;
    _operate.operate_id      = TEST_OPERATE_ID_SW;
    _operate.operate_ranage  = ""; // 开关类的此无需，务必空， 不得填值
    _operate.operate_type    = "0"; //开关
    _operate.operate_value   = "2"; // 关
    _operate.operate_explain = "大灯";
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


//发送设备操作报文
int CHttpServerWebService::sendDeviceCtrlMsg(char *ip,
        const int port,
        const char *operate_id,
        const char *value,
        const u_char value_len,
        const int32_t session_id)
{
//    device_control_req_enocean ctrl;
//    memset(&ctrl, 0, sizeof(device_control_req_enocean));
//    int body_len = sizeof(device_control_req_enocean) - sizeof(remote_header_t);
//    init_common_header(&ctrl.header, 0, TSHOME_PROTOCOL_VERSION, body_len, COMMON_DEVICE_CONTROL,
//                       REQUEST, SERVER);
//    ctrl.read_write= WRITE;
//    ctrl.data_len = value_len;
//
//    ctrl.data = atoi(value); //待修正后解注error: incompatible types in assignment of ??int?ˉ to ??unsigned char [0]?ˉ
//
//    ctrl.datapoint_id = htonl(atoi(operate_id));
//    ctrl.session_id = htonl(session_id);//add htonl 10.24
//
//	send_to_client_data tc;
//    memset(&tc, 0, sizeof(send_to_client_data));
//    tc.srv = static_cast<CHandlerWebService&>(Handler()).GetPJ_server();
//    tc.snd_len = sizeof(device_control_req_enocean);
//    memcpy(tc.snd_data, &ctrl, tc.snd_len);
//
//    strcpy(tc.ip, ip);
//    tc.port = port;
//
//    add_send_data(tc);
//
//    sem_post(&sem_sendto_client);
	
    return 0;
}

int CHttpServerWebService::Do_controlDevice(CTSHomeWebServiceIF &JsonData)
{
    CIPC_controlDevice   __op;
    CIPC_controlDevice_operate op;
    JsonData.Decode(__op);
    __op.dump();
	m_sessionId = __op.sessionId;
	
	/*
	  *构造返回消息，此接口只负责发送控制请求报文和回复错误信息。
	  *控制结果(响应消息)等收到来自主机的回复后再发送给web端。
	  *所以，该接口需要保存一个web通信对象和主机的配对关系，
	  *在收到主机的回复消息时，便于查询相关的web通信对象。
	  */
	CTSHomeWebServiceIF __CTSHomeWebServiceIF;
    CIPC_controlDevice_ACK Msg;

    if(static_cast<CHandlerWebService&>(Handler()).AuthSession(m_sessionId, EAW_CTRLDEVICE) ==0)
    {
//        int vecSize = __op.m_operate.size();
//        std::string homeIdStr = __op.homeIdStr;
//        int homeId = atoi(homeIdStr.c_str());
//        cout << "vecSize=" << vecSize << endl;
//        ts_user host_user;
//        user_sdp sdp;
//        //search the host user.
//        std::vector<ts_user> host_list;
//        int result = g_udb_mgr.home.get_hosts(homeId, host_list);
//        if(result == TS_SUCCESS && host_list.size() > 0)
//        {
//            // get the only one host user in this list.
//            host_user = host_list[0];
//            sdp = host_user.sdpVec[0];
//
//            for(int i = 0 ; i < vecSize ; i++)
//            {
//                op = __op.m_operate[i];
//
//                LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "NO." << i << ", op.dataPoint:"<< op.dataPoint<< ", op.dataLen="<<op.dataLen<<",op.data="<<op.data);
//                sendDeviceCtrlMsg(sdp.addr, sdp.port, op.dataPoint.c_str(), op.data.c_str(), atoi(op.dataLen.c_str()), host_user.session_id);
//				//CIPC_controlDevice_ACK_Operate ack_op;
//
//				//Msg.m_operate.push(ack_op);
//			}
			//根据前面所述逻辑，此时控制命令已发送完，
			//无需回复控制结果
			//立即返回方法
            Msg.result  = "0" ;
            Msg.reason  = "OK" ;
            return 0;
//        }
//        else
//        {
//            //主机不在线时，立即回复失败。
//            Msg.result= "1";
//            Msg.reason= "host_offline";
//            LOG4CPLUS_DEBUG(LOG_WEBSERVICES, "host offline");
//        }

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

