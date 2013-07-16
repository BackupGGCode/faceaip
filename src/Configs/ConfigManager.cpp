/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigManager.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigManager.cpp 5884 2012-07-02 09:17:36Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-07-02 09:17:36  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "ConfigManager.h"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int CConfigManager::SetDefaultConfig(int iConfigType)
{
    int iRet = 0;
    if ((iConfigType > CFG_IDX_ALL) || (iConfigType < CFG_IDX_General))
    {
        tracepoint();
        return CONFIG_APPLY_FILE_ERROR;
    }

    switch(iConfigType)
    {
        case CFG_IDX_General:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_General);

            break;
        }
        case CFG_IDX_DataBase:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_DataBase);

            break;
        }
        case CFG_IDX_TcpSocketServer:
        {
            //TCP������
            iRet |= recallConfig(CFG_NAME_TcpSocketServer);

            break;
        }
        case CFG_IDX_TcpSocketConnector:
        {
            //TCP�ͻ�������
            iRet |= recallConfig(CFG_NAME_TcpSocketConnector);

            break;
        }
        case CFG_IDX_ConsoleOverTcp:
        {
            //�����ն�
            iRet |= recallConfig(CFG_NAME_ConsoleOverTcp);

            break;
        }
        case CFG_IDX_WebService:
        {
            iRet |= recallConfig(CFG_NAME_WebService);

            break;
        }
        case CFG_IDX_PubService:
        {
            iRet |= recallConfig(CFG_NAME_PubService);

            break;
        }
        case CFG_IDX_SmsOverHttp:
        {
            iRet |= recallConfig(CFG_NAME_SmsOverHttp);

            break;
        }
        case CFG_IDX_ALL:
        {
            //�ָ�ȫ��,�����
            trace("Recall all \n");
            iRet |= recallConfigAll();
            break;
        }
        default:
        {
            trace("###########default \n");
            break;
        }
    }

    return iRet;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

PATTERN_SINGLETON_IMPLEMENT(CConfigManager);

int CConfigManager::initialize(std::string mfile, std::string sfile)
{
    CConfigMan::initialize(mfile, sfile);

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // ���ͨ������
    setupConfig(CFG_NAME_General, CFG_IDX_General, m_configGeneral);

    setupConfig(CFG_NAME_TcpSocketServer, CFG_IDX_TcpSocketServer, m_configTcpSocketServer);

    setupConfig(CFG_NAME_TcpSocketConnector, CFG_IDX_TcpSocketConnector, m_configTcpSocketConnector);
    setupConfig(CFG_NAME_ConsoleOverTcp, CFG_IDX_ConsoleOverTcp, m_configConsoleOverTcp);

    setupConfig(CFG_NAME_DataBase, CFG_IDX_DataBase, m_configDB);

    setupConfig(CFG_NAME_WebService, CFG_IDX_WebService, m_configWebService);
    setupConfig(CFG_NAME_PubService, CFG_IDX_PubService, m_configPubService);
    setupConfig(CFG_NAME_SmsOverHttp, CFG_IDX_SmsOverHttp, m_configSmsOverHttp);
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // ��ʼ�����, ����һ������
    CConfigMan::saveFile();

    return 0;
}

