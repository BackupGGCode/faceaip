/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * SocketMsgQue.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: SocketMsgQue.h 5884 2012-07-19 09:20:26Z WuJunjie $
 *
 *  Explain:
 *     -
 *       ����ppsocket��Ϣ�������б���socket������ָ�룬���ݻ�����Ҫ��������
 *       �����÷��ο��� CTeleSerialSocketProcessor
 *     -
 *
 *  Update:
 *     2012-07-19 09:20:26  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "SocketMsgQue.h"
#include "EZLock.h"

CSocketMsgQue::CSocketMsgQue(int size /* = 1024 */)// : m_Queue(size)
{
    m_nMaxMsg = size;
    m_nMsg = 0;
    m_bMsgFlg = true;
}

CSocketMsgQue::~CSocketMsgQue()
{}

BOOL CSocketMsgQue::SendMessage(unsigned int msg,Socket *pSocket, const char *p,size_t l,struct sockaddr *sa_from/*=NULL*/,socklen_t sa_len/*=0*/)
{
    SOCKET_MSG_NODE l_MSG;
    SOCKET_MSGQUEUE::iterator pi;

    m_Mutex.Lock();
    if(m_nMaxMsg>0
       && m_nMsg >= m_nMaxMsg)
    {
        m_Mutex.Unlock();
        return FALSE;
    }
    if (!m_bMsgFlg)
    {
        m_Mutex.Unlock();
        return FALSE;
    }

    /************************************************************************
    	������Ϣ:
    	1���������ȼ��Ѹ���Ϣ�������m_Queue��
    //	2���ڸö����в��Ҹ���Ϣ�ڵ㣬ֱ���Ҳ����ýڵ���˳�ѭ�����Ӹú������أ�
    ************************************************************************/
    l_MSG.msg = msg;
    l_MSG.pSocket = pSocket;
    l_MSG.pData = (char *)p;
    l_MSG.iLen = l;
    memcpy((char *)&l_MSG.sa_from, (char *)sa_from, sizeof(struct sockaddr));
    l_MSG. sa_len = sa_len;
    l_MSG.time = time(NULL);

    m_Queue.push_front(l_MSG);
    m_nMsg++;
    m_Mutex.Unlock();

    m_Semaphore.Post();
    return TRUE;
}

BOOL CSocketMsgQue::RecvMessage(SOCKET_MSG_NODE *pMsg, BOOL wait /* = TRUE */)
{
    /************************************************************************
    	������Ϣ������ȴ���һֱ�ȵ�����Ϣʱ���أ�����ֱ�ӷ��ء�
    	1������Ϣæ����m_QueueȡԪ�أ����ȡ�ɹ�����ֱ�ӷ��أ�
    	2������ѭ������Ϣæm_Queue��ȡԪ�أ�ֱ��ȡ�ɹ����˳�ѭ����
    ************************************************************************/

    if(wait)
    {
        m_Semaphore.Wait();
    }

    CEZLock guard(m_Mutex);
    //	trace("RecvMessage %d\n", m_nMsg);

    if (m_Queue.empty())
    {
        //		tracepoint();
        return FALSE;
    }
	#if 0
    if(!wait)
    {
        m_Semaphore.Wait();
    }
	#endif
    assert(m_nMsg);
    *pMsg = m_Queue.back();
    m_Queue.pop_back();
    m_nMsg--;

    return TRUE;
}

void CSocketMsgQue::QuitMessage ()
{
    CEZLock guard(m_Mutex);

    m_bMsgFlg = false;
}

void CSocketMsgQue::ClearMessage()
{
    CEZLock guard(m_Mutex);

    int n = m_nMsg;
    for(int i = 0; i < n; i++)
    {
        m_Semaphore.Wait();
        m_Queue.pop_back();
        m_nMsg--;
    }
}

int CSocketMsgQue::GetMessageCount()
{
    CEZLock guard(m_Mutex);

    return m_nMsg;
}

int CSocketMsgQue::GetMessageSize()
{
    CEZLock guard(m_Mutex);

    return m_nMaxMsg;
}

void CSocketMsgQue::SetMessageSize(int size)
{
    CEZLock guard(m_Mutex);

    m_nMaxMsg = size;
}
