/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigManagerNetInit.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigManagerNetInit.h 5884 2012-07-02 09:15:02Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __ConfigManagerNetInit_H__
#define __ConfigManagerNetInit_H__

#include "ConfigMan.h"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "../Configs/ConfigNet.h"
//
//#include "ConfigTcpSocketServer.h"
//#include "ConfigTcpSocketConnector.h"
//#include "ConfigConsoleOverTcp.h"
//
//#include "ConfigDatabase.h"
//#include "ConfigWebService.h"

#include "../Configs/ConfigNetworkInterface.h"
//#include "ConfigSmartWebs.h"

typedef enum __cfg_index_t {
    CFG_IDX_BEGIN =  0,	// ��ʼ�� ʵ��û���������ã�����ͳһ����
    CFG_IDX_Net,	
//    CFG_IDX_TcpSocketServer,				// TcpSocketServer ����
//    CFG_IDX_TcpSocketConnector,				// TcpSocketConnector ����
//    CFG_IDX_ConsoleOverTcp,				// ConsoleOverTcp ����
//    CFG_IDX_DataBase,				// ���ݿ�����
//    CFG_IDX_WebService,				//
    CFG_IDX_NetworkInterface,				//����ӿ�����
//    CFG_IDX_SmartWebs,				//����ӿ�����
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

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

class CConfigManagerNetInit : public CConfigMan
{
public:
    PATTERN_SINGLETON_DECLARE(CConfigManagerNetInit);

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
    CConfigNet		m_configNet;	/*!< ��ͨ���� */
//
//    CConfigTcpSocketServer			m_configTcpSocketServer;	/*!< dsd */
//    CConfigTcpSocketConnector			m_configTcpSocketConnector;	/*!< dsd */
//    CConfigConsoleOverTcp			m_configConsoleOverTcp;	/*!< dsd */
//
//    CConfigDatabase			m_configDB;	/*!< dsd */
//    CConfigWebService			m_configWebService;	/*!< WebService */
    CConfigNetworkInterface			m_ConfigNetworkInterface;	/*!< IF */
//    CConfigSmartWebs			m_ConfigSmartWebs;	/*!< IF */

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CConfigManagerNetInit(){};
    ~CConfigManagerNetInit(){};
};

#define g_Config (*CConfigManagerNetInit::instance())

#endif //__ConfigManagerNetInit_H__
