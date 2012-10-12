/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralSocketProcessor.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralSocketProcessor.cpp 5884 2012-07-18 02:51:38Z WuJunjie $
 *
 *  Explain:
 *     -单独的消息处理对象-
 *
 *  Update:
 *     2012-07-18 02:51:38  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "CommonInclude.h"
#include <stdio.h>
#include <Utility.h>
#include <pjlib.h>
#include <app/common/config.h>

#include "../Logs.h"

#include "GeneralAgent.h"

#include "GeneralSocketProcessor.h"

#include "iostream"
#include <vector>




extern ts_udb_mgr g_udb_mgr;
extern pj_bool_t g_run;
int pj_runnable;
static u_char snd_buff[512];
pj_thread_t *pt;

/*pjsip data*/
typedef struct snd_to_client_data_t
{
    pj_turn_srv *srv;                                       //pjsip server, real data sender
    char snd_data[sizeof(snd_buff)];                        //data
    int snd_len;                                            //data length
    char ip[48];                                               //destination ip
    int port;                                               //destination port
}
snd_to_client_data;

std::vector<snd_to_client_data> snd_data_buffer;
static pthread_mutex_t s_lock = PTHREAD_MUTEX_INITIALIZER;      //use in pt
//static pthread_cond_t s_cond = PTHREAD_COND_INITIALIZER;        //use in pt
sem_t sem_sndto_client;

static int add_snd_data(snd_to_client_data &data)
{
	snd_data_buffer.push_back(data);
	return TS_SUCCESS;
}

static int pop_first_snd_data(snd_to_client_data &data)
{
	if(snd_data_buffer.size() <= 0){
		return TS_FAILED;
	}
	data = snd_data_buffer[0];
	snd_data_buffer.erase(snd_data_buffer.begin());
	return TS_SUCCESS;
}



PATTERN_SINGLETON_IMPLEMENT(CGeneralSocketProcessor);
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// (const char*pName, int nPriority, int nMsgQueSize = 0, DWORD dwStackSize = 0)
//使用自己的队列, 原始队列不用
CGeneralSocketProcessor::CGeneralSocketProcessor() :CEZThread("CGeneralSocketProcessor", TP_DEFAULT/*, 100*/)
{
    printf("CGeneralSocketProcessor::CGeneralSocketProcessor()>>>>>>>>>\n");

    m_pSocketMsgQue = new CSocketMsgQue(0);// 0 - do not limit size

    int ini2 = sem_init(&sem_sndto_client, 0, 0);
    if(ini2 != 0)
    {
        printf("sem init failed \n");
        exit(1);
    }


    m_pPJ_TURN_SRV = NULL;

    memset(m_bufIPCBuffer, 0, sizeof(m_bufIPCBuffer));
}

CGeneralSocketProcessor::~CGeneralSocketProcessor()
{
    printf("CGeneralSocketProcessor Leave--------\n");

    if(m_pSocketMsgQue)
    {
        m_pSocketMsgQue->ClearMessage();
        delete m_pSocketMsgQue;
    }
}

BOOL CGeneralSocketProcessor::Start()
{
    if (m_bLoop)
    {
        return TRUE;
    }
    int ret = CreateThread();

    return ret;
}

BOOL CGeneralSocketProcessor::Stop()
{
    if(m_bLoop)
    {
        m_bLoop = FALSE;
        DestroyThread();
    }
    return TRUE;
}
#define MSG_TYPE_PRODUCER 100
#define MSG_TYPE_TIMER 101

void CGeneralSocketProcessor::ThreadProc()
{
    SOCKET_MSG_NODE msg;

    while (m_bLoop)
    {
        //__trip;
        if (FALSE == RecvSocketMessage(&msg, TRUE))
        {
            __trip ;
            sleep(1);
            continue;
        }

#if 0
        // 收到消息，在此加入处理代码
        // 下列代码打印
        {
            __fline;
            printf("CGeneralSocketProcessor::ThreadProc RecvSocketMessage\n");

            char buf[1024];
            struct sockaddr_in sa;
            memcpy((char *)&sa,&msg.sa_from,msg.sa_len);
            ipaddr_t a;
            memcpy(&a,&sa.sin_addr,4);
            std::string ip;
            Utility::l2ip(a,ip);

            snprintf(buf, sizeof(buf), "Received %d bytes from: %s:%d\n", msg.iLen,ip.c_str(),ntohs(sa.sin_port));
            printf(buf);
            if (msg.pData)
            {
                __fline;
                printf("%s\n",static_cast<std::string>(msg.pData).substr(0,msg.iLen).c_str());

                // 返回数据示例
                if (msg.pSocket)
                {
                    //sendto(msg.pSocket->GetSocket(), buf, strlen(buf), 0, sa_from, sa_len);
                    sendto(msg.pSocket->GetSocket(), static_cast<std::string>(msg.pData).substr(0,msg.iLen).c_str(), static_cast<std::string>(msg.pData).substr(0,msg.iLen).size(), 0, &msg.sa_from,msg.sa_len);
                }
            }
            else
            {
                __trip;
            }
            switch(msg.msg)
            {
                case MSG_TYPE_PRODUCER:
                {
                    printf("\nReceive msg from PRODUCER\n");
                    g_ThreadManager.DumpThreads();

                    break;
                }
                case MSG_TYPE_TIMER:
                {
                    printf("\nReceive msg from TIMER\n");

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
#endif
        {

            IpcHandler(
                msg.msg,
                msg.pSocket,
                msg.pData,
                msg.iLen,
                msg.sa_from,
                msg.sa_len
            );
        }


        // 这个对象申请的，故而这个对象删除
        if (msg.pData)
        {
            //__trip; // dbg
            delete []msg.pData;
        }

        // __trip;
    }
}

BOOL  CGeneralSocketProcessor::SendSocketMsg(unsigned int iMsg, Socket *pSocket, const char *pData,size_t lDataLen,struct sockaddr *sa_from/*=NULL*/,socklen_t sa_len/*=0*/)
{

    char *pBuff;
    if(pData&&lDataLen>0)
    {
        pBuff = new char[lDataLen];
        memcpy(pBuff, pData, lDataLen);
    }
    else
    {
        pBuff = NULL;
    }

    if(m_pSocketMsgQue)
    {
        return m_pSocketMsgQue->SendMessage( iMsg, pSocket, pBuff, lDataLen, sa_from, sa_len);
    }
    return FALSE;
}

int CGeneralSocketProcessor::Send2Webs(unsigned int	 iMsg, const char *pData, size_t lDataLen)
{
    return g_GeneralAgentTSIPC.Send2Socket(THE_UDPSVR_AGENT_NAME, pData, lDataLen);
}


BOOL  CGeneralSocketProcessor::RecvSocketMessage (SOCKET_MSG_NODE *pMsg, BOOL wait/* = TRUE*/)
{
    if(m_pSocketMsgQue)
    {
        return m_pSocketMsgQue->RecvMessage(pMsg, wait);
    }
    return FALSE;
}

int sndto_client(void *data)
{
//    snd_to_client_data *snd_data = NULL;
	

    //__fline;
    LOG4CPLUS_INFO(LOG_IPC, "start sndto_client");

    while(pj_runnable)
    {
        //__fline;
        //printf("sndto_client wait msg ...\n");
        sem_wait(&sem_sndto_client);
        //__fline;
        //printf("sndto_client get a msg\n");

        pthread_mutex_lock(&s_lock);

        //pop the oldest request.
		snd_to_client_data send_data = {0};
		int result = pop_first_snd_data(send_data);
		if(result == TS_SUCCESS)
        {
            //printf("in sndto_client: pjsrv=%p, _user->addr=%s, port=%d\n",snd_data->srv, snd_data->ip, snd_data->port);
            LOG4CPLUS_INFO(LOG_IPC, "send_data_to_client "<<send_data.ip << ":"<<send_data.port);
            //call the pjsip API to send data to client.
            send_data_to_client(send_data.srv,
                                (unsigned char *)send_data.snd_data,
                                send_data.snd_len,
                                send_data.ip,
                                send_data.port);
            //usleep(50*1000);
        }
        pthread_mutex_unlock(&s_lock);
    }
    pj_thread_join(pt);
    pj_thread_destroy(pt);
    return 1;
}

// 发送数据
int CGeneralSocketProcessor::Send2(Socket *pSocket, const char *pData, size_t lDataLen,struct sockaddr *sa_from,socklen_t sa_len)
{
    return sendto(pSocket->GetSocket(), pData, lDataLen, 0, sa_from, sa_len);
}

int CGeneralSocketProcessor::IpcHandler(unsigned int	 iMsg,Socket *pSocket, const char *pData,size_t lDataLen,struct sockaddr sa_from,socklen_t sa_len)
{
    // dbg code, echo data only
    //return Send2Webs(iMsg, pData, lDataLen);

    if (lDataLen<sizeof(IPC_HEADER))
    {
        LOG4CPLUS_ERROR(LOG_IPC, "lDataLen " << lDataLen << "<" << sizeof(IPC_HEADER));
        return -100;
    }

    size_t send = 0;
    int body_len = 0;
    pj_turn_srv *pj_srv = NULL;
    int fun_code = UNKNOWN;
    IPC_HEADER *header = NULL;
    // 少量内存直接用栈空间
    void *structs = &m_bufIPCBuffer;//calloc(IPC_DATA_MAX_SIZE, sizeof(char));

    //if(NULL == structs){
    //	goto invalid;
    //}

    pj_srv = GetPJ_server();

    //parse the protocol data
    fun_code = m_pHandler.incoming_handler(m_pHandler.aes_ins, (u_char *)pData, &structs);

    header = (IPC_HEADER *)structs;
    if(header->protocol != 0x01)
    {
        LOG4CPLUS_ERROR(LOG_IPC, "invalid header->protocol:" << header->protocol);
        return -101;
    }

    body_len = ntohs(header->length);
    LOG4CPLUS_INFO(LOG_IPC, "IpcHandler body_len " << body_len << " fun_code:"<< fun_code );
    switch(fun_code)
    {
        case IPC_HEARTBEAT:
        {
            LOG4CPLUS_INFO(LOG_IPC, "IPC_HEARTBEAT");

            if(header->type == IPC_REQUEST)
            {
                //response a heartbeat
                IPC_DG_HEART * heart = (IPC_DG_HEART *)structs;
                heart->header.type = IPC_RESPONSE;
                send = sizeof(IPC_DG_HEART);
                int result = Send2(pSocket, (const char *)heart, send, &sa_from, sa_len);

                if (result == -1) //send error
                {
                    perror("send on stream error");
                    LOG4CPLUS_ERROR(LOG_IPC, "send on stream error ");
                }else
                {
                    //ipc_srv->status = IPC_SERVER_RUN;// now the ipc is already connected to web server
                    //printf("%d bytes sended\n", result);
                    LOG4CPLUS_INFO(LOG_IPC, "Send2: " << result << " bytes");
                }
            }
            break;
        }
        case HOST_SERV_REQ:
        {
            printf("in HOST_SERV_REQ case:\n");
            IPC_DG_HOST_SERV *host_req = (IPC_DG_HOST_SERV *)structs;
            if(header->type == IPC_REQUEST)
            {
                if(!pj_srv->core.is_running)
                {//if the pjsip server stoped, this request will be unreachable.
                    PJ_LOG(4,(__FILE__, "the pjsip server stoped, this request will be unreachable."));
                    break;
                }
                int homeid = host_req->homeId;
                int reportid = host_req->header.searilNum;
                ts_user *host_user = NULL;
                //search the owner host user.
                std::vector<ts_user> host_list;
				int result = g_udb_mgr.home.get_hosts(homeid, host_list);
                if(result == 0 && host_list.size() > 0)
                {
                    // get the only one host user in this list.
                    host_user = &(host_list[0]);
                }
                printf("in HOST_SERV_REQ case:1,homeid=%d\n",homeid);
                if(NULL != host_user)
                {
                    //char *username = host_req->userName;
                    int cmd_state = host_req->cmd_state;
                    if(cmd_state == REQ_STOP_HOST)
                    {
                        //begin new a to_host_deny_connect_req_t object
                        host_deny_connect_req_t to_host;
                        memset(&to_host, 0, sizeof(to_host_deny_connect_req_t));
                        body_len = sizeof(to_host_deny_connect_req_t)-sizeof(remote_header_t);
                        init_common_header(&to_host.header, 0, TSHOME_PROTOCOL_VERSION, body_len, STOP_CONNECTION, REQUEST, SERVER);
                        printf("in HOST_SERV_REQ case:2\n");
                        to_host.session_id = host_user->session_id;
                        to_host.report_id = reportid;
                        //end
                        fill_stophostnet_req_to_host(snd_buff, &to_host);
                        printf("in HOST_SERV_REQ case:3\n");
                        //get a SDP object of the host user.
						std::vector<user_sdp> sdps = host_user->sdpVec;

                        if(sdps.size() <= 0)
                        {
                            break;
                        }
						user_sdp sdp = sdps[0];
                        //begin new a snd_to_client_data object
                        snd_to_client_data tc;
                        memset(&tc, 0, sizeof(snd_to_client_data));
                        tc.srv = pj_srv;
                        tc.snd_len = sizeof(to_host_deny_connect_req_t);
                        memcpy(tc.snd_data,&snd_buff, tc.snd_len);
                        //tc.ip = sdp.addr;
						strcpy(tc.ip, sdp.addr);
						tc.port = sdp.port;
                        //end

                        pthread_mutex_lock(&s_lock);
                        printf("in HOST_SERV_REQ case:5\n");
                        add_snd_data(tc);
                        printf("in HOST_SERV_REQ case:6\n");
                        //if(snd_data_buff.used_size == 1){
                        //	pthread_cond_signal(&s_cond);
                        //}
                        pthread_mutex_unlock(&s_lock);

                        sem_post(&sem_sndto_client);
                        //send over
                        memset(snd_buff, 0, sizeof(snd_buff));
                        printf("end HOST_SERV_REQ case:\n");
                        //goto invalid;
                        return 0;
                    }
                    else if(cmd_state == REQ_STOP_ASK)
                    {
                        //TODO
                        //goto invalid;
                        return 0;
                    }
                    else
                    {
                        //goto invalid;
                        return 0;
                    }
                }
                else
                {
                    //no host user, then responsed an ERR_HOST_UNLOGIN
                    IPC_DG_ERR err;
                    memset(&err, 0, sizeof(IPC_DG_ERR));
                    memcpy(&err.header, header, sizeof(IPC_HEADER));
                    err.header.type = IPC_RESPONSE;
                    err.header.function = GLOBAL_ERROR;
                    err.header.length = htons(sizeof(IPC_DG_ERR) - sizeof(IPC_HEADER));
                    err.err = ERR_HOST_UNLOGIN;//htons(ERR_HOST_UNLOGIN);
                    memset(err.space, 0, sizeof(err.space));
                    send = sizeof(IPC_DG_ERR);
                    int result = Send2(pSocket, (const char *)&err, send, &sa_from, sa_len);

                    if (result < 0) //send error
                    {
                        perror("send on stream error");
                        LOG4CPLUS_ERROR(LOG_IPC, "send on stream error");
                    }
                    //PJ_LOG(4,(__FILE__, "no host user"));
                    LOG4CPLUS_ERROR(LOG_IPC, "no host user");
                    //goto invalid;
                    return 0;
                }
            }
            else
            {
                //goto invalid;
                return 0;
            }
            break;
        }
        case NOTIFY_UPDATE:
        {
            //printf("in NOTIFY_UPDATE case:\n");
            LOG4CPLUS_INFO(LOG_IPC, "NOTIFY_UPDATE");
            int step = 0;
            IPC_DG_NOTIFY_UPDATE *notify_req = (IPC_DG_NOTIFY_UPDATE *)structs;
            if(header->type == IPC_REQUEST)
            {
                if(!pj_srv->core.is_running)
                {
                    PJ_LOG(4,(__FILE__, "the pjsip server stoped, this request will be unreachable."));
                    break;
                }
                int homeid = notify_req->homeId;
                //begin new an update_push_update_t object
                update_issue_req_t push_update;
                memset(&push_update, 0, sizeof(update_issue_req_t));
                body_len = sizeof(update_issue_req_t) - sizeof(remote_header_t);
                init_common_header(&push_update.header, 0, TSHOME_PROTOCOL_VERSION, body_len, UPDATE_ISSUE,
                                   REQUEST, SERVER);
                printf("in NOTIFY_UPDATE case:step=%d, homeid=%d\n",step++, homeid);
                std::vector<ts_user> host_list;
				int result = g_udb_mgr.home.get_hosts(homeid, host_list);
                if(result == 0 && host_list.size() > 0)
                {
                    //iterator each host user.
					size_t i = 0;
					for(; i < host_list.size(); i++)
                    {
                        push_update.session_id = host_list[i].session_id;
                        fill_push_update(snd_buff, &push_update);
                        //end fill datagram.


                        std::vector<user_sdp> sdps = host_list[i].sdpVec;

                        if(sdps.size() <= 0)
                        {
                            continue;
                        }
						user_sdp sdp = sdps[0];
                        //new a request to host client
                        snd_to_client_data tc;
                        memset(&tc, 0, sizeof(snd_to_client_data));
                        tc.srv = pj_srv;
                        tc.snd_len = sizeof(update_issue_req_t);
                        memcpy(tc.snd_data,snd_buff, tc.snd_len);
						strcpy(tc.ip, sdp.addr);
						//tc.ip = sdp.addr;
                        tc.port = sdp.port;

                        pthread_mutex_lock(&s_lock);
                        add_snd_data(tc);
                        //if(snd_data_buff.used_size == 1){
                        //	pthread_cond_signal(&s_cond);
                        //}
                        pthread_mutex_unlock(&s_lock);
                        sem_post(&sem_sndto_client);
                        memset(snd_buff, 0, sizeof(snd_buff));

                    }

                }
                else
                {
                    PJ_LOG(4,(__FILE__, "in NOTIFY_UPDATE case: no host user"));
                }

                printf("in NOTIFY_UPDATE case: step=%d\n",step++);
                std::vector<ts_user> mobile_list;
				result = g_udb_mgr.home.get_mobiles(homeid, mobile_list);
                //iterator each mobile user.
                if(result == 0 && mobile_list.size() > 0)
                {
                	size_t i = 0;
					for(; i < mobile_list.size(); i++)
                    {
                        push_update.session_id = mobile_list[i].session_id;
                        fill_push_update(snd_buff, &push_update);

                        std::vector<user_sdp> sdps = mobile_list[i].sdpVec;

                        if(sdps.size() <= 0)
                        {
                            continue;
                        }
						user_sdp sdp = sdps[0];
                        snd_to_client_data tc;
                        memset(&tc, 0, sizeof(snd_to_client_data));
                        tc.srv = pj_srv;
                        tc.snd_len = sizeof(update_issue_req_t);
                        memcpy(tc.snd_data,snd_buff, tc.snd_len);
                        //tc.ip = sdp.addr;
                        strcpy(tc.ip, sdp.addr);
                        tc.port = sdp.port;

                        pthread_mutex_lock(&s_lock);
						add_snd_data(tc);
                        //if(snd_data_buff.used_size == 1){
                        //	pthread_cond_signal(&s_cond);
                        //}
                        pthread_mutex_unlock(&s_lock);
                        sem_post(&sem_sndto_client);
                        memset(snd_buff, 0, sizeof(snd_buff));
                    }
                }
                else
                {
                    PJ_LOG(4,(__FILE__, "in NOTIFY_UPDATE case: no mobile user"));
                }
                printf("in NOTIFY_UPDATE case:step=%d\n",step++);
            }
            break;
        }
        case CHECK_UPDATE:
        {
            //printf("in DOWN_CONFIG\n");
            LOG4CPLUS_INFO(LOG_IPC, "CHECK_UPDATE");

            IPC_DG_CHECK_UPDATE_RES *down_res = (IPC_DG_CHECK_UPDATE_RES *)structs;
            if(header->type == IPC_RESPONSE)
            {
                int homeid = down_res->homeId;
                int sessionid = down_res->sessionId;
                LOG4CPLUS_INFO(LOG_IPC, "target user: homeId = "<< homeid << ",sessionId = " << sessionid);

                ts_user *pUserFinded = NULL;
                std::vector<ts_user> user_list;
				int result = g_udb_mgr.home.get_hosts(homeid, user_list);
                if(result == 0 && user_list.size() > 0)
                {
                	size_t i = 0;
					for(; i < user_list.size(); i++)
                    {
                    	
                        if(user_list[i].session_id == sessionid)
                        {
                            //printf("in DOWN_CONFIG 21, user->userName=%s,user->pwd=%s,user->sessionid=%d,user->homeid=%d\n",user->username,user->pwd,user->session_id,user->home_id);
                            //goto find_ok;
                            pUserFinded = &(user_list[i]);
                            break;
                        }
                    }
                }
                //the user is not a host user, but maybe a mobile user.
                result = g_udb_mgr.home.get_mobiles(homeid, user_list);
                if(result == 0 && pUserFinded==NULL /*not find in get_hosts*/&& user_list.size() > 0)
                {
                    size_t i = 0;
					for(; i < user_list.size(); i++)
                    {
                    	
                        if(user_list[i].session_id == sessionid)
                        {
                            //printf("in DOWN_CONFIG 21, user->userName=%s,user->pwd=%s,user->sessionid=%d,user->homeid=%d\n",user->username,user->pwd,user->session_id,user->home_id);
                            //goto find_ok;
                            pUserFinded = &(user_list[i]);
                            break;
                        }
                    }
                }
                //the user is invalid
                //user = NULL;
                //LOG4CPLUS_ERROR(LOG_IPC, "user is invalid.");
                //break;

                //find_ok:
                if (pUserFinded)
                {
                    //printf("in DOWN_CONFIG 31,\n");
                    //new check_update_res_t
                    update_res_t check_res;
                    memset(&check_res, 0, sizeof(update_res_t));
                    body_len = sizeof(update_res_t) - sizeof(remote_header_t);
                    init_common_header(&check_res.header, 0, TSHOME_PROTOCOL_VERSION, body_len, UPDATE_QUERY,
                                       RESPONSE, SERVER);
                    check_res.query_from = down_res->query_from;
                    check_res.conf_upd_mode = down_res->cfg_force;
                    check_res.conf_url_len = down_res->cfg_url_len;
                    memcpy(check_res.conf_url, down_res->cfg_url, check_res.conf_url_len);

                    check_res.file_upd_mode = down_res->soft_force;
                    check_res.file_url_len = down_res->soft_url_len;
                    memcpy(check_res.file_url, down_res->soft_url, check_res.file_url_len);

                    memcpy(check_res.md5, down_res->soft_md5, sizeof(check_res.md5));

                    check_res.session_id = pUserFinded->session_id;
                    //end new
                    aes_encode aes_ins2;
                    memset(&aes_ins2, 0, sizeof(aes_encode));
                    at_create_key_by_string((const u_char *)pUserFinded->aeskey, &aes_ins2);
                    fill_updateconfig_res(snd_buff, &check_res, aes_ins2);

                    //printf("in DOWN_CONFIG 40,\n");

                   	std::vector<user_sdp> sdps = pUserFinded->sdpVec;

                    if(sdps.size() <= 0)
                    {
                        break;
                    }
					user_sdp sdp = sdps[0];

                    //printf("in DOWN_CONFIG 44,\n");
                    snd_to_client_data tc;
                    memset(&tc, 0, sizeof(snd_to_client_data));
                    tc.srv = pj_srv;
                    tc.snd_len = sizeof(update_res_t);
                    memcpy(tc.snd_data,(const char *)snd_buff, tc.snd_len);
                    //printf("in DOWN_CONFIG 5,\n");
					strcpy(tc.ip, sdp.addr);
					//tc.ip = sdp.addr;
                    tc.port = sdp.port;

                    pthread_mutex_lock(&s_lock);
					add_snd_data(tc);
					//printf("in DOWN_CONFIG 6,\n");
                    //if(snd_data_buff.used_size == 1){
                    //	pthread_cond_signal(&s_cond);
                    //}
                    pthread_mutex_unlock(&s_lock);
                    sem_post(&sem_sndto_client);
                    //	}
                    memset(snd_buff, 0, sizeof(snd_buff));
                    //printf("in DOWN_CONFIG 7,\n");
                }
                else
                {
                    LOG4CPLUS_INFO(LOG_IPC, "sessionid not found : "<< sessionid);
                }
            }
            break;
        }
        case ASK_UDPSYS_STATES:
        {
            LOG4CPLUS_INFO(LOG_IPC, "ASK_UDPSYS_STATES");

            IPC_DG_ASK_UDPSYS_REQ *ask_req = (IPC_DG_ASK_UDPSYS_REQ *)structs;
            if(header->type == IPC_REQUEST)
            {
                unsigned int m_num = 0;		//mobile and host numbers.
                unsigned int port_num = 0;	//used port numbers.
                unsigned int thread_num = 0;	//started thread numbers.
                unsigned int service = 0x00;	//pjsip server service status.

                if(pj_srv->core.is_running)
                {
                    m_num = pj_hash_count(pj_srv->tables.alloc);
                    port_num = pj_srv->ports.next_udp - pj_srv->ports.min_udp + 1;
                    thread_num = pj_srv->core.thread_cnt;
                    service = 0x01;
                }
                int len_ = sizeof(IPC_DG_ASK_UDPSYS_RES) - sizeof(IPC_HEADER);

                IPC_DG_ASK_UDPSYS_RES ask_res;
                memset(&ask_res, 0, sizeof(IPC_DG_ASK_UDPSYS_RES));
                memcpy(&ask_res.header, &ask_req->header, sizeof(IPC_HEADER));
                ask_res.header.type = IPC_RESPONSE;
                ask_res.header.length = htons(len_);
                ask_res.service = service;
                ask_res.heart = 0x01;
                //pj_gettimeofday(&now);
                //long runned_time = (now.sec - start.sec) * 1000;
                //ask_res.runnedTime = runned_time;
                ask_res.m_num = m_num;
                ask_res.port_num = port_num + 1;
                ask_res.thread_num = thread_num + 5;

                int result = m_pHandler.outgoing_handler(m_pHandler.aes_ins, snd_buff, &ask_res, ASK_UDPSYS_STATES);
                if(result)
                {
                    send = sizeof(ask_res);
                    int result = Send2(pSocket, (const char *)snd_buff, send, &sa_from, sa_len);

                    if (result < 0) //send error
                    {
                        //perror("send on stream error");
                        LOG4CPLUS_ERROR(LOG_IPC, "send on stream error");
                    }
                }
                memset(snd_buff, 0, sizeof(snd_buff));
            }
            break;
        }
        case CTRL_UDPSYS:
        {
            LOG4CPLUS_INFO(LOG_IPC, "CTRL_UDPSYS");
#if 1			
            LOG4CPLUS_INFO(LOG_IPC, "not support, break now ...");
            break;
#else
            IPC_DG_CTRL_UDPSYS *ctrl = (IPC_DG_CTRL_UDPSYS *)structs;
            if(header->type == IPC_REQUEST)
            {
                int cmd = ctrl->cmd;
                if(cmd == STOP_UDPSYS)
                {
                    //PJ_LOG(3,(__FILE__, "in CTRL_UDPSYS case: stop"));
                    LOG4CPLUS_INFO(LOG_IPC, "cmd == STOP_UDPSYS");
                    pj_srv->core.quit = PJ_TRUE;		//stop the pjsip server.
                    g_run = PJ_FALSE;		//stop the pjsip server.
                    pj_runnable = TS_FALSE;	//stop the pt thread.
                    //end stop pt.
                    g_GeneralSocketProcessor.SetPJ_turn_srv(NULL);
                }
                else if(cmd == START_UDPSYS)
                {
                    //PJ_LOG(3,(__FILE__, "in CTRL_UDPSYS case: start"));
                    LOG4CPLUS_INFO(LOG_IPC, "cmd == START_UDPSYS");
                    if(pj_srv != NULL && !pj_srv->core.is_running)
                    {		//if pjsip server is not running, handle thie request.
                        pj_srv = NULL;
                        void *arg = (void *)pj_srv;
                        int i = ts_pj_server_start(&arg);
                        pj_srv = (pj_turn_srv *)arg;
                        if(i == TS_SUCCESS && NULL != pj_srv)
                        {
                            pj_runnable = TS_TRUE;
                            //start a new pjsip thread to work
                            pj_thread_create(pj_srv->core.pool, pj_srv->obj_name, &sndto_client,
                                             pj_srv, 0, 0, &pt);
                        }
                        g_GeneralSocketProcessor.SetPJ_turn_srv(pj_srv);
                    }
                }
                else if(cmd == RESTART_UDPSYS)
                {
                    //PJ_LOG(3,(__FILE__, "in CTRL_UDPSYS case: restart"));
                    LOG4CPLUS_INFO(LOG_IPC, "cmd == RESTART_UDPSYS");
                    pj_runnable = TS_FALSE;	//stop the pt thread.
                    pj_srv->core.quit = PJ_TRUE;		//stop the pjsip server.
                    g_run = PJ_FALSE;
                    while(pj_srv->core.is_running)
                    {
                        sleep(1);
                    }//until stoped.
                    pj_srv = NULL;
                    void *arg = (void *)pj_srv;
                    //create and start a new pjsip server.
                    int i = ts_pj_server_start(&arg);
                    pj_srv = (pj_turn_srv *)arg;
                    if(i == TS_SUCCESS && NULL != pj_srv)
                    {
                        pj_runnable = TS_TRUE;
                        pj_thread_create(pj_srv->core.pool, pj_srv->obj_name, &sndto_client,
                                         pj_srv, 0, 0, &pt);
                    }
                    else
                    {
                        //PJ_LOG(4,(__FILE__, "in CTRL_UDPSYS case: restart failed"));
                        LOG4CPLUS_INFO(LOG_IPC, "cmd == RESTART_UDPSYS, restart failed");
                    }

                    g_GeneralSocketProcessor.SetPJ_turn_srv(pj_srv);
                }
                else if(cmd == OPEN_HEART)
                {
                    //do nothing
                }
                else if(cmd == CLOSE_HEART)
                {
                    //do nothing
                }
            }
            break;
#endif
        }
        case INFRARED_LEARN:
        {
            //printf("in INFRARED_LEARN case:\n");
            LOG4CPLUS_INFO(LOG_IPC, "INFRARED_LEARN");
            IPC_DG_INF_LEARN *learn_req = (IPC_DG_INF_LEARN *)structs;
            if(header->type == IPC_REQUEST)
            {
                if(!pj_srv->core.is_running)
                {
                    break;
                }
                int homeid = learn_req->homeId;
                int reportid = learn_req->header.searilNum;
                ts_user *host_user = NULL;
                std::vector<ts_user> host_list;
				int result = g_udb_mgr.home.get_hosts(homeid, host_list);
                //printf("in INFRARED_LEARN case: 0 homeId=%d\n",homeid);
                LOG4CPLUS_INFO(LOG_IPC, "handle INFRARED_LEARN homeid:" << homeid);
                if(result == 0 && host_list.size() > 0)
                {
                    //at present, one home just have one host
                    host_user = &(host_list[0]);
                    printf("in INFRARED_LEARN case: found host user\n");
                }
                if(NULL == host_user)
                {
                    IPC_HEADER err_header = learn_req->header;
                    err_header.length = htons(2);
                    err_header.function = GLOBAL_ERROR;
                    IPC_DG_ERR err;
                    memcpy(&err.header, &err_header, sizeof(IPC_HEADER));
                    err.err = ERR_HOST_UNLOGIN;//htons(ERR_HOST_UNLOGIN);
                    send = sizeof(err);
                    int result = Send2(pSocket, (const char *)&err, send, &sa_from, sa_len);

                    if (result < 0) //send error
                    {
                        perror("send on stream error");
                        LOG4CPLUS_ERROR(LOG_IPC, "Send2 failed:" << result);
                    }

                    break;
                }
                printf("in INFRARED_LEARN case: 1,user=%p, sername=%s\n",host_user, host_user->username);
                _ir_study_req study_req;
                memset(&study_req, 0, sizeof(ir_study_req_t));
                body_len = sizeof(_ir_study_req) - sizeof(remote_header_t);
                init_common_header(&study_req.kic_head.header, 0, TSHOME_PROTOCOL_VERSION, body_len, DEVICE_OPERATION,
                                   REQUEST, SERVER);
                study_req.kic_head.session_id = host_user->session_id;
                study_req.kic_head.home_device_id= reportid;
                study_req.kic_head.device_type = IR_STUDY;
                study_req.gateway_id = learn_req->gateway_id;
                study_req.channel = learn_req->channel;
                study_req.function_code = learn_req->key;
                study_req.space = 0;

                std::vector<user_sdp> sdps = host_user->sdpVec;

                if(sdps.size() <= 0)
                {
                    break;
                }
				user_sdp sdp = sdps[0];
				
                printf("in INFRARED_LEARN case: 2\n");
                fill_wifictrl_req(snd_buff, &study_req);
                printf("in INFRARED_LEARN case: 3 pjsrv=%p, host_user->addr=%s, port=%d\n",pj_srv, sdp.addr, sdp.port);
                snd_to_client_data tc;
                memset(&tc, 0, sizeof(snd_to_client_data));
                tc.srv = pj_srv;
                tc.snd_len = sizeof(ir_study_req_t);
                memcpy(tc.snd_data,snd_buff, tc.snd_len);
                //tc.ip = sdp.addr;
                strcpy(tc.ip, sdp.addr);
                tc.port = sdp.port;
                //if(pthread_mutex_trylock(&s_lock) == EBUSY)
                pthread_mutex_lock(&s_lock);
				add_snd_data(tc);
				//if(snd_data_buff.used_size == 1){
                //	pthread_cond_signal(&s_cond);
                //}
                pthread_mutex_unlock(&s_lock);
                sem_post(&sem_sndto_client);
                memset(snd_buff, 0, sizeof(snd_buff));
            }
            break;
        }
        default:
        break;
    }

    LOG4CPLUS_INFO(LOG_IPC, "HANDLE END");

    return 0;
}

void CGeneralSocketProcessor::SetPJ_turn_srv(pj_turn_srv *ppj_turn_srv)
{
    m_pPJ_TURN_SRV = ppj_turn_srv;
    return;
}

pj_turn_srv * CGeneralSocketProcessor::GetPJ_server()
{
    assert(m_pPJ_TURN_SRV);

    return m_pPJ_TURN_SRV;
}

ipc_handler * CGeneralSocketProcessor::GetIPC_hander()
{
    return &m_pHandler;
}
int CGeneralSocketProcessor::sendDeviceCtrl(char *ip,
												const int port,
												device_control_req *ctrl)
{

	printf("ctrl:cmd_type=%d,data=%d, body_len=%d\n", ctrl->read_write, ctrl->data, ctrl->header.length);
    snd_to_client_data tc;
    memset(&tc, 0, sizeof(snd_to_client_data));
    tc.srv = GetPJ_server();
    tc.snd_len = sizeof(device_control_req);
	printf("tc:snd_len=%d\n", tc.snd_len);	
    memcpy(tc.snd_data, ctrl, tc.snd_len);

    //tc.ip = ip;
    strcpy(tc.ip, ip);
    tc.port = port;

    pthread_mutex_lock(&s_lock);
	add_snd_data(tc);

    pthread_mutex_unlock(&s_lock);
    sem_post(&sem_sndto_client);
    return 0;
}

