/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * tshome_typedef.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: tshome_typedef.h 5884 2012-08-28 04:05:46Z WuJunjie $
 *
 *  Explain:
 *     - #include "tshome_typedef.h" -
 *
 *  Update:
 *     2012-08-28 04:05:46  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __TSHOME_TYPEDEF_H__
#define __TSHOME_TYPEDEF_H__

#include <openssl/aes.h>
#include <vector>
#include "map"
#include <string>

#include "app/datagram/IPCProtocolDef.h"

#define BLOCK_SIZE AES_BLOCK_SIZE
#define ts_random(x) (rand()%x)
#define AES_KEY_BITS 128

typedef struct _ts_aes_encode
{
    AES_KEY encrypt_aes;
    AES_KEY decrypt_aes;
    unsigned char key[BLOCK_SIZE];
}
aes_encode;


/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*
  this is a FIFO queue implements with a array. there are two arrays in this data struct.
one is a real data store array as a memery storage, another is a points of each real data segment array
as a getter and putter value entry.
 
*/
typedef struct fifo_array_t
{
    unsigned int rows;			//max capacity of the array
    unsigned int columns;			//length of a element
    void **buff;				//a points array
    unsigned int used_size;		//real element number in this array
    unsigned int fetch_pos;		//cursor,use to get value of a point.
    unsigned int save_pos;		//cursor,use to save value of a point
    pthread_mutex_t lock;			//mutex lock
}
fifoArray;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef LOGIN_RESULT
	#define LOGIN_RESULT
	#define LOGIN_SUCCESS_ 	0
	#define LOGIN_FAILED_		-1
#endif

#define STOP_HOST_SENDER_TRUE 1
#define STOP_HOST_SENDER_FALSE 0
#define USERNAME_LEN 31

/**SERVER STATUS*/
#define IPC_SERVER_INIT		0x01
#define IPC_SERVER_READY	0x02
#define IPC_SERVER_RUN		0x03
#define IPC_SERVER_STOPED	0x02

#define PJ_SIGNED_ARRAY_SIZE(a)	((int)PJ_ARRAY_SIZE(a))

/** Status is OK. */
#define TS_SUCCESS  0

/** Status is FAILED. */
#define TS_FAILED  -1

/** True value. */
#define TS_TRUE	    1

/** False value. */
#define TS_FALSE    0

/** invalid value. */
#define TS_INVALID    -1


/*
 *create the user container.
*/
struct user_sdp_t
{
    //client address
    char addr[48]    ;
    int32_t port;				//client port
    char sdp[1024];			//SDP
    char flag;				//pair flag,'s'=single;'p'=paired
    char client_name[USERNAME_LEN+1];        //mobile client username
    user_sdp_t()
    {
        memset(addr, 0, sizeof(addr));
        port = 0;
        memset(sdp, 0, sizeof(sdp));
        flag = 0;
        memset(client_name, 0, sizeof(client_name));
    }
};

typedef struct home_t
{
    int32_t maxUsers;		//max user number in a home, except host user.
}
ts_home;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <app/common/md5_tools.h>
#include <app/common/rsa_tools.h>
/*
 *user info
*/
struct user_info
{
    char username[USERNAME_LEN+1];	//username
    char pwd[MD5_LEN+1];			//password
    int32_t home_id;				//home id
    int32_t session_id;			//session id
    std::vector<user_sdp> sdpVec;
    char aeskey[16];			//AES key
    char stop_host_sender;		//stop host net sender flag
    char reference;				//
    ts_home home;				//home reference
    user_info()
    {
        memset(username, 0, sizeof(username));
        memset(pwd, 0, sizeof(pwd));
        home_id=0;
        session_id = 0;
        //sdpVec.clear();
        memset(aeskey, 0, sizeof(aeskey));
        stop_host_sender = 0;
        reference = 0;
        memset(&home, 0, sizeof(ts_home));
    }
};


/**tshome protocol handler*/
struct tsp_hanlder_t
{
    /**global rsa instance*/
    RSA *rsa;

    /**request handler*/
    int (*handle_request)(pj_turn_srv *srv,
                          const char *username,
                          const unsigned int un_len,
                          const pj_uint8_t *rcv_buff,
                          const unsigned int rcv_len,
                          const char *addr,
                          const unsigned int addr_len,
                          const unsigned int _port,
                          pj_uint8_t *_snd_buff,
                          unsigned int *snd_len,
                          user_sdp *sdp);

    /**response handler*/
    int (*handle_response)(pj_turn_srv *srv,
                           const unsigned char *rcv_buff,
                           const unsigned int rcv_len);

    /**timeout handler*/
    int (*handle_timeout)(pj_turn_srv *srv,
                          const char *username,
                          const unsigned int un_len,
                          const char *ip,
                          const int ip_len,
                          const int _port,
                          user_sdp *sdp);

    /**destroy handler*/
    int (*handle_destroy)(pj_turn_srv *srv,
                          const char *username,
                          const unsigned int un_len,
                          const char *ip,
                          const int ip_len,
                          const int _port,
                          user_sdp *sdp);

    /**update sth*/
    void (*update)(int field, void *value);

};

/**IPC protocol handler*/
struct ipc_handler_t
{
    /**use in IPC with web*/
    aes_encode aes_ins;

    /**incoming message handler*/
    u_char (*incoming_handler)(aes_encode aes,
                               const u_char *rcv_buf,
                               void **structs);

    /**outgoing message handler*/
    int (*outgoing_handler)(aes_encode aes,
                            u_char *snd_buf,
                            const void *structs,
                            const IPC_FUN_CODE fun);

    /**destroy handler*/
    void (*destroy)(ipc_handler *handler);
};


struct udb_manager_t
{
    /** Object name */
    char	*obj_name;

    int sessionId;

    std::map<std::string, ts_user> tempUserMap;

    std::map<int, std::vector<ts_user> > hostUserMap;

    std::map<int, std::vector<ts_user> > mobileUserMap;

    std::map<int, int> sessionHomeMap;

    struct
    {
        /** Mutex */
        //pj_lock_t	*lock;
        pthread_mutex_t lock;
    void (*destroy)
        (void);
    }
    core;

    struct
    {
        /**select user from database*/
        int (*select_user)(const char *username, const unsigned un_len, ts_user &user);

        /**search user from temporary user hash table*/
        ts_user *(*search_user)(const char *username, unsigned un_len, ts_user &user);

        /**verify user*/
        int (*verify_usr)(const char *username,
                          unsigned u_len,
                          const char *pwd,
                          unsigned p_len);

        /**get user's password*/
        void (*get_passwd)(const char *username, int u_len, char  **pwd);

        /**generate user session id*/
        int (*gen_usr_session_id)();

        /**delete a user*/
        void (*remove_usr)(const char *username, unsigned u_len);
    }
    user;

    struct
    {
        /**save a host user*/
        ts_user *(*save_host)(int home_id, ts_user *host_user);

        /**save a mobile user*/
        ts_user *(*save_mobile)(int home_id, ts_user *mobile_user);

        /**get all host user of the home*/
        int (*get_hosts)(int home_id, std::vector<ts_user> & hosts);

        int (*set_hosts)(int home_id, std::vector<ts_user> hosts);

        /**get all mobiles of the home*/
        int (*get_mobiles)(int home_id, std::vector<ts_user> & mobiles);

        int (*set_mobiles)(int home_id, std::vector<ts_user> & mobiles);

        /**delect a host user*/
        void (*remove_host)(int home_id, int session_id);

        /**delect a mobile user*/
        void (*remove_mobile)(int home_id, int session_id);

        /**user pair. mobile:self_type = 1; host:self_type = 2*/
        int (*pair_user)(int home_id, int self_type, std::vector<ts_user> & paired);
    }
    home;

    struct
    {
        /**get home id bu session id*/
        int (*get_home_id)(int session_id);

        /**save a mapping*/
        void (*put_session_home_id)(int session_id, int home_id);

        /**delete a mapping*/
        void (*remove_mapping)(int session_id);
    }
    mapping;

    struct
    {
        /**get all host user of the home by deviceId*/
        int (*get_hosts_by_device_id)(const char *device_id, std::vector<ts_user> & hosts);
    }
    jstelcom;
};

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#endif /*__TSHOME_TYPEDEF_H__*/

