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
#include <pjlib.h>
#include <Parse.h>
#include "str_opr.h"
#ifdef USE_ENOCEAN
#include "../../Device/EnOcean/enocean_mgr.h"
#endif

#ifdef USE_EZCONFIG_LIBS
#include "../Configs/ConfigManager.h"
#endif

#include "GeneralConsoleOverTcp.h"
#include "GeneralAgentHandler.h"
#include "../Solar.h"
#include "../Logs.h"

#include "app.h"
extern pj_caching_pool g_cp;
#include "GeneralSocketProcessor.h"

// prompt
#define CONSOLE_PROMPT "[tshome-p2p-server]$ "
#define TOKEN_ConsoleOverTcp "==================="

CGeneralConsoleOverTcp::CGeneralConsoleOverTcp(ISocketHandler& h, std::string strSocketName)
        :TcpSocket(h)
{
    SetLineProtocol();

    m_enLoginStatus = LoginStatusNotLogin;

    SetSockName(strSocketName);

    m_strLastCmd = "";
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

    tprintf(" datetime : %s\n", get_date_time_string());

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
                && m_strUName=="aaa")
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
        tprintf(" exit,quit,q ...... quit debug.\n");
        tprintf(" status, st  ...... show socket fd.\n");
        tprintf(" thread,tr   ...... show thread info.\n");
        tprintf(" timer,ti    ...... show timer info.\n");
        tprintf(" exit,x    ...... exit, Suicide.\n");

        tprintf(" dpj        ...... dump_pjturn_status.\n");
		
        tprintf(" ctrl        ...... control device.\n");
    }
    else if (cmd == "status" || cmd == "st")
    {
        time_t ttStart =  g_Solar.GetStartTime();
        unsigned int ttLast = g_Solar.GetRunPeriod();
		
        tprintf(" Sys Status:\n%s\n", TOKEN_ConsoleOverTcp);
        tprintf("\t%-8s%s\n", "Ver:", g_Solar.GetVersion().c_str());
        tprintf("\t%-8s%s", "Start:", ctime(&ttStart)); 
        tprintf("\t%-8s%ld(h) %ld(m) %ld(s) -- %ld(sec)\n", "Run:", ttLast/3600, (ttLast>3600?( (ttLast-( ((int)(ttLast/3600))*3600))/60):(ttLast/60)), ttLast%60, ttLast);

        tprintf("\n Socket Status:\n%s\n", TOKEN_ConsoleOverTcp);
        static_cast<GeneralAgentHandler&>(Handler()).List(this);
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
        tprintf("DumpPjturnStatus ...\n%s\n", TOKEN_ConsoleOverTcp);

        DumpPjturnStatus(g_GeneralSocketProcessor.GetPJ_server());
    }
    else if (cmd == "ctrl")
    {
        tprintf("control device ...\n");

        //g_GeneralSocketProcessor.ControlDevice(NULL, 0);
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



void CGeneralConsoleOverTcp::DumpPjturnStatus(pj_turn_srv *srv)
{
    if (NULL == srv)
    {
        tprintf("pjturn server not ready.\n");
        return;
    }
    char addr[80];
    pj_hash_iterator_t itbuf, *it;
    pj_time_val now;
    unsigned i;

    for (i=0; i<srv->core.lis_cnt; ++i)
    {
        pj_turn_listener *lis = srv->core.listener[i];
        tprintf("Server address : %s\n", lis->info);
    }

    tprintf("Worker threads : %d\n", srv->core.thread_cnt);
    tprintf("Total mem usage: %u.%03uMB\n", (unsigned)(g_cp.used_size / 1000000),
            (unsigned)((g_cp.used_size % 1000000)/1000));
    tprintf("UDP port range : %u %u %u (next/min/max)\n", srv->ports.next_udp,
            srv->ports.min_udp, srv->ports.max_udp);
    //tprintf("TCP port range : %u %u %u (next/min/max)\n", srv->ports.next_tcp,
    //        srv->ports.min_tcp, srv->ports.max_tcp);
    tprintf("Clients #      : %u\n", pj_hash_count(srv->tables.alloc));

    tprintf("\n");

    if (pj_hash_count(srv->tables.alloc)==0)
    {
        return;
    }

    tprintf("#    Client addr.          Alloc addr.            Username Lftm Expy #prm #chl\n");
    tprintf("------------------------------------------------------------------------------\n");

    pj_gettimeofday(&now);

    it = pj_hash_first(srv->tables.alloc, &itbuf);
    i=1;
    while (it)
    {
        pj_turn_allocation *alloc = (pj_turn_allocation*)
                                    pj_hash_this(srv->tables.alloc, it);
        tprintf("%-3d %-22s %-22s %-8.*s %-4d %-4ld %-4d %-4d\n",
                i,
                alloc->info,
                pj_sockaddr_print(&alloc->relay.hkey.addr, addr, sizeof(addr), 3),
                (int)alloc->cred.data.static_cred.username.slen,
                alloc->cred.data.static_cred.username.ptr,
                alloc->relay.lifetime,
                alloc->relay.expiry.sec - now.sec,
                pj_hash_count(alloc->peer_table),
                pj_hash_count(alloc->ch_table));

        it = pj_hash_next(srv->tables.alloc, it);
        ++i;
    }
}

