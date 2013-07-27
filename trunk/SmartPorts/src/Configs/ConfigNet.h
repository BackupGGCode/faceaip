/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigNet.h - brief
 *
 * Copyright (C) 2013 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigNet.h 5884 2013-07-15 10:32:42Z WuJunjie $
 *
 *  Notes:
 *     -
 *      explain
 *     -
 *
 *  Update:
 *     2013-07-15 10:32:42 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __ConfigNet_H__
#define __ConfigNet_H__

#include "ConfigBase.h"

// very important, max interface I support
#define MAX_NETWORK_INTERFACE_NUM 4

//!	��ͨ����
typedef struct tagCONFIG_NET
{
    // network
    std::string strPrimaryDNS;//dns1
    std::string strSecondaryDNS;//dns2

    // ��ϵͳ��ʼ����ʱ������Ϊ��ȷ����ֵ
    int iNetworkInterfaceNum;//total num of lan wifi etc

    int iGarpPeriod;//garp >=0, 0-disable

    int iSshdPort;// >=0, 0-disable
}
CONFIG_NET;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<CONFIG_NET, 1, 4> CConfigNet;

#endif //__ConfigNet_H__

