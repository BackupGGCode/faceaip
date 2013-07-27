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

#include "ConfigNet.h"
#include "ConfigWebService.h"
#include "ConfigNetworkInterface.h"
#include "ConfigSmartWebs.h"
#include "ConfigNTPClient.h"

typedef enum __cfg_index_t {
    CFG_IDX_BEGIN =  0,	// ��ʼ�� ʵ��û���������ã�����ͳһ����
    CFG_IDX_General,	// ��ͨ
    CFG_IDX_TcpSocketServer,				// TcpSocketServer ����
    CFG_IDX_TcpSocketConnector,				// TcpSocketConnector ����
    CFG_IDX_ConsoleOverTcp,				// ConsoleOverTcp ����
    CFG_IDX_DataBase,				// ���ݿ�����
    CFG_IDX_Net,				//
    CFG_IDX_WebService,				//
    CFG_IDX_NetworkInterface,				//����ӿ�����
    CFG_IDX_SmartWebs,				//����ӿ�����
    CFG_IDX_NTPClient,				//����ӿ�����
    CFG_IDX_ALL,			// ʵ��û���������ã� ���ڻָ�ȫ������
}CFG_INDEX;

#define CFG_NAME_General "General"
#define CFG_NAME_TcpSocketServer "TcpSocketServer"
#define CFG_NAME_TcpSocketConnector "TcpSocketConnector"
#define CFG_NAME_ConsoleOverTcp "ConsoleOverTcp"
#define CFG_NAME_DataBase "DataBase"
#define CFG_NAME_Net "Net"
#define CFG_NAME_WebService "WebService"
#define CFG_NAME_NetworkInterface "NetworkInterface"
#define CFG_NAME_SmartWebs "SmartWebs"
#define CFG_NAME_NTPClient "NTPClient"

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
    CConfigNet			m_configNet;	/*!< Net */
    CConfigWebService			m_configWebService;	/*!< WebService */
    CConfigNetworkInterface			m_ConfigNetworkInterface;	/*!< IF */
    CConfigSmartWebs			m_ConfigSmartWebs;	/*!< IF */
    CConfigNTPClient			m_ConfigNTPClient;	/*!< ntp �ͻ��� */

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CConfigManager(){};
    ~CConfigManager(){};
};

#define g_Config (*CConfigManager::instance())

#endif //__CONFIG_MANAGER_H__
