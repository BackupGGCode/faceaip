/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * SocketMsgQue.h - _explain_
 *
 * Copyright (C) 2011 WuJunjie(Joy.Woo@hotmail.com), All Rights Reserved.
 *
 * $Id: SocketMsgQue.h 5884 2012-07-19 09:20:26Z WuJunjie $
 *
 *  Explain:
 *     -
 *       ����ppsocket��Ϣ�������б���socket������ָ�룬���ݻ�����Ҫ��������
 *       �����÷��ο��� CGeneralSocketProcessor
 *     -
 *
 *  Update:
 *     2012-07-19 09:20:26  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _SocketMsgQue_H_
#define _SocketMsgQue_H_

#if defined(WIN32)
	#pragma warning (disable : 4786)
#endif
#include <string.h>
#include <list>
#include "Socket.h"
#include "EZMutex.h"
#include "EZThread.h"
#include "EZSemaphore.h"
#include "EZPoolAllocator.h"

#define MSG_PRIORITY	10

class SOCKET_MSG_NODE
{
public:
    unsigned int		msg;
    Socket *pSocket;
    char *pData;
    int iLen;
    struct sockaddr sa_from;
    socklen_t sa_len;
    time_t		time;

    SOCKET_MSG_NODE(int iLen=0)
    {
        msg = 0;
        pSocket  =NULL;
        iLen = 0;
        pData = NULL;

        memset(&sa_from, 0, sizeof(struct sockaddr));
        sa_len = 0;
        time = 0;

    }
    ~SOCKET_MSG_NODE()
    {
    }
}
;

typedef std::list<SOCKET_MSG_NODE, pool_allocator<SOCKET_MSG_NODE> > SOCKET_MSGQUEUE;

class CSocketMsgQue
{
public:
    // if size==0 , do not limit
    CSocketMsgQue(int size = 1024);
    virtual ~CSocketMsgQue();

    int SendMessage (unsigned int	 msg,Socket *socket=NULL, const char *p=NULL,size_t l=0,struct sockaddr *sa_from=NULL,socklen_t sa_len=0);
    int RecvMessage (SOCKET_MSG_NODE *pMsg, BOOL wait = TRUE);
    void QuitMessage ();
    void ClearMessage();
    int GetMessageCount();
    int GetMessageSize();
    void SetMessageSize(int size);

protected:
private:
    SOCKET_MSGQUEUE m_Queue;
    bool m_bMsgFlg;
    CEZMutex m_Mutex;
    CEZSemaphore m_Semaphore;
    int m_nMsg;
    int m_nMaxMsg;
};

#endif// _SocketMsgQue_H_
