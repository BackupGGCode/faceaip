/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigConsoleOverTcp.h - _explain_
 *
 * Copyright (C) 2013 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigConsoleOverTcp.h 5884 2013-06-07 11:28:25Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-06-07 11:28:25  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef __ConfigConsoleOverTcp_H__
#define __ConfigConsoleOverTcp_H__

#include "ConfigBase.h"
//!	
typedef struct tagConfigConsoleOverTcp
{
    int SPort;// �˿�
 
    // �ն�״̬ά��/ �������
    // 0- �����
    // >0 ��
    int CheckPeriod;
    // ��ʱʱ�� 
    int PeerTimeout;
}
ConfigConsoleOverTcp;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigConsoleOverTcp, 1, 4> CConfigConsoleOverTcp;

#endif //__ConfigConsoleOverTcp_H__
