/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigWebService.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigWebService.h 5884 2012-09-07 10:46:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-07 10:46:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __CONFIGWEBSERVICE_H__
#define __CONFIGWEBSERVICE_H__

#include "ConfigBase.h"
//!
typedef struct tagConfigWebService
{
    int iWebServicePort;// �������˿�

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
ConfigWebService;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigWebService, 1, 4> CConfigWebService;

#endif //__CONFIGSMARTHOMETELCOM_H__
