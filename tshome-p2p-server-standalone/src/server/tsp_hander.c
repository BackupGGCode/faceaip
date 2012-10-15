/*
 * tsp_handler.c
 *
 *  Created on: 2012-2-18
 *      Author: sunzq
 */
#include <app/common/config.h>

#include "CommonInclude.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mysql/mysql.h>
#include <pthread.h>
#include <pjlib.h>
#include "app.h"
#include "EZThread.h"
#include "../IpcServer/GeneralSocketProcessor.h"

#include "string.h"
#include "str_opr.h"
#include "../Logs.h"

extern ts_udb_mgr g_udb_mgr;

static tsp_handler *this_tsp_handler = NULL;

// ��ǣ�ע������������ض���Ϊ�ַ�������������strlenȡ����
const char * const hostToken = "SDPH";		// ���������������
const char * const clientTokenWan = "SDPL";// ���� �ն������������
const char * const clientTokenLan = "SDPIL";// ����ģʽ���ն�ֱ������������
// ��С���ݰ�����
const size_t MIN_PACKET_LEN = 7;
        
CEZMutex g_mutexCommunicationresponse;
CEZMutex g_mutexTspHandlerDestroy;
CEZMutex g_mutexTspHandlerLogout;
CEZMutex g_mutexTspHandlerRequest;

// ���󣬷�ֹԽ��
static u_char send_buff[1500];		//send to web IPC server.

//for log
//static char *method;					//current method name,used for logging.
static char msg[1024];					//message for logging.

/**************prototype**************/
static void set_rsa(int field, void *rsa);

static RSA * init_rsa();

static int handle_remote_request(pj_turn_srv *srv,
                                 const char *userName,
                                 const unsigned int un_len,
                                 const pj_uint8_t *rcv_buff,
                                 const unsigned int rcv_len,
                                 const char *addr,
                                 const unsigned int addr_len,
                                 const unsigned int port,
                                 pj_uint8_t *snd_buff,
                                 unsigned int *snd_len,
                                 user_sdp *sdp);

static int handle_remote_response(pj_turn_srv *srv,
                                  const unsigned char *rcv_buff,
                                  const unsigned int rcv_len);

static int handle_remote_timeout(pj_turn_srv *srv,
                                 const char *username,
                                 const unsigned int un_len,
                                 const char *ip,
                                 const int ip_len,
                                 const int port,
                                 user_sdp *sdp);

static int handle_remote_destroy(pj_turn_srv *srv,
                                 const char *username,
                                 const unsigned int un_len,
                                 const char *ip,
                                 const int ip_len,
                                 const int port,
                                 user_sdp *sdp);


/********************public API********************/

pj_status_t tsp_handler_init(pj_turn_srv *pj_srv)
{
    assert(pj_srv != NULL);
    pj_srv->core.handler.rsa = init_rsa();
    //set callbacks
    pj_srv->core.handler.handle_request = &handle_remote_request;
    pj_srv->core.handler.handle_response = &handle_remote_response;
    pj_srv->core.handler.handle_timeout = &handle_remote_timeout;
    pj_srv->core.handler.handle_destroy = &handle_remote_destroy;
    pj_srv->core.handler.update = &set_rsa;

    this_tsp_handler = &pj_srv->core.handler;
    return PJ_SUCCESS;
}

/*
set a RSA instance
*/
static void set_rsa(int field, void *r)
{
    (void)field;
    this_tsp_handler->rsa = (RSA *)r;
}

/*
get the current RSA instance.
*/
RSA * init_rsa()
{
    RSA *rsa = NULL;
    rsa = rt_create_rsa();
    return rsa;
}

// if return TS_INVALID, not login, otherwise logined
static int has_login(const int sessionId)
{
    return g_udb_mgr.mapping.get_home_id(sessionId);
}
/*
fill error response
*/
static void fill_error_res(u_char *snd_buff, unsigned int *snd_len, unsigned short fun_code,
                           int sessionId, unsigned short error_code)
{
    global_error_t err;
    memset(&err, 0 , sizeof(err));
    int body_len = sizeof(global_error_t) - sizeof(remote_header_t);
    init_common_header(&err.header, 0, TSHOME_PROTOCOL_VERSION, body_len, fun_code,
                       ERROR, SERVER);
    err.error_code = error_code;
    err.session_id = sessionId;
    memset((char *)err.space, 0 , sizeof(err.space));
    fill_global_error(snd_buff, &err);
    *snd_len = sizeof(global_error_t);
}

static ts_user *find_user(const char *username, unsigned u_len, ts_user &NewUser)
{
    ts_user *user = g_udb_mgr.user.search_user(username, u_len, NewUser);
	
    return user; 
}

/**
 *����ָ�����ն���Ե�����sdp�ָ��ɿ��ظ�ʹ��״̬��
 *homeId:
 *username:�ն��û���
 */
static int restore_host_sdp(int homeId, const char *username, unsigned u_len, user_sdp *sdp)
{
	std::vector<ts_user> host_list;
	int result = g_udb_mgr.home.get_hosts(homeId, host_list);
	if(result == 0 && host_list.size() > 0)
	{
		//at present, one home just have one host
		ts_user host_user = host_list[0];

		if(host_user.sdpVec.size() > 0)
		{
			size_t i = 0;
			for(; i < host_user.sdpVec.size(); i++)
			{
				if(!strcmp((const char *)host_user.sdpVec[i].client_name,(const char *)username))
				{
					printf("in restore_host_sdp, host_list[0].sdpVec[%d].flag=%c\n", i, host_list[0].sdpVec[i].flag);
					host_user.sdpVec[i].flag = 's';/*single*/
					memset(host_user.sdpVec[i].client_name, 0, sizeof(host_user.sdpVec[i].client_name));
					//m_mutexCommunicationStatus.Enter();
					host_list[0] = host_user;
					printf("in restore_host_sdp, host_list[0].sdpVec[%d].flag=%c\n", i, host_list[0].sdpVec[i].flag);
					g_udb_mgr.home.set_hosts(homeId, host_list);
					//m_mutexCommunicationStatus.Leave();
					if(NULL != sdp)
						memcpy(sdp, &(host_user.sdpVec[i]), sizeof(user_sdp));
					break;
				}
			}
		}
	}
	return TS_SUCCESS;
}

static int do_when_logout(const char *username, unsigned u_len, user_sdp *sdp)
{
    CEZGuard guard(g_mutexTspHandlerLogout);
    LOG4CPLUS_DEBUG(LOG_TURN, "do_when_logout");

    if(NULL != username)
    {
        ts_user user;
        ts_user *res = find_user(username, u_len, user);
        if(NULL != res)
        {
            //method = "do_when_logout";
            int sessionId = user.session_id;
            int homeid = g_udb_mgr.mapping.get_home_id(sessionId);

            if(homeid == TS_INVALID)
            {
                return NULL;
            }
            sprintf(msg, "destroy host info for user(%s),homeId=%d,sessionId=%d", username,homeid,sessionId);
            LOG4CPLUS_DEBUG(LOG_TURN, msg);
            g_udb_mgr.home.remove_host(homeid, sessionId);

            sprintf(msg, "destroy mobile info for user(%s),homeId=%d,sessionId=%d", username,homeid,sessionId);
            LOG4CPLUS_DEBUG(LOG_TURN, msg);
            g_udb_mgr.home.remove_mobile(homeid, sessionId);

            sprintf(msg, "destroy temp info for user(%s),homeId=%d,sessionId=%d", username,homeid,sessionId);
            LOG4CPLUS_DEBUG(LOG_TURN, msg);
            g_udb_mgr.user.remove_usr(username, u_len);

            sprintf(msg, "destroy sessionId-homeId mapping info for user(%s),homeId=%d,sessionId=%d", username,homeid,sessionId);
            LOG4CPLUS_DEBUG(LOG_TURN, msg);
            g_udb_mgr.mapping.remove_mapping(sessionId);

            //set sdp.flag = 's',be reuseful
            restore_host_sdp(homeid, username, u_len, sdp);
            return TS_SUCCESS;
        }
        else
        {
            sprintf(msg,"user(%s) is not exist!\n", username);
            LOG4CPLUS_DEBUG(LOG_TURN, msg);
            return TS_FAILED;
        }
    }
    return NULL;
}

/*
called by pjnath structs, used to handle a protocol datagram.
*/
static int handle_remote_request(pj_turn_srv *srv,
                                 const char *userName,
                                 const unsigned int un_len,
                                 const unsigned char *rcv_buff,
                                 const unsigned int rcv_len,
                                 const char *addr,
                                 const unsigned int addr_len,
                                 const unsigned int port,
                                 unsigned char *snd_buff,
                                 unsigned int *snd_len,
                                 user_sdp *sdp)
{
    CEZGuard guard(g_mutexTspHandlerRequest);
    struct timeval t_a, t_b;//handle takes time
    double tTime = 0;
    gettimeofday(&t_a, NULL);
    //method = "handle_remote_request";
    char username[50] = {0};
    memcpy(username,userName,un_len);
    pj_ssize_t send;
    if(rcv_buff && rcv_len >= MIN_PACKET_LEN)
    {
        // sdph: sdp of host

        //char *ptr = NULL;
        //const char * const token = "SDPH:";
        char sdpSearial[10];
        int serial = 0;
        int maxSize = 0;
		
        //ptr = strstr((char *)rcv_buff, token);
		
        //if(NULL != ptr)
        if (!strncasecmp((const char *)hostToken, (const char *)rcv_buff, strlen(hostToken)))
        {
            strncpy(sdpSearial, (const char *)(rcv_buff+5), sizeof(sdpSearial)-1);
            serial = atoi(sdpSearial);

            ts_user user;
            ts_user *res = find_user(username, un_len, user);
            //ts_user *user = find_user(username, un_len);
            if(NULL == res)
            {
                int result = g_udb_mgr.user.select_user(username, un_len, user);
                if(result != 0)
                {
                    fill_error_res((u_char *)snd_buff, snd_len, 0, 0, GE_NO_USER);
                    return PJ_SUCCESS;
                }

                maxSize = user.home.maxUsers/2 + 1;
                user_sdp init_sdp;
                memset(&init_sdp, 0,  sizeof(user_sdp));
                for(int i = 0; i < maxSize; i++)
                {
                    user.sdpVec.push_back(init_sdp);
                }
				
                //pthread_mutex_init(&(user->lock),NULL);
            }

            int home__id = user.home_id;
            LOG4CPLUS_DEBUG(LOG_TURN, "SDPH(" <<serial<< ") case, username=" << username << ", homeid=" <<home__id );

            //the new sdp.
            user_sdp sdp4vec;
            memset(&sdp4vec, 0,  sizeof(sdp4vec));
            memcpy(sdp4vec.addr, addr, addr_len);
            //sdp4vec.addr[addr_len] = '\0';
            memcpy(sdp4vec.sdp, rcv_buff, rcv_len);
            sdp4vec.port = port;
            sdp4vec.flag = 's';
			printf("sdp4Vec.sdp=%s, addr=%s, port=%d\n", sdp4vec.sdp, sdp4vec.addr, sdp4vec.port);
			LOG4CPLUS_DEBUG(LOG_TURN, " sdp4Vec.sdp=" << sdp4vec.sdp << ",port=" << sdp4vec.port );
            //pthread_mutex_lock(&(user->lock));
            int vecSize = user.sdpVec.size();
            if(serial >= vecSize)
            {
                printf("too many sdp \n");
                LOG4CPLUS_DEBUG(LOG_TURN, " in SDPH case, too many sdp" << ", homeid=" <<home__id );
                //pthread_mutex_unlock(&(user->lock));
                return 0;
            }
            if(serial == 0)
            {//login
                sdp4vec.flag = 'p';
                user.reference = 0;
            }


            int cmped = 0;//sdp�Ƿ��ظ�
            if (vecSize > 0)
            {
                //int i = 0;
                //for(; i < vecSize; i++)
                //{
                	//printf("vecSize=%d,user.sdpVec[%d].sdp=%s\n", vecSize, i, user.sdpVec[i].sdp);
                    if(strncasecmp(user.sdpVec[serial].sdp, sdp4vec.sdp, sizeof(sdp4vec.sdp)) == 0)
                    {
                        cmped = 1;
                        //break;
                    }
                //}
            }
printf("cmped=%d\n", cmped);
            /*
              *if an user relogin, and his allocation still save in server memery, we must destroy 
              *his old allocation to free some memery and socket resource, in order to let other user
              *can reuse this resource.
              */
            if(!cmped)
            {
                user_sdp oldSdp = user.sdpVec[serial];
				printf("oldSdp.addr=%s, port=%d\n", oldSdp.addr, oldSdp.port);
                if(oldSdp.port != 0){
					pj_turn_allocation *oldAlloc = pj_turn_allocation_find_by_sdp(srv, &oldSdp);
					printf("oldAlloc=%p\n", oldAlloc);
                    pj_turn_allocation_shutdown(oldAlloc);
                	user.reference--;
					
					printf("sdp4Vec.addr=%s, port=%d\n", sdp4vec.addr, sdp4vec.port);
					pj_turn_allocation *newAlloc = pj_turn_allocation_find_by_sdp(srv, &sdp4vec);
					printf("newAlloc=%p\n", newAlloc);
                }
				printf("serial=%d\n",serial);
                user.sdpVec[serial] = sdp4vec;
                user.reference++;
				g_udb_mgr.tempUserMap[username] = user;
            }
            //pthread_mutex_unlock(&(user->lock));


            *snd_len = 0;//if this is a host, response nothing
        }
        else if(!strncasecmp((const char *)clientTokenWan, (const char *)rcv_buff, strlen(clientTokenWan)))
        {
            // sdpl: sdp of terminal
            
            ts_user user;
            ts_user *res= find_user(username, un_len, user);
            //ts_user *user = (ts_user *)find_user(username, un_len);
            if(NULL == res)
            {
                int result = g_udb_mgr.user.select_user(username, un_len, user);
                if(result != 0)
                {
                    fill_error_res((u_char *)snd_buff, snd_len, 0, 0, GE_NO_USER);
                    return PJ_SUCCESS;
                }
                //pthread_mutex_init(&(user->lock),NULL);
                user_sdp init_sdp;
				__trip;
                memset(&init_sdp, 0,  sizeof(init_sdp));
				
                //д�ӿ�
                __trip;
                user.sdpVec.push_back(init_sdp);
                __trip;
            }
            int home__id = user.home_id;
            //PJ_LOG(3,(THIS_FILE, "(%s): in SDPL case, username=%s,homeid=%d", method, username, home__id));
            LOG4CPLUS_DEBUG(LOG_TURN, " in SDPL case, username=" << username << ", homeid=" <<home__id );

            user_sdp sdp4vec;
            memcpy(sdp4vec.addr, addr, addr_len);
            memcpy(sdp4vec.sdp, rcv_buff, rcv_len);
            sdp4vec.port = port;
            sdp4vec.flag = 's';

            //pthread_mutex_lock(&(user->lock));
            int vecSize = user.sdpVec.size();
            if(vecSize == 0)
            {//login
                sdp4vec.flag = 'p';
                user.reference = 0;
            }

            int cmped = 0;//sdp�Ƿ��ظ�
            if (vecSize > 0)
            {
                int i = 0;
                for(; i < vecSize; i++)
                {
                    if(strncasecmp(user.sdpVec[i].sdp, sdp4vec.sdp, sizeof(sdp4vec.sdp)) == 0)
                    {
                        cmped = 1;
                        break;
                    }
                }
            }
            /*
              *if an user relogin, and his allocation still save in server memery, we must destroy 
              *his old allocation to free some memery and socket resource, in order to let other user
              *can reuse this resource.
              */
            if(!cmped)
            {
                user_sdp oldSdp = user.sdpVec[0];
				__trip;
                if(oldSdp.port != 0){
					__trip;
                    pj_turn_allocation_shutdown(pj_turn_allocation_find_by_sdp(srv, &oldSdp));
					restore_host_sdp(home__id, username, un_len, NULL);
                }
				__trip;
                user.sdpVec[0] = sdp4vec;
                user.reference = 1;
				g_udb_mgr.tempUserMap[username] = user;
            }
            //pthread_mutex_unlock(&(user->lock));

            std::vector<ts_user> user_list ;
            int result = g_udb_mgr.home.pair_user(home__id, 1, user_list);
            if(result == 0 && user_list.size() > 0)
            {
                //at present, one home just have one host
                ts_user host_user = user_list[0];
                printf("in case sdpl: host_user name=%s\n", host_user.username);
                //pthread_mutex_lock(&(host_user->lock));
                LOG4CPLUS_DEBUG(LOG_TURN, "in case sdpl: host_user name="<<host_user.username<<",homeId="<<host_user.home_id);
                user_sdp sdp2;
				memset(&sdp2, 0, sizeof(user_sdp));
                //std::vector<user_sdp> sdps = host_user.sdpVec;

                if(host_user.sdpVec.size() > 0)
                {
                    size_t i = 0;
                    for(; i < host_user.sdpVec.size(); i++)
                    {
                        if(host_user.sdpVec[i].flag == 's')
                        {
                            host_user.sdpVec[i].flag = 'p';/*paired*/
                            memcpy(host_user.sdpVec[i].client_name, username, strlen(username));
                            //m_mutexCommunicationStatus.Enter();
                            user_list[0] = host_user;
                            g_udb_mgr.home.set_hosts(home__id, user_list);
                            //m_mutexCommunicationStatus.Leave();
                            sdp2 = host_user.sdpVec[i];

                            printf("host's sdp : host_user.sdpVec[%d/%d].addr=%s, port=%d\n", i, host_user.sdpVec.size(), sdp2.addr, sdp2.port);
                            break;
                        }
                    }

                }
                //pthread_mutex_unlock(&(host_user->lock));
                __trip;
				//printf("Paired for %s:%d,%s,SDPH is%s,\naddr=%s,port=%d",sdp4vec.addr, sdp4vec.port, sdp4vec.sdp, sdp2.sdp, sdp2.addr, sdp2.port);
                if(0 != sdp2.port)
                {
                __trip;
                    //send a mobile sdp data to host
                    send_data_to_client (srv,
                                         rcv_buff,
                                         rcv_len,
                                         sdp2.addr,
                                         sdp2.port);
					__trip;
                    //fill the send buffer with a host sdp data
                    *snd_len = strlen((const char *)sdp2.sdp)+1;
					__trip;
                    memcpy(snd_buff, sdp2.sdp, *snd_len - 1);
					__trip;
                    sprintf(msg, "Paired for %s:%d,%s,SDPH is%s,\naddr=%s,port=%d",sdp4vec.addr, sdp4vec.port, sdp4vec.sdp, sdp2.sdp, sdp2.addr, sdp2.port);
                    //PJ_LOG(3,(THIS_FILE, "%s:%s", "sdpl case", msg));
                    LOG4CPLUS_DEBUG(LOG_TURN, msg );
                    LOG4CPLUS_DEBUG(LOG_TURN, "paired successful" );
                }
                else
                {
                    *snd_len = 0;
                    LOG4CPLUS_ERROR(LOG_TURN, "No valid SDPH for this paired");
                    //�˴�Ӧ�ûظ��������޵Ĵ���
                    fill_error_res((u_char *)snd_buff, snd_len, 0, 0, 10);
                }
            }
            else
            {
                LOG4CPLUS_ERROR(LOG_TURN, "no host user");
                fill_error_res((u_char *)snd_buff, snd_len, 0, 0, GE_NO_HOST_CONNECT);
            }
        }
        else
        {
            remote_header_t header;
            memset(&header, 0, sizeof(remote_header_t));
            u_char *recv_buff = (u_char *)(rcv_buff);
            //			int i;
            DEB_CODE(
                __fline;printf("recv_buff\n");
                dumpBuffer(stdout, (unsigned char *)recv_buff,
                           rcv_len,
                           SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
            );
            if(pass_common_header(recv_buff, &header))
            {
                // parse a header, if receive buffer is null,return now.
                return PJ_SUCCESS;
            }

            //if(header != NULL){
            if(header.protocol_type != 0)
            {
                //if the datagram is not a remote protocol type, do nothing, return now.
                return PJ_SUCCESS;
            }
            //printf("after parse header \n");
            int body_len = 0;
            switch(header.function_code)
            {
                case HEARTBEAT:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "HEARTBEAT ");
                    // maintain a heartbeat by pjnath
                    break;
                }
                case ASK_SECRET_KEY:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "ASK_SECRET_KEY ");
                    //printf("\nin case ASK_SECRET_KEY\n");
                    ask_secret_req_t ask_req;
                    memset(&ask_req, 0, sizeof(ask_secret_req_t));
                    if(!parse_ask_secretkey_req(recv_buff, &ask_req)
                       && ask_req.header.comm_type == REQUEST)
                    {
                        // step 1 now wujj 2012-8-24 15:29:32
                        //ts_user *user = (ts_user *)find_user(username, un_len);
                        //if(NULL == user){
                        //	fill_error_res((u_char *)snd_buff, snd_len, header.function_code, 0, GE_NO_USER);
                        //	return PJ_SUCCESS;
                        //}
                        ask_secret_res_t ask_res;
                        memset(&ask_res, 0, sizeof(ask_secret_res_t));
                        body_len = sizeof(ask_secret_res_t) - sizeof(remote_header_t);
                        init_common_header(&ask_res.header, 0, TSHOME_PROTOCOL_VERSION, body_len , header.function_code,
                                           RESPONSE, SERVER);
                        RSA *rsa = this_tsp_handler->rsa;
                        char * rs = rt_get_rsa_publicKey(rsa);
                        int offsize = sizeof(ask_res.secret_key) - strlen(rs);
                        memset(ask_res.secret_key, '0', sizeof(ask_res.secret_key));
                        memcpy(ask_res.secret_key + offsize, rt_get_rsa_publicKey(rsa),
                               sizeof(ask_res.secret_key));
                        memcpy(ask_res.secret_mode, rt_get_rsa_bigNum(rsa),
                               strlen(rt_get_rsa_bigNum(rsa)));
                        fill_ask_secretkey_res((u_char *)(snd_buff), &ask_res);
                        *snd_len = sizeof(ask_secret_res_t);
                    }
                    break;
                }
                case PASS_SECRET_KEY:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "PASS_SECRET_KEY ");
                    //printf("\nin case PASS_SECRET_KEY sizeof(pass_secret_res_t)=%d\n", sizeof(pass_secret_res_t));
                    pass_secret_req_t pass_req;
                    memset(&pass_req, 0, sizeof(pass_secret_req_t));
                    if(!parse_pass_secretkey_req((u_char *)rcv_buff, &pass_req)
                       && pass_req.header.comm_type == REQUEST)
                    {
                        ts_user __NewUser;
                        ts_user *user = find_user(username, un_len, __NewUser);
                        //ts_user *user = (ts_user *)find_user(username, un_len);

                        if(NULL == user)
                        {
                            fill_error_res((u_char *)snd_buff, snd_len, header.function_code, 0, GE_NO_USER);
                            return PJ_SUCCESS;
                        }
                        if(NULL == this_tsp_handler->rsa)
                        {
                            LOG4CPLUS_ERROR(LOG_TURN, "the RSA do not initialize ");

                            return PJ_SUCCESS;
                        }
                        char temp_key[RSA_TEXT_LEN];
                        rt_decrypt((char *)pass_req.secret_key,temp_key , this_tsp_handler->rsa);
                        memcpy(user->aeskey, temp_key, sizeof(user->aeskey));
                        pass_secret_res_t pass_res;
                        memset(&pass_res, 0, sizeof(pass_secret_res_t));
                        body_len = sizeof(pass_secret_res_t) - sizeof(remote_header_t);
                        init_common_header(&pass_res.header, 0, TSHOME_PROTOCOL_VERSION, body_len, header.function_code,
                                           RESPONSE, SERVER);
                        unsigned int session_id = 0;//user->sessionId;//a2i((const char *)user->sessionId, sizeof(user->sessionId));
                        //if(session_id == 0)
                        session_id = g_udb_mgr.user.gen_usr_session_id(); // generate a session id, but don`t save
                        user->session_id = session_id;
                        pass_res.session_id = session_id;
g_udb_mgr.tempUserMap[username] = __NewUser;

                        fill_pass_secretkey_res((u_char *)snd_buff, &pass_res);

                        *snd_len = sizeof(pass_secret_res_t);
                    }
                    else
                    {
                        fill_error_res((u_char *)snd_buff, snd_len, header.function_code, 0, GE_PARSE_ERROR);
                        return PJ_SUCCESS;
                    }
                    break;
                }
                case LOGIN:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "LOGIN ");
                    //printf("in case LOGIN\n");
                    login_req_t login_req;
                    memset(&login_req, 0, sizeof(login_req_t));
                    if(!parse_login_req((u_char *)rcv_buff, &login_req)
                       && login_req.header.comm_type == REQUEST)
                    {
                        // fill a login_res_t object
                        login_res_t login_res;
                        //printf("sizeof(login_res_t) = %d\n", sizeof(login_res_t));
                        //LOG4CPLUS_DEBUG(LOG_TURN, "sizeof(login_res_t) =" << sizeof(login_res_t));

                        memset(&login_res, 0, sizeof(login_res_t));
                        body_len = sizeof(login_res_t) - sizeof(remote_header_t);
                        init_common_header(&login_res.header, 0, TSHOME_PROTOCOL_VERSION, body_len, header.function_code,
                                           RESPONSE, SERVER);
                        memset(login_res.space, 0, sizeof(login_res.space)/sizeof(u_char));
                        *snd_len = sizeof(login_res_t);
                        //for(i = 0; i < *snd_len; i++){
                        //printf("%d,", snd_buff[i]);
                        //}
                        //printf("\n");
                        ts_user __NewUser;
                        ts_user *selected_user = find_user(username, un_len, __NewUser);

                        //ts_user *selected_user = find_user(username, un_len);//(User *)hmap_search(temp_map,username);
                        ts_user login_user;
                        //memset(&login_user, 0 ,sizeof(ts_user));
                        //				if(login_user->aeskey == null) return : no aeskey error code
                        aes_encode aes_ins2 ;
                        memset(&aes_ins2, 0, sizeof(aes_encode));

                        if(NULL == selected_user)
                        {
                            fill_error_res((u_char *)snd_buff, snd_len, header.function_code, 0, GE_NO_USER);
                            return PJ_SUCCESS;
                        }
                        //create AES
                        at_create_key_by_string((const u_char *)selected_user->aeskey, &aes_ins2);
                        at_decrypt((char *)login_req.user_name, (char *)login_user.username,
                                   login_req.username_len, aes_ins2);
                        at_decrypt((char *)login_req.password, (char *)login_user.pwd,
                                   (int16_t)login_req.password_len, aes_ins2);
                        printf("in case LOGIN, after decrypt, user name = %s, pwd = %s\n",login_user.username, login_user.pwd);
                        LOG4CPLUS_DEBUG(LOG_TURN, "username:" << login_user.username<<" Pwd:" << login_user.pwd);

                        int result = TS_FAILED;
                        result = g_udb_mgr.user.verify_usr(login_user.username, strlen(login_user.username),
                                                           login_user.pwd, strlen(login_user.pwd));

                        int session_id = selected_user->session_id;
                        int homeid = selected_user->home_id;
                        //printf("in case LOGIN, selected user session id=%d, req.sessionid=%d,home id = %d\n",session_id, login_req.session_id, homeid);
                        if(result == 0 && (session_id == login_req.session_id))
                        { // login success
                            //memcpy(selected_user->sessionId, &session_id, sizeof(session_id));
                            // sort the user
                            if(login_req.header.comm_source == HOST_S)
                            {
                                printf("in case LOGIN, login success, save the user into host map\n");
                                g_udb_mgr.home.save_host(homeid, selected_user);
                                LOG4CPLUS_INFO(LOG_TURN, "Host user login success: userName = "<< selected_user->username << " homeId = " << selected_user->home_id);
                            }
                            else if(login_req.header.comm_source == CLIENT)
                            {
                                //printf("in case LOGIN, login success, save the user into mobile map\n");
                                g_udb_mgr.home.save_mobile(homeid, selected_user);
                                LOG4CPLUS_INFO(LOG_TURN, "Mobile user login success: userName = "<< selected_user->username << " homeId = " << selected_user->home_id);

                                //tell the peer's address
                                //ts_user host_user = {0};
                                std::vector<ts_user> host_list;
                                int result = g_udb_mgr.home.pair_user(homeid, 1, host_list);
                                if (result == 0 && host_list.size() > 0)
                                {
                                    //at present, one home just have one host
                                    ts_user host_user = host_list[0];
									user_sdp host_user_sdp = host_user.sdpVec[0];
                                    struct in_addr inp;
                                    inet_aton(host_user_sdp.addr, &inp);
                                    login_res.peer_ip = inp.s_addr;
                                    login_res.peer_port = host_user_sdp.port;
                                }
								#if 0
                                if (NULL != host_user)
                                {
                                    user_sdp host_user_sdp = host_user->sdpVec[0];
                                    struct in_addr inp;
                                    inet_aton(host_user_sdp.addr, &inp);
                                    login_res.peer_ip = inp.s_addr;
                                    login_res.peer_port = host_user_sdp.port;
                                }
								#endif
                            }
                            g_udb_mgr.mapping.put_session_home_id(session_id, homeid);
                        }
                        else
                        {
                            result = 0x01;
                        }
                        login_res.session_id = session_id;
                        login_res.status = result;
                        //end fill login_res_t
                        fill_login_res((u_char *)snd_buff, &login_res);
                        //for(i = 0; i < *snd_len; i++){
                        //printf("%d,", snd_buff[i]);
                        //}
                        //printf("\n");
                    }
                    break;
                }
                case UPDATE_QUERY:
                {
                    //printf("in case REQUEST_UPDATE\n");
                    LOG4CPLUS_INFO(LOG_TURN, "UPDATE_QUERY");
                    update_req_t check_req;
                    memset(&check_req, 0, sizeof(update_req_t));
                    if(!parse_updateconfig_req((u_char *)rcv_buff, &check_req)
                       && check_req.header.comm_type == REQUEST)
                    {
                        int session_id = check_req.session_id;
                        if(has_login(session_id) != TS_INVALID)
                        {//login successfully
                        	ts_user __NewUser;

                            ts_user *logined_user = find_user(userName, un_len, __NewUser);

                            IPC_DG_CHECK_UPDATE_REQ cfg_req;
                            memset(&cfg_req, 0, sizeof(IPC_DG_CHECK_UPDATE_REQ));
                            cfg_req.header.start = 0xFF;
                            cfg_req.header.protocol = 0x01;
                            short len = sizeof(IPC_DG_CHECK_UPDATE_REQ) - sizeof(IPC_HEADER);
                            cfg_req.header.length = len;
                            cfg_req.header.searilNum = 0;
                            cfg_req.header.function = CHECK_UPDATE;
                            cfg_req.header.type = IPC_REQUEST;
                            memset(cfg_req.header.space, 0, sizeof(cfg_req.header.space));
                            int homeId = g_udb_mgr.mapping.get_home_id(session_id);
                            cfg_req.homeId = homeId;
                            cfg_req.sessionId = session_id;
                            cfg_req.last_time = check_req.latest_time;
                            //printf("last_time=%lld\n",cfg_req.last_time);
                            memcpy(cfg_req.client_aes, logined_user->aeskey, sizeof(cfg_req.client_aes));
                            cfg_req.query_from = check_req.query_from;
                            cfg_req.pid_len = strlen((char *)check_req.product_id);
                            memcpy(cfg_req.product_id, check_req.product_id, sizeof(cfg_req.product_id));

                            ipc_handler *handler = g_GeneralSocketProcessor.GetIPC_hander();
                            handler->outgoing_handler(handler->aes_ins, send_buff, &cfg_req, CHECK_UPDATE);
                            send = sizeof(IPC_HEADER)+ntohs(cfg_req.header.length);
                            //printf("send size = %d", send);
                            int status = g_GeneralSocketProcessor.Send2Webs(0, (const char *)send_buff, send);
                            if (status<0) //send error // 0 - success, 1 failed, 2012-8-18 11:08:35
                            {
                                LOG4CPLUS_ERROR(LOG_TURN, "send_to_web error("<< status << ") send:"<< send);
                            }
                            LOG4CPLUS_INFO(LOG_TURN, "send_to_web error("<< status << ") send:"<< send);

                            memset(send_buff, 0, sizeof(send_buff));
                            //printf("send to web successful!\n");
                        }
                        else
                        {
                            fill_error_res((u_char *)snd_buff, snd_len, header.function_code, session_id, HADNOT_LOGIN);
                            LOG4CPLUS_ERROR(LOG_TURN, "UPDATE_QUERY HADNOT_LOGIN");

                            return PJ_SUCCESS;
                        }
                    }
                    break;
                }
                case P2P:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "P2P");
                    //by pjnath.
                    break;
                }
                case DEVICE_OPERATION:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "DEVICE_OPERATION");
                    handle_remote_response(srv, rcv_buff, rcv_len);
                    //do nothing on server
                    break;
                }
                case STOP_CONNECTION:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "STOP_CONNECTION");
                    //printf("in case STOP_CONNECTION\n");
                    stop_host_connect_req_t deny_req;
                    memset(&deny_req, 0, sizeof(host_deny_connect_req_t));
                    if(!parse_stophostnet_req((u_char *)rcv_buff, &deny_req)
                       && deny_req.header.comm_type == REQUEST)
                    {
                        int sessionId = deny_req.session_id;
                        //printf("in case STOP_CONNECTION, this user's session id is %d\n", sessionId);
                        if(has_login(sessionId) != TS_INVALID)
                        {
                        	ts_user __NewUser;
                            ts_user *logined_user = (ts_user *)find_user(username, un_len, __NewUser);
                            aes_encode aes_ins3 ;
                            memset(&aes_ins3, 0, sizeof(aes_encode));
                            at_create_key_by_string((const u_char *)logined_user->aeskey, &aes_ins3);
                            char check_str[6];
                            at_decrypt((char *)deny_req.check_code, check_str,
                                       6, aes_ins3);
                            //printf("in case STOP_CONNECTION, the check code is %s\n", check_str);
                            if(!strcmp("tiansu", (const char *)check_str))
                            {
                                //find host user
                                ts_user *host_user = NULL;
                                int home_id = g_udb_mgr.mapping.get_home_id(sessionId);
                                std::vector<ts_user> host_list;
                                int result = g_udb_mgr.home.get_hosts(home_id, host_list);
                                if(result == 0 && host_list.size() > 0)
                                {
                                    //at present, one home just have one host
                                    host_user = &(host_list[0]);
                                }
                                if(NULL != host_user)
                                {
                                    //printf("in case STOP_CONNECTION, host user name is  %s\n", host_user->username);
                                    logined_user->stop_host_sender = STOP_HOST_SENDER_TRUE;
									
                                    to_host_deny_connect_req_t to_host;
                                    memset(&to_host, 0, sizeof(to_host_deny_connect_req_t));
                                    memcpy(&to_host, &deny_req, sizeof(to_host_deny_connect_req_t));
                                    to_host.header.comm_source = 0x01;
                                    to_host.session_id = host_user->session_id;
                                    user_sdp sdp2 = host_user->sdpVec[0];
                                    send_data_to_client (srv,
                                                         (const unsigned char*)&to_host,
                                                         rcv_len,
                                                         sdp2.addr,
                                                         sdp2.port);
                                    *snd_len = 0;// response nothing,immediately
                                    //printf("in case STOP_CONNECTION, send req to host over, host ip=%s,port=%d,length=%d\n", sdp->addr, sdp->port, *snd_len);
                                }
                                else
                                {
                                    //printf("in case STOP_CONNECTION, no host be found\n");
                                    fill_error_res((u_char *)snd_buff, snd_len, header.function_code, sessionId, GE_NO_HOST_CONNECT);
                                    return PJ_SUCCESS;
                                }
                            }
                        }
                        else
                        {
                            fill_error_res((u_char *)snd_buff, snd_len, header.function_code, sessionId, HADNOT_LOGIN);
                            return PJ_SUCCESS;
                        }
                    }
                    break;
                }
                case LOGOUT:
                {
                    LOG4CPLUS_INFO(LOG_TURN, "P2P");
                    //printf("in case LOGOUT\n");
                    logout_req_t logout_req;
                    memset(&logout_req, 0, sizeof(logout_req_t));
                    if(!parse_logout_req((u_char *)rcv_buff, &logout_req)
                       && logout_req.header.comm_type == REQUEST)
                    {
                        int sessionId = logout_req.session_id;
                        //printf("in case LOGOUT, this user's session id is %d\n", sessionId);
                        if(has_login(sessionId) != TS_INVALID)
                        {
                            LOG4CPLUS_INFO(LOG_TURN, "User logout...");
                            do_when_logout(username, un_len, sdp);

                            logout_res_t logout_res;
                            memset(&logout_res, 0, sizeof(logout_res_t));
                            body_len = sizeof(logout_res_t) - sizeof(remote_header_t);
                            init_common_header(&logout_res.header, 0, TSHOME_PROTOCOL_VERSION, body_len, header.function_code,
                                               RESPONSE, SERVER);
                            logout_res.session_id = logout_req.session_id;
                            logout_res.res_code = 0x00;
                            memset(logout_res.space, 0, sizeof(logout_res.space));
                            fill_logout_res((u_char *)snd_buff, &logout_res);
                            *snd_len = sizeof(logout_res_t);
                        }
                        else
                        {
                            //printf("in case LOGOUT, this user had been never logined\n");
                            fill_error_res((u_char *)snd_buff, snd_len, header.function_code, sessionId, HADNOT_LOGIN);
                            return PJ_SUCCESS;
                        }
                    }
                    break;
                }
                default:
                break;

            }

            //}
        }
    }
    else
    {
        LOG4CPLUS_ERROR(LOG_TURN, "rcv_len("<<rcv_len <<") should > 0");
    }

    gettimeofday(&t_b, NULL);
    tTime = (t_b.tv_sec - t_a.tv_sec) * 1000 + (t_b.tv_usec - t_a.tv_usec)/1000.0;
    LOG4CPLUS_INFO(LOG_TURN, "HANDLE END takes:"<< tTime << " ms");
    //printf("-----------------------handle this request takes : %lf ms-----------------------\n", tTime);
    return PJ_SUCCESS;
}

static int handle_remote_response(pj_turn_srv *srv, const unsigned char *rcv_buff, const unsigned int rcv_len)
{
    //printf("\n\n*************handle_remote_response*****************\n\n");
    LOG4CPLUS_INFO(LOG_TURN, "handle_remote_response");
    //int i = 0;
    //for(;i<24;i++){
    //printf("%d,",rcv_buff[i]);
    //}
    //printf("\n");
    DBG_CODE(
        __fline;printf("recv_buff(%d)\n", rcv_len);
        dumpBuffer(stdout, (unsigned char *)rcv_buff,
                   rcv_len,
                   SHOW_ASCII | SHOW_BINAR | SHOW_HEXAD | SHOW_LINER);
    );
    CEZGuard guard(g_mutexCommunicationresponse);
    if(NULL != rcv_buff && rcv_len >= sizeof(remote_header_t))
    {
        __trip;
        remote_header_t header;
        pj_ssize_t send;
        memset(&header, 0, sizeof(remote_header_t));
        u_char *recv_buff = (u_char *)(rcv_buff);
        if(pass_common_header(recv_buff, &header))
        {
            // parse a header, if receive buffer is null,return now.
            return PJ_SUCCESS;
        }
        if(header.protocol_type != 0)
        {
            //if the datagram is not a remote protocol type, do nothing, return now.
            return PJ_SUCCESS;
        }
        //		int body_len = 0;
        switch(header.function_code)
        {
            case STOP_CONNECTION:
            {
                LOG4CPLUS_INFO(LOG_TURN, "STOP_CONNECTION");
                //printf("\n\n*************receive a stop connection res*****************\n\n");
                host_deny_connect_res_t deny_res;
                memset(&deny_res, 0, sizeof(host_deny_connect_res_t));
                if(!parse_stophostnet_res_from_host((u_char *)rcv_buff, &deny_res)
                   && deny_res.header.comm_type == RESPONSE)
                {
                    int sessionId = deny_res.session_id;
                    //printf("\n\n*************receive a stop connection res*****************sessionid=%d\n\n",sessionId);
                    int home_id = g_udb_mgr.mapping.get_home_id(sessionId);
                    std::vector<ts_user> mobile_list;
                    int result = g_udb_mgr.home.get_mobiles(home_id, mobile_list);
                    if(result == 0 && mobile_list.size() > 0)
                    {

                        if(deny_res.res_code == DENY_SUCCESS)
                        {
                            //if success, notify all mobiles.
                            size_t i = 0;
                            for(; i < mobile_list.size(); i++)
                            {
                                deny_res.session_id = mobile_list[i].session_id;

                                user_sdp sdp = mobile_list[i].sdpVec[0];

                                send_data_to_client (srv,
                                                     (const unsigned char*)&deny_res,
                                                     rcv_len,
                                                     sdp.addr,
                                                     sdp.port);
                            }
                        }
                        else
                        {
                            //some errs take about
                            size_t i = 0;
                            for(; i < mobile_list.size(); i++)
                            {
                                if (mobile_list[i].stop_host_sender == STOP_HOST_SENDER_TRUE)
                                {

                                    user_sdp sdp = mobile_list[i].sdpVec[0];

                                    send_data_to_client (srv,
                                                         (const unsigned char*)&deny_res,
                                                         rcv_len,
                                                         sdp.addr,
                                                         sdp.port);
                                    break;
                                }
                            }

                        }

                    }
                    //send to web
                    IPC_DG_HOST_SERV to_web_res;
                    memset(&to_web_res, 0, sizeof(IPC_DG_HOST_SERV));
                    to_web_res.header.protocol = 0x01;
                    to_web_res.header.start = 0xFF;
                    int len_ = sizeof(IPC_DG_HOST_SERV) - sizeof(IPC_HEADER);
                    //printf("************len=%d, homeid=%d*****************\n",len_, home_id);
                    to_web_res.header.length = htons(len_);
                    to_web_res.header.type = IPC_RESPONSE;
                    to_web_res.header.searilNum = htonl(deny_res.report_id);
                    to_web_res.header.function = HOST_SERV_REQ;
                    memset(to_web_res.header.space, 0, sizeof(to_web_res.header.space));
                    to_web_res.homeId = (home_id);
                    memset(to_web_res.userName, 0, sizeof(to_web_res.userName));
                    to_web_res.cmd_state = deny_res.res_code+2;
                    u_char buff[128];

                    ipc_handler *handler = g_GeneralSocketProcessor.GetIPC_hander();
                    handler->outgoing_handler(handler->aes_ins, buff, &to_web_res, HOST_SERV_REQ);
                    send = sizeof(to_web_res);
                    int status = g_GeneralSocketProcessor.Send2Webs(0, (const char *)buff, send);

                    if (status<0) //send error // 0 - success, 1 failed, 2012-8-18 11:08:35
                    {
                        LOG4CPLUS_ERROR(LOG_TURN, "send_to_web error("<< status << ") send:"<< send);
                    }
                }
                break;
            }
            case DEVICE_OPERATION:
            {
                LOG4CPLUS_INFO(LOG_TURN, "DEVICE_OPERATION");
                ir_study_res_t wifi_res;
                memset(&wifi_res, 0, sizeof(ir_study_res_t));
                if(!parse_wifictrl_res((u_char *)rcv_buff, &wifi_res)
                   && wifi_res.kic_head.header.comm_type == RESPONSE
                   && wifi_res.kic_head.device_type == IR_STUDY)
                {
                    int sessionId = wifi_res.kic_head.session_id;
                    int reportId = wifi_res.kic_head.home_device_id;
                    int homeid = g_udb_mgr.mapping.get_home_id(sessionId);
                    IPC_DG_INF_LEARN inf_res;
                    inf_res.header.start = 0xFF;
                    inf_res.header.protocol = 0x01;
                    int len_ = sizeof(IPC_DG_INF_LEARN) - sizeof(IPC_HEADER);
                    //len_ = htons(len_);
                    inf_res.header.length = len_;
                    inf_res.header.searilNum = reportId;
                    inf_res.header.function = INFRARED_LEARN;
                    inf_res.header.type = IPC_RESPONSE;
                    inf_res.homeId = homeid;
                    inf_res.gateway_id = wifi_res.gateway_id;
                    inf_res.channel = wifi_res.channel;
                    inf_res.key = wifi_res.function_code;
                    inf_res.result = wifi_res.res_code;
                    u_char buff[128];

                    ipc_handler *handler = g_GeneralSocketProcessor.GetIPC_hander();
                    handler->outgoing_handler(handler->aes_ins, buff, &inf_res, INFRARED_LEARN);
                    send = sizeof(inf_res);
                    int status = g_GeneralSocketProcessor.Send2Webs(0, (const char *)buff, send);

                    if (status<0) //send error // 0 - success, 1 failed, 2012-8-18 11:08:35
                    {
                        LOG4CPLUS_ERROR(LOG_TURN, "send_to_web error("<< status << ") send:"<< send);
                    }

                }
                break;
            }
            default :
            break;
        }
    }
    else
    {
        //__trip;
        LOG4CPLUS_ERROR(LOG_TURN, "rcv_len:"<<rcv_len);
    }
    LOG4CPLUS_INFO(LOG_TURN, "END handle_remote_response");
    return PJ_SUCCESS;
}

/*static int handle_remote_timeout(pj_turn_srv *srv, const char *username, const unsigned int un_len)
{	
	LOG4CPLUS_INFO(LOG_TURN, "User connection timeout");
 
	ts_user *user = (ts_user *)find_user(username, un_len);
	if (NULL != user) {
		pj_turn_allocation_destroy_by_user(srv, user);
		do_when_logout(username, un_len);
		LOG4CPLUS_INFO(LOG_TURN, "do_when_logout:"<<username);
	}
	else
	{
		LOG4CPLUS_ERROR(LOG_TURN, "not found user:"<<username);
	}
	return PJ_SUCCESS;	
}*/

static int handle_remote_timeout(pj_turn_srv *srv,
                                 const char *username,
                                 const unsigned int un_len,
                                 const char *ip,
                                 const int ip_len,
                                 const int clt_port,
                                 user_sdp *sdp)
{
    LOG4CPLUS_INFO(LOG_TURN, "User connection timeout");
    return handle_remote_destroy(srv, username, un_len, ip, ip_len, clt_port, sdp);
}

static int handle_remote_destroy(pj_turn_srv *srv,
                                 const char *username,
                                 const unsigned int un_len,
                                 const char *ip,
                                 const int ip_len,
                                 const int clt_port,
                                 user_sdp *sdp)
{
    CEZGuard guard(g_mutexTspHandlerDestroy);

    LOG4CPLUS_INFO(LOG_TURN, "User's SDP destroy");
	ts_user user;
	ts_user *res = find_user(username, un_len, user);
    //ts_user *user = find_user(username, un_len);
    if (NULL != res)
    {
        user.reference--;
        LOG4CPLUS_INFO(LOG_TURN, "SDP destroy:"<<username);
        int ref = user.reference;
        LOG4CPLUS_INFO(LOG_TURN, "alive sdp count:"<<ref);
        if(user.reference <= 0)
        {
            do_when_logout(username, un_len, sdp);
            LOG4CPLUS_INFO(LOG_TURN, "do_when_logout:"<<username);
            return PJ_SUCCESS;
        }
        else if(user.reference > 0)
        {
            if(user.sdpVec.size() > 0)
            {
                std::vector<user_sdp>::iterator p;

                for(p = user.sdpVec.begin(); p != user.sdpVec.end(); p++)
                {
                    if (strncasecmp(p->addr, ip, ip_len) == 0 && p->port == clt_port)
                    {
                        user.sdpVec.erase(p);
						user_sdp newSdp;
						memset(&newSdp, 0, sizeof(user_sdp));
						user.sdpVec.push_back(newSdp);
                        g_udb_mgr.home.save_host(user.home_id, &user);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        LOG4CPLUS_ERROR(LOG_TURN, "not found user:"<<username);
    }
    return PJ_SUCCESS;
}


