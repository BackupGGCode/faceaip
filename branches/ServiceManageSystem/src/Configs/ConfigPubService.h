/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigPubService.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigPubService.h 5884 2012-09-07 10:46:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-07 10:46:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __ConfigPubService_H__
#define __ConfigPubService_H__

#include "ConfigBase.h"
//!
typedef struct tagConfigPubService
{
    int iPubServicePort;// �������˿�

    // URL of webservice
    std::string strURL_requestAuth							;
    std::string strURL_requestAuthByHost							;
    std::string strURL_heartbeat   ;
    std::string strURL_registerURL               ;
    std::string strURL_stopHostNetwork               ;
    std::string strURL_notifyUpdate                    ;
    std::string strURL_getDeviceStateList               ;
    std::string strURL_controlDevice                    ;
    std::string strURL_getPubIP4                    ;

}
ConfigPubService;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigPubService, 1, 4> CConfigPubService;

#endif //__ConfigPubService_H__
