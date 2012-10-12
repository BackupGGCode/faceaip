/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Solar.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: Solar.cpp 5884 2012-05-16 09:17:46Z WuJunjie $
 *
 *  Explain:
 *     -
 *      主调类
 *     -
 *
 *  Update:
 *     2012-05-16 09:17:46   WuJunjie  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "CommonInclude.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <pjlib.h>

#include <ez_fs.h>
#include "Logs.h"

#include "EZThread.h"
#include "EZTimer.h"

#include "Configs/ConfigManager.h"

#include "Solar.h"
#include <app/common/config.h>

#include "app/server/mysqlpool.h"


// config fname
const char * const ConfigFileSolar_1= "solar.cfg";
const char * const ConfigFileSolar_2 = "solar.cfg.duplicate";

const char * const copy_right = "@(#) Copyright (c) 2012 tiansu-china.com.  All rights reserved";
const char * const version_string = "v 0.9.2 - ";
const char * const desc="P_2012-7-24.Solar.1010";

const char * const INSTANCE_PID_FILE = "/tmp/tshome-p2p-server.pid";

int already_running(const char *filename);

#include "app.h"
#include "./IpcServer/GeneralAgent.h"
#include "IpcServer/GeneralSocketProcessor.h"
#ifdef USE_HTTPKEEPCONNECTORSMARTHOME
#include "SmartHomeTelcom/AgentSmartHomeTelcom.h"

#endif // USE_HTTPKEEPCONNECTORSMARTHOME
#ifdef USE_HTTPSERVERSMARTHOMETELCOMCTRLSERVER
#include "SmartHomeTelcom/SmartHomeTelcomCtrlServer/AgentSmartHomeTelcomCtrlServer.h"

#endif // 
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
PATTERN_SINGLETON_IMPLEMENT(CSolar);

CSolar::CSolar():m_TimerDBAlive("CSolar-KeepAlive-DB"), m_TimerMaintain("CSolar-Maintain")
{
    m_strVer = version_string;
    m_strVer += __DATE__;
    m_strVer += " ";
    m_strVer +=__TIME__;

    m_iRunPeriod = 0;
    m_ttStartTime = time(NULL);
}

CSolar::~CSolar()
{
    g_Config.saveFile();
}

void CSolar::ShowVersion()
{
    printf("---------------------------\n\n");
    printf("%s\n", copy_right);
    printf("%s\n", desc);
    printf("$ Solar: main.c,v 2.6 %s %s Exp $\n\n", __DATE__, __TIME__);
    printf("---------------------------\n");
}

void CSolar::Initialize(int argc, char * argv[])
{
    ShowVersion();
    SetPlatform();

    g_TimerManager.Start();
    g_ThreadManager.RegisterMainThread(ThreadGetID());

    //g_Config.useDoubleCfgFile(1);
    g_Config.initialize(ConfigFileSolar_1, ConfigFileSolar_2);
    InitializeLogs();
    g_Config.saveFile();

    LOG4CPLUS_INFO(LOG_SOLAR, "Starting(" << desc << ") ...");

//#if 0
    // comment it when development
#if 0
    LOG4CPLUS_INFO(LOG_SOLAR, "is already_running ...");
    if (already_running(INSTANCE_PID_FILE) != 0)
    {
        // important msg, write to console and log files.
        LOG4CPLUS_ERROR(LOG_SOLAR, "Allowed to run only one instance, pls check");
        LOG4CPLUS_INFO(LOG_SOLAR, "Exit ...");

        printf("Allowed to run only one instance, pls check\n");
        printf("Exit ...\n");

        exit(-1);
    }
#endif

    //begin -- start server
    // 启动顺序请慎重修改

    // 开启原服务器
    LOG4CPLUS_INFO(LOG_SOLAR, "StartPJTurnServer ...");
    StartPJTurnServer();

    // 启动处理线程
    LOG4CPLUS_INFO(LOG_SOLAR, "g_GeneralSocketProcessor ...");
    g_GeneralSocketProcessor.Start();

    LOG4CPLUS_INFO(LOG_SOLAR, "g_GeneralAgentTSIPC ...");
    g_GeneralAgentTSIPC.Start(NULL);
    
    //江苏电信版本的相关模块
#ifdef USE_HTTPKEEPCONNECTORSMARTHOME
    g_SmartHomeAgentTelcom.Start();
#endif

#ifdef USE_HTTPSERVERSMARTHOMETELCOMCTRLSERVER
    g_AgentSmartHomeTelcomCtrlServer.Start();
#endif

	// 启动维护定时器
	m_TimerDBAlive.Start(this
	                                , (TIMERPROC)&CSolar::TimerProcDBLive
	                                , 0
	                                , 3 * 1000 /*period ms*/
	                                , 0
	                                , 10*1000
	                               );
	m_TimerMaintain.Start(this
	                                , (TIMERPROC)&CSolar::TimerProcMaintain
	                                , 0
	                                , 1 * 1000 /*每秒执行一次，请勿修改*/
	                                , 0
	                                , 10*1000
	                               );
    //end -- start server

    // 启动完毕， 保存一次配置文件
    g_Config.saveFile();
    printf("%s Initialize end, running ...\n", argv[0]);
	
    LOG4CPLUS_INFO(LOG_SOLAR, argv[0] << " Initialize end");
}

extern int pj_runnable;
extern pj_thread_t *pt;
int sndto_client(void *data);
void CSolar::StartPJTurnServer()
{
    CConfigP2P __cfgP2P;
    __cfgP2P.update();
    int iPjSvrPort = atoi(__cfgP2P.getConfig().strPort.c_str());

    //char *method = (char *)"main";
    // 同全局变量重名
    pj_turn_srv *ts_pj_srv = NULL;
    int result = 0;
    //pj_status_t status;
    char b_en[16] = { 0 };

    result = ts_pj_server_create(iPjSvrPort, &ts_pj_srv);
    if(NULL == ts_pj_srv)
    {
        //PJ_LOG(4,(__FILE__, "%s:create pj_turn_server failed", method));
        LOG4CPLUS_ERROR(LOG_SOLAR, "ts_pj_server_create:"<<result);
        return;
    }

    //create user database manager
    result = ts_udb_create(ts_pj_srv);
    if (result != TS_SUCCESS)
    {
        LOG4CPLUS_ERROR(LOG_SOLAR, "ts_udb_create:"<<result);
        return;
    }


    strcpy((char *) b_en, AES_PWD);
    result = ipc_handler_init(b_en, sizeof(b_en), g_GeneralSocketProcessor.GetIPC_hander());
    if(result != TS_SUCCESS)
    {
        LOG4CPLUS_ERROR(LOG_SOLAR, "ipc_handler_init:"<<result);
        return;
    }

    void *arg = (void *)ts_pj_srv;
    result = ts_pj_server_start(&arg);
    // 启动数据发送线程
    if(result == TS_SUCCESS && NULL != ts_pj_srv)
    {
        pj_runnable = TS_TRUE;
        //start a new pjsip thread to work
        pj_thread_create(ts_pj_srv->core.pool, ts_pj_srv->obj_name, &sndto_client,
                         ts_pj_srv, 0, 0, &pt);
        //__fline;
        //printf("pj_thread_create\n");
    }
    g_GeneralSocketProcessor.SetPJ_turn_srv(ts_pj_srv);

    LOG4CPLUS_INFO(LOG_SOLAR, "StartPJTurnServer Finished");

    return;
}

int CSolar::SetPlatform()
{
	system("ulimit -c unlimited");

	return 0;
}

int CSolar::InitializeLogs()
{
    //	初始化日志系统
    CConfigGeneral __cfgGeneral;
    __cfgGeneral.update();

    log4cplus::tstring pattern = LOG4CPLUS_TEXT("%d %-5p %-5c - %m [%l]%n"); // c , model
    SharedAppenderPtr append_1;
    if (__cfgGeneral.getConfig().strLogFileName.size()<=0)
    {
        append_1=new ConsoleAppender();
    }
    else
    {
        std::string strFilePathName = __cfgGeneral.getConfig().strLogFilePath +"/"+  __cfgGeneral.getConfig().strLogFileName;
        printf("Initialize Logs:%s, MaxSize:%d Files:%d - "
			, strFilePathName.c_str()
			, __cfgGeneral.getConfig().maxFileSize
			, __cfgGeneral.getConfig().maxBackupIndex);
        int iret = ez_creat_dir(__cfgGeneral.getConfig().strLogFilePath.c_str());
        if (iret<0)
            printf("failed\n");
        else
            printf("success\n");

        append_1 = new RollingFileAppender(LOG4CPLUS_TEXT(strFilePathName)
                                   , __cfgGeneral.getConfig().maxFileSize
                                   , __cfgGeneral.getConfig().maxBackupIndex);
    }
    append_1->setName(LOG4CPLUS_TEXT("TsHome"));
    append_1->setLayout( std::auto_ptr<Layout>(new PatternLayout(pattern)) );
    Logger::getRoot().addAppender(append_1);
    NDCContextCreator _context(LOG4CPLUS_TEXT("tss"));

    return 0;
}

void CSolar::Done()
{
    while (1)
    {
        CEZSemaphore sem;
        sem.Pend();
    }
}

void CSolar::Term()
{
    LOG4CPLUS_INFO(LOG_SOLAR, "CSolar::Term");

    return;
}

/* set advisory lock on file */
int lockfile(int fd)
{
    struct flock fl;

    fl.l_type = F_WRLCK;  /* write lock */
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;  //lock the whole file

    return(fcntl(fd, F_SETLK, &fl));
}

int already_running(const char *filename)
{
    int fd;
    char buf[16];

    fd = open(filename, O_RDWR | O_CREAT, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
    if (fd < 0)
    {
        printf("can't open %s: %m\n", filename);
        exit(-1);
    }

    /* 先获取文件锁 */
    if (lockfile(fd) == -1)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            //printf("file: %s already locked\n", filename);
            close(fd);
            return -1;
        }
        printf("can't lock %s: %m\n", filename);
        exit(-1);
    }
    /* 写入运行实例的pid */
    ftruncate(fd, 0);
    sprintf(buf, "%ld", (long)getpid());
    write(fd, buf, strlen(buf) + 1);

    return 0;
}

void CSolar::TimerProcDBLive()
{
	//__fline;
	//printf("CSolar::TimerProcDBLive()  ");
	const char *sql = "select version()";
    int res = 0;
	MYSQL_RES *res_ptr = NULL;
    //MYSQL_ROW sqlrow;
    //get a mysql connection from pool.
    MysqlConn conn = pool_getConn();
	MYSQL *mysql = conn.mysql;
    if (NULL == mysql)
    {
    	__fline;
		printf("in TimerProcDBLive conn is null\n");
        LOG4CPLUS_ERROR(LOG_DB, "pool_getConn");
        //return NULL;
    }else{
		//printf("mysql=%p\n", mysql);
	    res = mysql_query(mysql, sql);
		if(!res){
			res_ptr = mysql_use_result(mysql);
			/*while((sqlrow = mysql_fetch_row(res_ptr))){
				dataPointId= atoi(sqlrow[0]);
				cout << "dataPointId=" << dataPointId<<"\n";
			}*/
			mysql_free_result(res_ptr);
		}
    }
	//recycle mysql connection
    pool_recycle(conn);
}

void CSolar::TimerProcMaintain()
{
    // 为了计数准确，不依赖系统时间，自己维护
    m_iRunPeriod ++;
}

const std::string & CSolar::GetVersion()
{
    return m_strVer;
}

unsigned int CSolar::GetRunPeriod()
{
    return m_iRunPeriod;
}

time_t CSolar::GetStartTime()
{
    return m_ttStartTime;
}

