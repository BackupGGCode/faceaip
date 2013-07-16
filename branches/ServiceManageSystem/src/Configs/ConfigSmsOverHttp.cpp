/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigSmsOverHttp.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigSmsOverHttp.cpp 5884 2012-09-07 10:46:00Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-07 10:46:00  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "ConfigSmsOverHttp.h"

template<>
void exchangeTable<ConfigSmsOverHttp>(CConfigExchange& xchg, CConfigTable& table, ConfigSmsOverHttp& config, int index, int app)
{
    xchg.exchange(table, "SmsOverHttpPort"    , config.iSmsOverHttpPort, 0, 65535 ,60088);

    // this module ÊÕµ½µÄ
    xchg.exchange(table, "registerURL", config.strURL_registerURL, "/SmsOverHttp/registerURL");

    xchg.exchange(table, "requestAuth", config.strURL_requestAuth, "/SmsOverHttp/requestAuth");
    xchg.exchange(table, "requestAuthByHost", config.strURL_requestAuthByHost, "/SmsOverHttp/requestAuthByHost");
    
    xchg.exchange(table, "heartbeat", config.strURL_heartbeat, "/SmsOverHttp/heartbeat");
    xchg.exchange(table, "stopHostNetwork", config.strURL_stopHostNetwork, "/SmsOverHttp/stopHostNetwork");
    xchg.exchange(table, "notifyUpdate", config.strURL_notifyUpdate, "/SmsOverHttp/notifyUpdate");
    xchg.exchange(table, "getDeviceStateList", config.strURL_getDeviceStateList, "/SmsOverHttp/getDeviceStateList");
    xchg.exchange(table, "controlDevice", config.strURL_controlDevice, "/SmsOverHttp/controlDevice");

    xchg.exchange(table, "getPubIP4", config.strURL_getPubIP4, "/SmsOverHttp/getPubIP4");
}
