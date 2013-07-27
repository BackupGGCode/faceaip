/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigTcpSocketServer.h - _explain_
 *
 * Copyright (C) 2013 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigTcpSocketServer.h 5884 2013-06-05 05:11:14Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-06-05 05:11:14  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __ConfigTcpSocketServer_H__
#define __ConfigTcpSocketServer_H__

#include "ConfigBase.h"
//!	
typedef struct tagConfigTcpSocketServer
{
    int SPort;// �˿�
 
    // �ն�״̬ά��/ �������
    // 0- �����
    // >0 ��
    int CheckPeriod;
    // ��ʱʱ�� 
    int PeerTimeout;

// ��������
    std::string strSName;
}
ConfigTcpSocketServer;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigTcpSocketServer, 1, 4> CConfigTcpSocketServer;

#endif //__ConfigTcpSocketServer_H__
