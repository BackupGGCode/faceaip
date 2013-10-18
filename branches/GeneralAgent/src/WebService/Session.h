/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Session.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: Session.h 5884 2013-02-26 09:38:52Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-02-26 09:38:52  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "EZThread.h"


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

//
#define SESSION_TIMEOUT_DEFAULT 300

class CSession
{
public:
    CSession();
    ~CSession();

    std::vector<int> m_vAccess;

    std::string m_strSession;

    std::string m_strUName;
    time_t m_ttLastRequestTime;

    int m_iTimeout;

    /* = */
    CSession& operator = (const CSession& obj);

    /* == */
    bool operator==(const CSession& obj) const;

    /* != */
    bool operator!=(const CSession& obj) const;
    /* printf info when debugging */
    void Dump();
} ;

class CSessionManager
{
public:
    CSessionManager();
    ~CSessionManager();
    // ��session��
    int newSession(const CSession &newSession);
    int deleteSession(std::string strSession);
    int findSession(std::string strSession, CSession &newSession);

    int Maintain();

private:

    CEZMutex m_mutexmapSession;

    std::map<std::string , CSession> m_mapSession;
};
