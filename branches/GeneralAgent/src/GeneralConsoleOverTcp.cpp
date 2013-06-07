/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralConsoleOverTcp.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralConsoleOverTcp.cpp 0001 2012-04-28 09:58:37Z WuJunjie $
 *
 *  Explain:
 *     -self dbg platform-
 *
 *  Update:
 *     2012-04-28 09:58:37 WuJunjie Create
 *     2012-05-02 12:38:49 WuJunjie add login process, name=pwd is ok here
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "CommonInclude.h"
#include <stdio.h>
#include <Utility.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
//#include <pjlib.h>
#include <Parse.h>
#include "str_opr.h"
#ifdef USE_ENOCEAN
#include "../../Device/EnOcean/enocean_mgr.h"
#endif

#ifdef USE_EZCONFIG
#include "Configs/ConfigManager.h"
//#include "../Configs/ConfigP2P.h"

#endif

#include "GeneralConsoleOverTcp.h"
#include "GeneralAgentHandler.h"
#include "GeneralAgent.h"
//#include "../SmartHomeTelcom/AgentSmartHomeTelcom.h"
//#include "../SmartHomeTelcom/HandlerSmartHomeTelcom.h"
//#include "../Solar.h"
#include "Logs.h"

//#include "app.h"
//extern pj_caching_pool g_cp;
#include "GeneralSocketProcessor.h"
//#include "../service/notification/myAPNS.h"
//#include "../SmartHomeTelcom/HttpKeepConnectorSmartHomeTelcom.h"

#ifdef USE_MY_WEBSERVICE
#include "WebService/AgentWebService.h"
#endif //
//extern ts_udb_mgr g_udb_mgr;
int hostTimeout;
int termTimeout;
// prompt
#define CONSOLE_PROMPT "[GeneralConsoleOverTcp]$ "
#define TOKEN_ConsoleOverTcp "========================================================="

CGeneralConsoleOverTcp::CGeneralConsoleOverTcp(ISocketHandler& h, std::string strSocketName)
        :TcpSocket(h)
{
    SetLineProtocol();

    m_enLoginStatus = LoginStatusNotLogin;

    //SetSockName(strSocketName);

    m_strLastCmd = "";

    //CConfigP2P m_configP2P;
   // m_configP2P.update();
    //hostTimeout = m_configP2P.getConfig().HostTimeout;
    //termTimeout = m_configP2P.getConfig().TermTimeout;

    m_ttLastUpdate = 0;
    m_bAutoDumpPJ = false;
    m_bAutoDumpUser = false;
}

void CGeneralConsoleOverTcp::OnAccept()
{
    time_t timep;
    struct tm *p;
    time (&timep);
    p=localtime(&timep);

    char strDT[GET_DATE_TIME_STRING_DEFAULT_STR_LEN];

    Send(" hostname : " + Utility::GetLocalHostname() + "\n");
    Send(" address  : " + Utility::GetLocalAddress() + "\n");
    //tprintf(" datetime : %s\n", asctime(gmtime(&timep)));

    tprintf(" datetime : %s\n", get_date_time_string(strDT, NULL));

    Send("Number of sockets in list : " + Utility::l2string(Handler().GetCount()) + "\n");
    //Send("\n");

    tprintf("\n(none) login:");
    //Send(CONSOLE_PROMPT);

    LOG4CPLUS_INFO(LOG_SOLAR, "CGeneralConsoleOverTcp::OnAccept:" << GetRemoteAddress() << ":" << GetRemotePort());
}

CGeneralConsoleOverTcp::~CGeneralConsoleOverTcp()
{
    LOG4CPLUS_INFO(LOG_SOLAR, "CGeneralConsoleOverTcp::Destroy:" << GetRemoteAddress() << ":" << GetRemotePort());
}

#define SIZE 5000
void CGeneralConsoleOverTcp::tprintf(const char *format, ...)
{
    va_list ap;
    size_t n;
    char tmp[SIZE];

    va_start(ap,format);
#ifdef _WIN32

    n = vsprintf(tmp,format,ap);
#else

    n = vsnprintf(tmp,SIZE - 1,format,ap);
#endif

    va_end(ap);

    SendBuf(tmp, strlen(tmp));
}
#include "EZThread.h"
#include "EZTimer.h"
void CGeneralConsoleOverTcp::OnLine(const std::string& InputLine)
{
    std::string CmdLine = InputLine;

    DBG_CODE(
        std::cout << CmdLine << std::endl << "m_enLoginStatus:";
        std::cout << (int)m_enLoginStatus << " | " <<std::endl;
    );


    if (m_enLoginStatus>LoginStatusWaitPwd)
    {
        if (CmdLine.size()<1)
        {
            CmdLine = m_strLastCmd;
        }
        else
        {
            m_strLastCmd = CmdLine;
        }
    }

    if (CmdLine.size() > 0)
    {

        if (LoginStatusNotLogin == m_enLoginStatus)
        {
            m_strUName = CmdLine;
            m_enLoginStatus = LoginStatusWaitPwd;
        }
        else if (LoginStatusWaitPwd == m_enLoginStatus)
        {
            m_strUPwd = CmdLine;
            ////////
            bool bAccountisOK = false;

            // 示例，相等即可
            if (m_strUName == m_strUPwd
                /*&& m_strUName=="92617564685949712105142592000"*/)
            {
                bAccountisOK = true;
            }
            /////////////
            if (bAccountisOK)
            {
                m_enLoginStatus = LoginStatusLoginUser;
            }
            else
            {
                m_enLoginStatus = LoginStatusNotLogin;
            }
            //
        }
    }

    DBG_CODE(
        std::cout << "After:" << (int)m_enLoginStatus << " | " <<std::endl;
    );
    Parse pa(CmdLine, " ");
    std::string cmd = pa.getword();
    std::string arg = pa.getrest();
    DBG_CODE(
        if (arg.size()>0)
    {
        std::cout << "cmd:" << cmd << std::endl;
        std::cout << "arg:" << arg << std::endl;
    }
)
    ;
    if (m_enLoginStatus < LoginStatusLoginUser)
    {
        // login
    }
    else if (CmdLine.size()<1)
    {
        tprintf("press h for help\n");
    }
    else if (cmd == "help" || cmd == "h")
    {
        tprintf(" help,h      ...... show cmd list.\n");
        tprintf(" quit,q ...... quit debug.\n");
        tprintf(" status, st  ...... show socket fd.\n");
        tprintf(" thread,tr   ...... show thread info.\n");
        tprintf(" timer,ti    ...... show timer info.\n");
        tprintf(" exit,x    ...... exit, Suicide.\n");

        tprintf(" dpj        ...... dump_pjturn_status.\n");

        tprintf(" user [a|all|homeId]        ...... dump user info.\n");
        tprintf(" kick userName        ...... kick user.\n");
        tprintf(" ipc        ...... dump IPC web servers info.\n");
        tprintf(" ctrl        ...... control device.\n");
        tprintf(" sd         ...... send data over named socket.\n");
    }
    else if (cmd == "status" || cmd == "st")
    {
        time_t ttStart =  g_GeneralAgent.GetStartTime();
        unsigned int ttLast = g_GeneralAgent.GetRunPeriod();

        tprintf(" Sys Status:\n%s\n", TOKEN_ConsoleOverTcp);
        tprintf("\t%-8s%s\n", "Ver:", /*g_GeneralAgent.GetVersion().c_str()*/"hh");
        tprintf("\t%-8s%s", "Start:", ctime(&ttStart));
        tprintf("\t%-8s%ld(h) %ld(m) %ld(s) -- %ld(sec)\n", "Run:", ttLast/3600, (ttLast>3600?( (ttLast-( ((int)(ttLast/3600))*3600))/60):(ttLast/60)), ttLast%60, ttLast);

        tprintf("\n Socket Status:\n%s\n", TOKEN_ConsoleOverTcp);
        static_cast<GeneralAgentHandler&>(Handler()).List(this);
		
#ifdef USE_MY_WEBSERVICE
        g_AgentWebService.GetWebServiceHandler()->List(this);
#endif
    }
    else if (cmd == "exit" || cmd == "x")
    {
        LOG4CPLUS_INFO(LOG_SOLAR, "suicided, exit process ...");
        tprintf(" suicided, exit process ...\n");
        exit(-1);
    }
    else if (cmd == "quit" || cmd == "q")
    {
        tprintf(" exit now...\n");
        this->SetCloseAndDelete();
    }
    else if (cmd == "timer" || cmd == "ti")
    {
        //tprintf(" exit now...\n");
        std::string __str;
        g_TimerManager.DumpTimers(__str);
        tprintf(__str.c_str());
    }
    else if (cmd == "thread" || cmd == "tr")
    {
        //tprintf(" exit now...\n");
        std::string __str;
        g_ThreadManager.DumpThreads(__str);
        tprintf(__str.c_str());
    }
#ifdef USE_EZCONFIG_LIBS
    else if (cmd == "showhttpc")
    {
        tprintf(" show http config...\n");

        //        CConfigHttpClient __cfg;
        //        __cfg.update();
        //
        //        tprintf( "OperateURL:[%s]\n", __cfg.getConfig().OperateURL.c_str());
        //        tprintf( "StatusURL:[%s]\n", __cfg.getConfig().StatusURL.c_str());
    }
    else if (cmd == "serverip")
    {
        tprintf("set server ip address\n");


        if (arg.size()>5)
        {

            std::string strProcessURL = "http://"+arg+"/exchange/get_operate_json_ts.php";
            std::string strStatusURL = "http://"+arg+"/exchange/submit_status_json_ts.php";
            std::cout << strProcessURL << std::endl;
            std::cout << strStatusURL << std::endl;

            //            CConfigHttpClient __cfg;
            //            __cfg.update();
            //            __cfg.getConfig().OperateURL = strProcessURL;
            //            __cfg.getConfig().StatusURL = strStatusURL;
            //            __cfg.commit();
            //
            //            tprintf( "OperateURL:[%s]\n", __cfg.getConfig().OperateURL.c_str());
            //            tprintf( "StatusURL:[%s]\n", __cfg.getConfig().StatusURL.c_str());
            //            tprintf(" done.\n");
        }
        else
        {
            tprintf(" invalid arg:%s\n", arg.c_str());
        }


    }
#endif //USE_EZCONFIG_LIBS
    else if (cmd == "dpj")
    {
        //tprintf("DumpPjturnStatus ...\n%s\n", TOKEN_ConsoleOverTcp);

        DumpPjturnStatus(/*g_GeneralSocketProcessor.GetPJ_server()*/);

        m_bAutoDumpPJ = !m_bAutoDumpPJ;

        //tprintf("m_bAutoDumpPJ:%d\n", m_bAutoDumpPJ);

    }
    else if (cmd == "user")
    {
        //tprintf("Dump user info ...\n%s\n", TOKEN_ConsoleOverTcp);
        Parse mypa(arg, " ");
        std::string home = mypa.getword();
        if(home == "" || home == "a" || home == "all")
        {
            home = "a";

            m_bAutoDumpUser= !m_bAutoDumpUser;

           // tprintf("m_bAutoDumpUser:%d\n", m_bAutoDumpUser);
        }
        DumpUserInfo(home);
    }
    else if (cmd == "kick")
    {
        Parse mypa(arg, " ");
        std::string name = mypa.getword();
        //Kick(name);
    }
    else if (cmd == "ipc")
    {
        //Parse mypa(arg, " ");
        //std::string name = mypa.getword();
        DumpIPCWebinfo();
    }
    else if (cmd == "push")
    {
        TestAPNSPush(arg);
    }
    else if (cmd == "ctrl")
    {
        tprintf("control device ...\n");

        //g_GeneralSocketProcessor.ControlDevice(NULL, 0);
    } // else if (cmd == "ctrl")
    else if (cmd == "sd")
    {
        tprintf("send data over named socket ...\n");

        Parse mypa(arg, " ");
        std::string strName = mypa.getword();
        std::string strSend = mypa.getword();

		g_GeneralAgent.Send2Socket(strName, strSend.c_str(), strSend.size());

		
        tprintf("Send2Socket(%s, %s, %d) has been sended.\n", strName.c_str(), strSend.c_str(), strSend.size());

        //g_GeneralSocketProcessor.ControlDevice(NULL, 0);
        
    } // else if (cmd == "ctrl")
    else if (cmd == "ttst")
    {
        tprintf("ttst ...\n");

        //g_GeneralSocketProcessor.ControlDevice(NULL, 0);
    } // else if (cmd == "ctrl")
    else
    {
        //static_cast<GeneralAgentHandler&>(Handler()).List(this);
    }

    // 最后发送提示符
    if (LoginStatusNotLogin == m_enLoginStatus)
    {
        tprintf("\n(none) login:");
    }
    else if (LoginStatusWaitPwd == m_enLoginStatus)
    {
        tprintf("\nPassword:");
    }
    else
    {
        tprintf(CONSOLE_PROMPT);
    }

}

void CGeneralConsoleOverTcp::Update()
{
    //printf("m_bAutoDumpPJ:%d m_bAutoDumpUser:%d\n", m_bAutoDumpPJ, m_bAutoDumpUser);

    // 防止过于频繁
    time_t ttNow = time(NULL);

    if (ttNow-m_ttLastUpdate<=2)
    {
        return;
    }

    m_ttLastUpdate = ttNow;

    if (m_bAutoDumpPJ)
        DumpPjturnStatus(/*g_GeneralSocketProcessor.GetPJ_server()*/);

    if (m_bAutoDumpUser)
        DumpUserInfo("a");
}

void CGeneralConsoleOverTcp::DumpPjturnStatus(/*pj_turn_srv *srv*/)
{
    tprintf("DumpPjturnStatus\n");
    tprintf("------------------------------------------------------------------------------\n");
}

char * timeFormat (time_t *utc_time, char *str_time, int str_len)
{
    struct tm *local_time = NULL;
    local_time = localtime(utc_time);
    memset(str_time, 0, str_len);
    strftime(str_time, str_len, "%m-%d %H:%M:%S", local_time);
    return str_time;
}

void CGeneralConsoleOverTcp::Kick(std::string userName)
{}

void CGeneralConsoleOverTcp::DumpIPCWebinfo()
{
    tprintf("DumpIPCWebinfo\n");
    tprintf("------------------------------------------------------------------------------\n");
}

void CGeneralConsoleOverTcp::DumpUserInfo(std::string home)
{
    tprintf("DumpUserInfo\n");
    tprintf("------------------------------------------------------------------------------\n");
}

int hexToInt(char *hex, int len)
{
    int i,tmp;
    int value = 0;
    for (i = 0 ; i < len; i++)
    {

        if (hex[i] >= '0' && hex[i] <= '9')
        {
            tmp = hex[i]-'0';
        }
        else
        {
            tmp = hex[i] - 87;
        }

        value += tmp << 4*(len-i-1);
    }
    return value;
}


void CGeneralConsoleOverTcp::TestAPNSPush(std::string msg)
{}


