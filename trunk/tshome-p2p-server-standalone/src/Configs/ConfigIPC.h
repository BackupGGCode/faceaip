/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigIPC.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: ConfigIPC.h 5884 2012-07-27 04:02:19Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-07-27 04:02:19  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef __CONFIGIPC_H__
#define __CONFIGIPC_H__

#include "ConfigBase.h"
//!	
typedef struct tagConfigIPC
{
    int IpcPort;// �˿�
 
    // �ն�״̬ά��/ �������
    // 0- �����
    // >0 ��
    int CheckPeriod;
    // ��ʱʱ�� 
    int PeerTimeout;
}
ConfigIPC;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigIPC, 1, 4> CConfigIPC;

#endif //__CONFIGIPC_H__
