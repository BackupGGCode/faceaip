/*
 * udb_mgr.c
 *
 *  Created on: 2012-2-13
 *      Author: sunzq
 */

#define KEY_LEN 50			//length of the map key
#define THIS_FILE	"udb_mgr.c"

#define SELECT_HOMEID_BY_DEVICEID "SELECT homeId FROM t_jstelcom_user WHERE deviceId='%s'"

#include <app/common/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "app.h"
#include <pjlib.h>
#include <map>
#include <vector>
#include <string>
#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"
#include "../CommonInclude.h"

#include "../Logs.h"
#include "../Configs/ConfigManager.h"
#include <app/server/mysqlpool.h>

ts_udb_mgr g_udb_mgr;

/**pj server for this*/
static pj_turn_srv *pj_srv;

static CEZMutex m_mutexCommunicationStatus;


typedef struct integer{
	int i;
}INTEGER;

/***********prototype***********/
static ts_user *search_user_impl(const char *username, unsigned un_len);
static int verify_usr_impl(const char *username, 
					  unsigned u_len,
					  const char *pwd,
					  unsigned p_len);

static void get_usr_passwd_impl(const char *username, int u_len, char  **pwd);

static int gen_session_id_impl();

static int select_user_from_db_impl(const char *username, const unsigned un_len, ts_user **user);

static void remove_usr_impl(const char *username, unsigned u_len);

static ts_user *save_host_impl(int home_id, ts_user *host_user);

static ts_user *save_mobile_impl(int home_id, ts_user *mobile_user);

static int get_hosts_impl(int home_id, std::vector<ts_user> & hosts);

static int set_hosts_impl(int home_id, std::vector<ts_user> hosts);


static int get_hosts_by_device_id_impl(const char *device_id, std::vector<ts_user> & hosts);

static int get_mobiles_impl(int home_id, std::vector<ts_user> & mobiles);

static int set_mobiles_impl(int home_id, std::vector<ts_user> & mobiles);


static void remove_host_impl(int home_id, int session_id);

static void remove_mobile_impl(int home_id, int session_id);

static int pair_user_impl(int home_id, int self_type, std::vector<ts_user> & paired);

static int get_home_id_impl(int session_id);

static void put_session_home_id_impl(int session_id, int home_id);

static void remove_mapping_impl(int session_id);

static void close_userdb_impl();

/****************public API*****************/

int ts_udb_create(pj_turn_srv *srv)
{
	CConfigDB __cfgDB;
	 __cfgDB.update();

	assert(srv != NULL);
	LOG4CPLUS_INFO(LOG_DB, "ts_udb_create:" << __cfgDB.getConfig().strHost);
	int result = pool_init_full(	 __cfgDB.getConfig().strHost.c_str(),
					__cfgDB.getConfig().strUser.c_str(), 
					__cfgDB.getConfig().strPwd.c_str(),
					__cfgDB.getConfig().strDatabase.c_str(), 
					__cfgDB.getConfig().iPort, 
					NULL, 
					0);
	
	if (result != TS_SUCCESS)
	{
		LOG4CPLUS_ERROR(LOG_DB, "pool_init_full failed");
		return TS_FAILED;
	}
	else
	{
		LOG4CPLUS_INFO(LOG_DB, "pool_init_full succeeded");
	}

//	g_udb_mgr = PJ_POOL_ZALLOC_T(srv.core.pool, ts_udb_mgr);
//	if (NULL == g_udb_mgr)
//	{
//		LOG4CPLUS_ERROR(LOG_DB, "PJ_POOL_ZALLOC_T failed");
//		return TS_FAILED;
//	}
	g_udb_mgr.obj_name = (char *)THIS_FILE;
	g_udb_mgr.sessionId = BEGIN_SESSION_ID;
	pj_srv = srv;

//	pj_status_t status = pj_lock_create_recursive_mutex(srv.core.pool, g_udb_mgr.obj_name, 
//								&g_udb_mgr.core.lock);
	//pthread_mutex_init(&(g_udb_mgr.core.lock),NULL);
//	if (status != PJ_SUCCESS)
//	{
//		return TS_FAILED;
//	}
	
	//set callbacks
	g_udb_mgr.core.destroy = &close_userdb_impl;
	
	g_udb_mgr.user.search_user = &search_user_impl;
	g_udb_mgr.user.select_user = &select_user_from_db_impl;
	g_udb_mgr.user.verify_usr = &verify_usr_impl;
	g_udb_mgr.user.get_passwd = &get_usr_passwd_impl;
	g_udb_mgr.user.gen_usr_session_id = &gen_session_id_impl;
	g_udb_mgr.user.remove_usr = &remove_usr_impl;

	g_udb_mgr.home.save_host = &save_host_impl;
	g_udb_mgr.home.save_mobile = &save_mobile_impl;
	g_udb_mgr.home.get_hosts = &get_hosts_impl;
	g_udb_mgr.home.set_hosts = &set_hosts_impl;
	g_udb_mgr.jstelcom.get_hosts_by_device_id = &get_hosts_by_device_id_impl;
	g_udb_mgr.home.get_mobiles = &get_mobiles_impl;	
	g_udb_mgr.home.set_mobiles = &set_mobiles_impl;
	g_udb_mgr.home.remove_host = &remove_host_impl;
	g_udb_mgr.home.remove_mobile = &remove_mobile_impl;
	g_udb_mgr.home.pair_user = &pair_user_impl;

	g_udb_mgr.mapping.get_home_id = &get_home_id_impl;
	g_udb_mgr.mapping.put_session_home_id = &put_session_home_id_impl;
	g_udb_mgr.mapping.remove_mapping = &remove_mapping_impl;

	return TS_SUCCESS;
}

/**search user from temporary user hash table*/
static ts_user *search_user_impl(const char *username, const unsigned un_len)
{
	if(g_udb_mgr.tempUserMap.find(username) == g_udb_mgr.tempUserMap.end())
    {
        return NULL;
    }
    else
    {
		return &(g_udb_mgr.tempUserMap[username]);
    }
}

/*
select an user from database with username.
*/
static int select_user_from_db_impl(const char *username, const unsigned un_len, ts_user **user)
{
	LOG4CPLUS_INFO(LOG_DB, "select_user_from_db_impl ");
	//get a mysql connection from pool.
	MysqlConn conn = pool_getConn();
	MYSQL *mysql = conn.mysql;
	if (NULL == mysql) {
		//PJ_LOG(3,(THIS_FILE, "database connection is null"));
		LOG4CPLUS_ERROR(LOG_DB, "pool_getConn");

		return 3;
	}
	if (NULL == username || un_len <= 0)
	{
		//PJ_LOG(3,(THIS_FILE, "invalid user name or length"));
		LOG4CPLUS_ERROR(LOG_DB, "invalid user name or length:"<<un_len);
		return 2;
	}
	
	//preparement parameter.
	MYSQL_BIND in[1];
	//result column parameter.
	MYSQL_BIND out[3];

	//preparement parameter.
	MYSQL_BIND home_in[1];
	//result column parameter.
	MYSQL_BIND home_out[1];
	if(NULL != mysql){
		//new an user, destroied when logout and allocation destroied.
		ts_user tempUser;// = PJ_POOL_ZALLOC_T(pj_srv->core.pool, ts_user);
		//mysql statement
		MYSQL_STMT *stmt = NULL;
		stmt = mysql_stmt_init(mysql);
		//prepare sql
		//char *query = (char *)"select username, pwd, homeid from user where username=?";
		const char *query = "select userName, pwd, home from t_user where username=?";

		if(NULL == stmt){
			PJ_LOG(3,(THIS_FILE, "stmt is null"));
			return 3;
		}
		//excute prepare sql
		if(mysql_stmt_prepare(stmt, query, strlen(query)))
		{
			PJ_LOG(3,(THIS_FILE, "mysql_stmt_prepare: %s", mysql_error(mysql)));
			return 3;
		}
		//bind parameters for preparement and result
		memset(in,0,sizeof(in));
		in[0].buffer_type = MYSQL_TYPE_STRING;
		in[0].buffer = (void *)username;
		in[0].buffer_length = strlen(username);

		mysql_stmt_bind_param(stmt,in);

		memset(out,0,sizeof(out));

		out[0].buffer_type = MYSQL_TYPE_STRING;
		out[0].buffer = tempUser.username;
		out[0].buffer_length = sizeof(tempUser.username);

		out[1].buffer_type = MYSQL_TYPE_STRING;
		out[1].buffer = tempUser.pwd;
		out[1].buffer_length = sizeof(tempUser.pwd);

		out[2].buffer_type = MYSQL_TYPE_LONG;
		out[2].buffer = &tempUser.home_id;

		mysql_stmt_bind_result(stmt, out);
		//end bind

		//execute select
		mysql_stmt_execute(stmt);
		mysql_stmt_store_result(stmt);
		int i = 0;
		while(mysql_stmt_fetch(stmt) == 0) {//fetch next result
			i++;
			//在下面查完home表后一起保存。
			//g_udb_mgr.tempUserMap[tempUser.username] = tempUser;
		}
		mysql_stmt_close(stmt);
		stmt = NULL;
		//end select user.
		///////////////////////////////////////////////////////////////////////////
		if(i == 0){
			PJ_LOG(3,(THIS_FILE, "no selected user by userName : %s", username));
			return 1;
		}
		//begin select home info.
		stmt = mysql_stmt_init(mysql);
		//prepare sql
		const char *home_query = "select maxUsers from t_home where id=?";

		if(NULL == stmt){
			PJ_LOG(3,(THIS_FILE, "stmt is null"));
			return 3;
		}
		//excute prepare sql
		if(mysql_stmt_prepare(stmt, home_query, strlen(home_query)))
		{
			PJ_LOG(3,(THIS_FILE, "mysql_stmt_prepare: %s", mysql_error(mysql)));
			return 3;
		}
		//bind parameters for preparement and result
		memset(home_in,0,sizeof(home_in));
		home_in[0].buffer_type = MYSQL_TYPE_LONG;
		home_in[0].buffer = (void *)&tempUser.home_id;

		mysql_stmt_bind_param(stmt,home_in);

		memset(home_out,0,sizeof(home_out));

		home_out[0].buffer_type = MYSQL_TYPE_LONG;
		home_out[0].buffer = (void *)&(tempUser.home.maxUsers);

		mysql_stmt_bind_result(stmt, home_out);
		//end bind

		//execute select
		mysql_stmt_execute(stmt);
		mysql_stmt_store_result(stmt);
		i = 0;
		while(mysql_stmt_fetch(stmt) == 0) {//fetch next result
			i++;
			g_udb_mgr.tempUserMap[tempUser.username] = tempUser;
		}
		mysql_stmt_close(stmt);

		//recycle mysql connection
		pool_recycle(conn);

		if(i == 0){
			PJ_LOG(3,(THIS_FILE, "no selected home by homeId : %d", tempUser.home_id));
			return 1;
		}
		*user = &(g_udb_mgr.tempUserMap[tempUser.username]);
		return 0;
	}

	return 3;
}

static int verify_usr_impl(const char *username, 
					  const unsigned u_len,
					  const char *pwd,
					  const unsigned p_len)
{
	if(NULL == username || u_len <= 0){
		return TS_FAILED;
	}else{
		ts_user *user = search_user_impl(username, u_len);

		if(user == NULL){
			//user = select_user_from_db_impl(username, u_len);
			return TS_FAILED;
		}
		if(user == NULL){
			return 1;//no such a user
		}

		int _result = MD5_VALI_FALSE;
		mt_validate(user->pwd, pwd, &_result);
		return _result;
	}
	return TS_FAILED;
}

static void get_usr_passwd_impl(const char *username, int u_len, char  **pwd)
{
	if(NULL == username){
		return;
	}else{
		ts_user *user = NULL;
		user = search_user_impl(username, u_len);

		if(user == NULL){
			int result = select_user_from_db_impl(username, u_len, &user);
			if(result != 0){
				return;
			}
		}
		if(user == NULL){
			return;//no such a user
		}
		
		strcpy(*pwd, user->pwd);
	}
}

static int gen_session_id_impl()
{
	
	CEZGuard guard(m_mutexCommunicationStatus);
	//pthread_mutex_lock(&(g_udb_mgr.core.lock));
	int s = TS_INVALID;
	g_udb_mgr.sessionId++;
	s = g_udb_mgr.sessionId;
	//pthread_mutex_unlock(&(g_udb_mgr.core.lock));
	return s;
}

static void remove_usr_impl(const char *username, unsigned u_len)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	g_udb_mgr.tempUserMap.erase(username);
}

static ts_user *save_host_impl(int home_id, ts_user *host_user)
{
	if (home_id <= 0 || NULL == host_user)
	{
		return NULL;
	} else {
		CEZGuard guard(m_mutexCommunicationStatus);
		if(g_udb_mgr.hostUserMap.find(home_id) == g_udb_mgr.hostUserMap.end())
	    {
	    	//如果主机用户列表不存在
			std::vector<ts_user> hostVec;
	    	hostVec.push_back(*host_user);
			g_udb_mgr.hostUserMap[home_id] = hostVec;
	    }
	    else
	    {
	    	//如果重复存在，替换
			std::vector<ts_user> hostVec = g_udb_mgr.hostUserMap[home_id];
			std::vector<ts_user>::iterator p;
	    	for(p = hostVec.begin(); p != hostVec.end(); p++)
			{
				if(!strncasecmp(p->username, host_user->username, sizeof(host_user->username)))
				{
					hostVec.erase(p);
					g_udb_mgr.hostUserMap[home_id] = hostVec;
					break;
				}
				
			}
			g_udb_mgr.hostUserMap[home_id].push_back(*host_user);
	    }
	}
	
	return host_user;
}

static ts_user *save_mobile_impl(int home_id, ts_user *mobile_user)
{
	if (home_id <= 0 || NULL == mobile_user)
	{
		return NULL;
	} else {
		CEZGuard guard(m_mutexCommunicationStatus);
		if(g_udb_mgr.mobileUserMap.find(home_id) == g_udb_mgr.mobileUserMap.end())
	    {
	    	//如果终端用户列表不存在
			std::vector<ts_user> mobileVec;
	    	mobileVec.push_back(*mobile_user);
			g_udb_mgr.mobileUserMap[home_id] = mobileVec;
	    }
	    else
	    {
	    	std::vector<ts_user> mobileVec = g_udb_mgr.hostUserMap[home_id];
			std::vector<ts_user>::iterator p;
	    	for(p = mobileVec.begin(); p != mobileVec.end(); p++)
			{
				if(!strncasecmp(p->username, mobile_user->username, sizeof(mobile_user->username)))
				{
					mobileVec.erase(p);
					g_udb_mgr.hostUserMap[home_id] = mobileVec;
					break;
				}
				
			}
			g_udb_mgr.mobileUserMap[home_id].push_back(*mobile_user);
	    }
	}

	return mobile_user;
}

static int get_hosts_impl(int home_id, std::vector<ts_user> & hosts)
{
	if(g_udb_mgr.hostUserMap.find(home_id) == g_udb_mgr.hostUserMap.end())
    {
    	return TS_INVALID;
    }
    else
    {
    	hosts = g_udb_mgr.hostUserMap[home_id];
		return 0;
    }
	
}

static int set_hosts_impl(int home_id, std::vector<ts_user> hosts)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	g_udb_mgr.hostUserMap[home_id] = hosts;
	return 0;
}


static int get_hosts_by_device_id_impl(const char *device_id, std::vector<ts_user> & hosts)
{
	int res = 0;
	MYSQL_RES *res_ptr = NULL;
    MYSQL_ROW sqlrow;
	int homeId = 0;
	char sql[1000] = {0};
	int result = TS_INVALID;
	if(device_id == NULL || !strcmp(device_id, ""))
	{
		return result;
	}
    //get a mysql connection from pool.
    MysqlConn conn = pool_getConn();
	MYSQL *mysql = conn.mysql;
    if (NULL == mysql)
    {
        LOG4CPLUS_ERROR(LOG_DB, "pool_getConn");
    }else{
	    
	    sprintf(sql, SELECT_HOMEID_BY_DEVICEID, device_id);
	    res = mysql_query(mysql, sql);
	    if(res)
	    {
	        LOG4CPLUS_ERROR(LOG_DB, "select t_jstelcom_user failed");
	    }else{
			LOG4CPLUS_INFO(LOG_DB, "select t_jstelcom_user success");
			res_ptr = mysql_store_result(mysql);
			while((sqlrow = mysql_fetch_row(res_ptr))){
				homeId = atoi(sqlrow[0]);
				printf("homeid=%d\n", homeId);
			}
			result = get_hosts_impl(homeId, hosts);
		}
	}
	//recycle mysql connection
    pool_recycle(conn);
	return result;
}


static int get_mobiles_impl(int home_id, std::vector<ts_user> & mobiles)
{
	if(g_udb_mgr.mobileUserMap.find(home_id) == g_udb_mgr.mobileUserMap.end())
    {
    	return TS_INVALID;
    }
    else
    {
    	mobiles = g_udb_mgr.mobileUserMap[home_id];
		return 0;
    }
}

static int set_mobiles_impl(int home_id, std::vector<ts_user> & mobiles)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	g_udb_mgr.mobileUserMap[home_id] = mobiles;
	return 0;
}


static void remove_host_impl(int home_id, int session_id)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	
	if(g_udb_mgr.hostUserMap.find(home_id) == g_udb_mgr.hostUserMap.end())
    {
    	return;
    }
    else
    {
    	std::vector<ts_user> vec = g_udb_mgr.hostUserMap[home_id];
		std::vector<ts_user>::iterator p;
    	for(p = vec.begin(); p != vec.end(); p++)
		{
			if(p->session_id == session_id){
				vec.erase(p);
				g_udb_mgr.hostUserMap[home_id] = vec;
				break;
			}
			
		}
		if(vec.size() == 0){
			g_udb_mgr.hostUserMap.erase(home_id);
		}
    }
	
}

static void remove_mobile_impl(int home_id, int session_id)
{	
	CEZGuard guard(m_mutexCommunicationStatus);
	if(g_udb_mgr.mobileUserMap.find(home_id) == g_udb_mgr.mobileUserMap.end())
    {
    	return;
    }
    else
    {
    	std::vector<ts_user> vec = g_udb_mgr.mobileUserMap[home_id];
		std::vector<ts_user>::iterator p;
    	for(p = vec.begin(); p != vec.end(); p++)
		{
			if(p->session_id == session_id){
				vec.erase(p);
				g_udb_mgr.mobileUserMap[home_id] = vec;
				break;
			}
			
		}
		if(vec.size() == 0){
			g_udb_mgr.mobileUserMap.erase(home_id);
		}
    }
}

static int pair_user_impl(const int home_id, const int self_type, std::vector<ts_user> & paired)
{
	if (self_type == 1) //mobile
	{
		return get_hosts_impl(home_id, paired);
	} else if (self_type == 2)
	{
		return get_mobiles_impl(home_id, paired);
	}
	return TS_INVALID;
}

static int get_home_id_impl(int session_id)
{
	if(g_udb_mgr.sessionHomeMap.find(session_id) == g_udb_mgr.sessionHomeMap.end())
    {
        return TS_INVALID;
    }
    else
    {
		return g_udb_mgr.sessionHomeMap[session_id];
    }
}

static void put_session_home_id_impl(int session_id, int home_id)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	if (session_id > BEGIN_SESSION_ID)
	{
		g_udb_mgr.sessionHomeMap[session_id] = home_id;
	}
}

static void remove_mapping_impl(int session_id)
{
	CEZGuard guard(m_mutexCommunicationStatus);
	if(g_udb_mgr.sessionHomeMap.find(session_id) == g_udb_mgr.sessionHomeMap.end())
    {
        return;
    }
    else
    {
		g_udb_mgr.sessionHomeMap.erase(session_id);
    }
}

static void close_userdb_impl()
{
	/*close connection pool*/
	pool_close();

	g_udb_mgr.tempUserMap.clear();
	g_udb_mgr.hostUserMap.clear();
	g_udb_mgr.mobileUserMap.clear();
	g_udb_mgr.sessionHomeMap.clear();
	 /* Destroy lock */
	//pj_lock_destroy(g_udb_mgr->core.lock);
    
}

