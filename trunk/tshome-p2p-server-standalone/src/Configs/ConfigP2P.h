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

//!	http客户端
typedef struct tagConfigP2P
{	
    // 获取操作 url
    std::string strPort 		;
    int maxClient;
    // 最小udp端口
    int minUdpPort;
    // 最大udp端口
    int maxUdpPort;
} ConfigP2P;

//1 -结构数目
//4 -观察者最大数目
typedef TConfig<tagConfigP2P, 1, 4> CConfigP2P;

#endif //__CONFIGP2P_H__
