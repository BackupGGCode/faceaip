/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralConsoleOverTcp.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralConsoleOverTcp.cpp 0001 2012-04-28 09:58:37Z WuJunjie $
 *
 *  Explain:
 *     -self dbg platform-
 *
 *  Update:
 *     2012-04-28 09:58:37 WuJunjie Create
 *     2012-05-02 12:38:49 WuJunjie add login process, name=pwd is ok here
 *     2013-06-08 11:17:39 WuJunjie add st function
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "CommonInclude.h"
#include <stdio.h>
#include <Utility.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <Parse.h>
#include "str_opr.h"
#include "EZThread.h"
#include "EZTimer.h"

#ifdef USE_ENOCEAN
#include "../../Device/EnOcean/enocean_mgr.h"
#endif

#ifdef USE_EZCONFIG
#include "ConfigManager.h"

#endif

#include "GeneralConsoleOverTcp.h"
#include "GeneralAgentHandler.h"
#include "GeneralAgent.h"

#include "../Logs.h"

#include "GeneralSocketProcessor.h"

#ifdef USE_MY_WEBSERVICE
#include "WebService/AgentWebService.h"
#endif //
#include "../SysMan/Maintenance.h"

#include "../Solar.h"
#include "LineProtocolCmd.h"
CmdList_T InputCmd[] =
    {
        /* ---------------------------------------------------------------------------------------------------------------------------------- */
        /* CMD                      COMMENT                         FUNC  ACK_CMD                    ACK_COMMENT                     ACK_FUNC */
        /* ---------------------------------------------------------------------------------------------------------------------------------- */
        {"help,h"                 , "show cmd list"              , NULL, "help,h"                 , "show cmd list"              , NULL},
        {"quit,q"                 , "quit debug"                 , NULL, "quit,q"                 , "quit debug"                 , NULL},
        {"status,st,d"            , "show socket fd"             , NULL, "status, st, d"          , "show socket fd"             , NULL},
        {"thread,tr"              , "show thread info"           , NULL, "thread,tr"              , "show thread info"           , NULL},
        {"timer,ti"               , "show timer info"            , NULL, "timer,ti"               , "show timer info"            , NULL},
        {"exit,x"                 , "exit, Suicide"              , NULL, "exit,x"                 , "exit, Suicide"              , NULL},
        {"sd,s"                   , "send data over named socket", NULL, "sd"                     , "send data over named socket", NULL},
        {"tc"                    , "test config"         , NULL, "tc"                    , "test my config interface"         , NULL},
        {"dpj"                    , "dump_pjturn_status"         , NULL, "dpj"                    , "dump_pjturn_status"         , NULL},
        {"user [a|all|homeId]"    , "dump user info"             , NULL, "user [a|all|homeId]"    , "dump user info"             , NULL},
        {"kick userName"          , "kick user"                  , NULL, "kick userName"          , "kick user"                  , NULL},
        {"ipc"                    , "dump IPC web servers info"  , NULL, "ipc"                    , "dump IPC web servers info"  , NULL},
        {"ctrl"                   , "control device"             , NULL, "ctrl"                   , "control device"             , NULL},
    };
#define INPUT_CMD_ITEMS sizeof(InputCmd)/sizeof(CmdList_T)

// prompt
#define CONSOLE_PROMPT "[GeneralConsoleOverTcp]$ "
#define TOKEN_ConsoleOverTcp "========================================================="
#define SUB_TOKEN_ConsoleOverTcp "------------------------------"

CGeneralConsoleOverTcp::CGeneralConsoleOverTcp(ISocketHandler& h, std::string strSocketName)
        :TcpSocket(h)
{
    SetLineProtocol();

    m_enLoginStatus = LoginStatusNotLogin;

    //SetSockName(strSocketName);

    m_strLastCmd = "";

    m_ttLastUpdate = 0;
    m_bAutoDumpStatus = false;
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

    tprintf("\n(none) login:");

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
                /*&& m_strUName=="yourPwd"*/)
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
        for (size_t ii=0; ii<INPUT_CMD_ITEMS; ii++)
        {
            tprintf(" %-20s ...... %s.\n", InputCmd[ii].pCmd, InputCmd[ii].pCmdComment);
        }
    }
    else if (cmd == "status" || cmd == "st" || cmd == "d")
    {
        //tprintf("DumpGeneralStatus ...\n%s\n", TOKEN_ConsoleOverTcp);

        DumpGeneralStatus(/*g_GeneralSocketProcessor.GetPJ_server()*/);

        m_bAutoDumpStatus = !m_bAutoDumpStatus;

        //tprintf("m_bAutoDumpStatus:%d\n", m_bAutoDumpStatus);

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
    else if (cmd == "tc")
    {
        tprintf(" test config...\n");
        TestConfigs(cmd, arg);
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
        }
        else
        {
            tprintf(" invalid arg:%s\n", arg.c_str());
        }


    }
    else if (cmd == "dpj")
    {
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
    else if (cmd == "ctrl")
    {
        tprintf("control device ...\n");

        //g_GeneralSocketProcessor.ControlDevice(NULL, 0);
    } // else if (cmd == "ctrl")
    else if (cmd == "sd" || cmd == "s")
    {
        tprintf("send data over named socket ...\n");

        Parse mypa(arg, " ");
        std::string strName = mypa.getword();
        std::string strSend = mypa.getrest();

        g_GeneralAgent.Send2Socket(strName, strSend.c_str(), strSend.size());

        tprintf("Send2Socket(%s, %s, %d) has been sended.\n", strName.c_str(), strSend.c_str(), strSend.size());
    }
    else if (cmd == "ttst")
    {
        tprintf("ttst ...\n");

    } // else if (cmd == "ttst")
    else
    {}

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
    //printf("m_bAutoDumpStatus:%d m_bAutoDumpUser:%d\n", m_bAutoDumpStatus, m_bAutoDumpUser);

    // 防止过于频繁
    time_t ttNow = time(NULL);

    if (ttNow-m_ttLastUpdate<=2)
    {
        return;
    }

    m_ttLastUpdate = ttNow;

    if (m_bAutoDumpStatus)
        DumpGeneralStatus(/*g_GeneralSocketProcessor.GetPJ_server()*/);

    if (m_bAutoDumpUser)
        DumpUserInfo("a");
}

void CGeneralConsoleOverTcp::DumpGeneralStatus()
{
    //tprintf("DumpGeneralStatus\n");
    //tprintf("------------------------------------------------------------------------------\n");

    time_t ttStart =  g_Solar.GetStartTime();
    unsigned int ttLast = g_Solar.GetRunPeriod();

    tprintf(" Sys Status:\n%s\n", TOKEN_ConsoleOverTcp);
    tprintf("\t%-8s%s\n", "Ver:", /*g_GeneralAgent.GetVersion().c_str()*/"hh");
    tprintf("\t%-8s%s", "Start:", ctime(&ttStart));
    tprintf("\t%-8s%ld(h) %ld(m) %ld(s) -- %ld(sec)\n", "Run:", ttLast/3600, (ttLast>3600?( (ttLast-( ((int)(ttLast/3600))*3600))/60):(ttLast/60)), ttLast%60, ttLast);

    tprintf("\tIsOnInternet:%s\n", g_Solar.IsOnInternet()?"ON":"OFF");
    tprintf("\tNAT IP:%s\n", g_Solar.IsOnInternet()?g_Solar.GetNatIP():"NotValid");
	
    tprintf("\n Socket Status:\n%s\n", TOKEN_ConsoleOverTcp);
    static_cast<GeneralAgentHandler&>(Handler()).List(this);

#ifdef USE_MY_WEBSERVICE

    tprintf(" g_AgentWebService:\n%s\n", TOKEN_ConsoleOverTcp);
    g_AgentWebService.GetWebServiceHandler()->List(this);
#endif
#ifdef FUNCTION_DATETIME_AND_NTP
    char pBuff[32] ;

    tprintf(" Ntp Status:\n%s\n", SUB_TOKEN_ConsoleOverTcp);

    time2string(g_Maintenance.GetLastUpdate(), pBuff, NULL);
    tprintf("\t Last Succeed at:%s, times:%d\n", pBuff, g_Maintenance.GetRunTimerProcSendNTPClient_Succ());
    time2string(g_Maintenance.GetLastUpdateFailed(), pBuff, NULL);
    tprintf("\t Last Failed  at:%s, times:%d\n", pBuff, g_Maintenance.GetRunTimerProcSendNTPClient_Failed());

#endif //FUNCTION_DATETIME_AND_NTP
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

void CGeneralConsoleOverTcp::TestConfigs(const std::string &strCmd, const std::string &strArg)
{
    CConfigGeneral __cfgGeneral;
    __cfgGeneral.update();

    __fline;
    tprintf(" TestConfigs %s %s\n", strCmd.c_str(), strArg.c_str());

    __cfgGeneral.getConfig().iLocalNo++;
    __cfgGeneral.commit();

}
