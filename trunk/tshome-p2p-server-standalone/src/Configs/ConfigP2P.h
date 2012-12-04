/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigP2P.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: ConfigP2P.h 5884 2012-07-27 04:00:55Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-07-27 04:00:55  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __CONFIGP2P_H__
#define __CONFIGP2P_H__

#include "ConfigBase.h"

//!	http�ͻ���
typedef struct tagConfigP2P
{	
    // ��ȡ���� url
    int StunPort 		;
    int maxClient;
    // ��Сudp�˿�
    int minUdpPort;
    // ���udp�˿�
    int maxUdpPort;

    // �ն�״̬ά��/ �������
    // 0- �����
    // >0 ��
    int HostCheckPeriod;
	int TermCheckPeriod;
    // ������ʱʱ�� 
    //Ŀǰpj��ʱ��Ϊ300s�� ����������Ϊ300s���ϣ��򲻻���ҵ���������
    int HostTimeout;
    // �ն˳�ʱʱ��
    int TermTimeout;
} ConfigP2P;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<tagConfigP2P, 1, 4> CConfigP2P;

#endif //__CONFIGP2P_H__
