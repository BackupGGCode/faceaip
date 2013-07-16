/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigSmsOverHttp.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigSmsOverHttp.h 5884 2012-09-07 10:46:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-07 10:46:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __ConfigSmsOverHttp_H__
#define __ConfigSmsOverHttp_H__

#include "ConfigBase.h"
//!
typedef struct tagConfigSmsOverHttp
{
    int iSmsOverHttpPort;// �������˿�

    // URL of webservice
    std::string strURL_registerURL               ;
	
    // flowing reserved
    std::string strURL_requestAuth							;
    std::string strURL_requestAuthByHost							;
    std::string strURL_heartbeat   ;
    std::string strURL_stopHostNetwork               ;
    std::string strURL_notifyUpdate                    ;
    std::string strURL_getDeviceStateList               ;
    std::string strURL_controlDevice                    ;
    std::string strURL_getPubIP4                    ;

}
ConfigSmsOverHttp;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigSmsOverHttp, 1, 4> CConfigSmsOverHttp;

#endif //__ConfigSmsOverHttp_H__
