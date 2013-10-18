/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigManager.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigManager.h 5884 2012-07-02 09:15:02Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __CONFIG_MANAGER_H__
#define __CONFIG_MANAGER_H__

#include "ConfigMan.h"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "ConfigGeneral.h"

#include "ConfigTcpSocketServer.h"
#include "ConfigTcpSocketConnector.h"
#include "ConfigConsoleOverTcp.h"

#include "ConfigDatabase.h"
#include "ConfigWebService.h"

typedef enum __cfg_index_t {
    CFG_IDX_General =  0,	// ��ͨ
    CFG_IDX_TcpSocketServer,				// TcpSocketServer ����
    CFG_IDX_TcpSocketConnector,				// TcpSocketConnector ����
    CFG_IDX_ConsoleOverTcp,				// ConsoleOverTcp ����
    CFG_IDX_DataBase,				// ���ݿ�����
    CFG_IDX_WebService,				//
    CFG_IDX_ALL,			// ʵ��û���������ã� ���ڻָ�ȫ������
}CFG_INDEX;

#define CFG_NAME_General "General"
#define CFG_NAME_TcpSocketServer "TcpSocketServer"
#define CFG_NAME_TcpSocketConnector "TcpSocketConnector"
#define CFG_NAME_ConsoleOverTcp "ConsoleOverTcp"
#define CFG_NAME_DataBase "DataBase"
#define CFG_NAME_WebService "WebService"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

class CConfigManager : public CConfigMan
{
public:
    PATTERN_SINGLETON_DECLARE(CConfigManager);

    //! ��ʼ��
    // ��������ʹ��Ĭ�����֣���Ϊ���ļ���ʽ
    // ��һ������ʹ�ø����֣�Ϊ���ļ���ʽ
    // ����������ʹ�ø����֣�Ϊ˫�ļ���ʽ
    int initialize(std::string mfile="", std::string sfile="");

    //! ����Ĭ�����ã���GUI���������ģ�����
    int SetDefaultConfig(int iConfigType);

private:
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    //����������
    CConfigGeneral		m_configGeneral;	/*!< ��ͨ���� */

    CConfigTcpSocketServer			m_configTcpSocketServer;	/*!< dsd */
    CConfigTcpSocketConnector			m_configTcpSocketConnector;	/*!< dsd */
    CConfigConsoleOverTcp			m_configConsoleOverTcp;	/*!< dsd */

    CConfigDatabase			m_configDB;	/*!< dsd */
    CConfigWebService			m_configWebService;	/*!< WebService */

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CConfigManager(){};
    ~CConfigManager(){};
};

#define g_Config (*CConfigManager::instance())

#endif //__CONFIG_MANAGER_H__
