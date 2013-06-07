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

#include <Parse.h>
#ifdef USE_ENOCEAN
#include "../../Device/EnOcean/enocean_mgr.h"
#endif

#ifdef USE_EZCONFIG
#include "Configs/ConfigManager.h"
#endif

#include "GeneralConsoleOverTcp.h"
#include "GeneralAgentHandler.h"

// prompt
#define CONSOLE_PROMPT "[GeneralAgent]$ "

CGeneralConsoleOverTcp::CGeneralConsoleOverTcp(ISocketHandler& h, std::string strSocketName)
        :TcpSocket(h)
{
    SetLineProtocol();

    m_enLoginStatus = LoginStatusNotLogin;

    SetSockName(strSocketName);
}

void CGeneralConsoleOverTcp::OnAccept()
{
    time_t timep;
    struct tm *p;
    time (&timep);
    p=localtime(&timep);

    Send(" hostname : " + Utility::GetLocalHostname() + "\n");
    Send(" address  : " + Utility::GetLocalAddress() + "\n");
    //tprintf(" datetime : %s\n", asctime(gmtime(&timep)));

    tprintf(" datetime : %d-%d-%d %d:%d:%d\n"
            , (1900+p->tm_year)
            , ( 1+p->tm_mon)
            , p->tm_mday
            , p->tm_hour
            , p->tm_min
            , p->tm_sec);

    Send("Number of sockets in list : " + Utility::l2string(Handler().GetCount()) + "\n");
    //Send("\n");

    tprintf("\n(none) login:");
    //Send(CONSOLE_PROMPT);
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
void CGeneralConsoleOverTcp::OnLine(const std::string& line)
{

    DBG_CODE(
        std::cout << line << std::endl << "m_enLoginStatus:";
        std::cout << (int)m_enLoginStatus << " | " <<std::endl;
    );
    if (line.size() > 0)
    {

        if (LoginStatusNotLogin == m_enLoginStatus)
        {
            m_strUName = line;
            m_enLoginStatus = LoginStatusWaitPwd;
        }
        else if (LoginStatusWaitPwd == m_enLoginStatus)
        {
            m_strUPwd = line;
            ////////
            bool bAccountisOK = false;

            // 示例，相等即可
            if (m_strUName == m_strUPwd)
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
    Parse pa(line, " ");
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
    else if (line.size()<1)
    {
        tprintf("press h for help\n");
    }
    else if (cmd == "help" || cmd == "h")
    {
        tprintf(" help,h      ...... show cmd list.\n");
        tprintf(" exit,quit,q ...... quit debug.\n");
        tprintf(" status, st  ...... show socket fd.\n");
        tprintf(" thread,tr   ...... show thread info.\n");
        tprintf(" timer,ti    ...... show timer info.\n");
        tprintf(" entt        ...... enocean_start_teach_in.\n");
        tprintf(" ents        ...... enocean_stop_teach_in.\n");

        tprintf(" showhttpc   ...... show http config.\n");
        tprintf(" serverip    ...... set server ip address.\n");
        
        tprintf(" reboot      ...... host reboot.\n");
    }
    else if (cmd == "status" || cmd == "st")
    {
        static_cast<GeneralAgentHandler&>(Handler()).List(this);
    }
    else if (cmd == "exit" || cmd == "quit" || cmd == "q")
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
#ifdef USE_ENOCEAN
    else if (cmd == "entt")
    {
        tprintf(" enocean_start_teach_in... ");
        enocean_start_teach_in();
        tprintf(" done.\n");
    }
    else if (cmd == "ents")
    {
        tprintf(" enocean_stop_teach_in... ");
        enocean_stop_teach_in();
        tprintf(" done.\n");
    }
#endif //USE_ENOCEAN
#ifdef USE_EZCONFIG
    else if (cmd == "showhttpc")
    {
        tprintf(" show http config...\n");

        //CConfigHttpClient __cfg;
        //__cfg.update();

        //tprintf( "OperateURL:[%s]\n", __cfg.getConfig().OperateURL.c_str());
        //tprintf( "StatusURL:[%s]\n", __cfg.getConfig().StatusURL.c_str());
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

            //CConfigHttpClient __cfg;
            //__cfg.update();
            //__cfg.getConfig().OperateURL = strProcessURL;
            //__cfg.getConfig().StatusURL = strStatusURL;
            //__cfg.commit();

            //tprintf( "OperateURL:[%s]\n", __cfg.getConfig().OperateURL.c_str());
            //tprintf( "StatusURL:[%s]\n", __cfg.getConfig().StatusURL.c_str());
            //tprintf(" done.\n");
        }
        else
        {
            tprintf(" invalid arg:%s\n", arg.c_str());
        }


    }
#endif //USE_EZCONFIG
    else if (cmd == "reboot")
    {
        tprintf("rebooting host ...\n");

        system("reboot");
    }
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
