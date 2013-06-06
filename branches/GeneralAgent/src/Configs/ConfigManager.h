/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigManager.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
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

// ��zipѹ����ʽ���
// ����ע�ͣ� �Ա��ֹ��޸ģ� 2012-6-21 16:36:36 wujunjie 
// #define HAVE_ZLIB_ZFILE

#include <string.h>
#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"
#include <json.h>

#ifdef HAVE_ZLIB_ZFILE
#include "zlib.h"
#else
#include "File.h"
#endif //HAVE_ZLIB_ZFILE

class CSaveDelay : public CEZThread
{
public:
    CSaveDelay();
    ~CSaveDelay();
    int Start();
    int Stop();
    void ThreadProc();

};

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "ConfigGeneral.h"
#include "ConfigTcpSocketServer.h"
#include "ConfigDB.h"
#include "ConfigWebService.h"

typedef enum __cfg_index_t {
    CFG_IDX_GENERAL =  0,	// ��ͨ
    CFG_IDX_TcpSocketServer,				// ���ݿ�����
    CFG_IDX_DB,				// ���ݿ�����
    CFG_IDX_MY_WEBSERVICE,				// 
    CFG_IDX_ALL,			// ʵ��û���������ã� ���ڻָ�ȫ������
}CFG_INDEX;

#define CFG_NAME_GENERAL "General"
#define CFG_NAME_TcpSocketServer "TcpSocketServer"
#define CFG_NAME_DB "DB"
#define CFG_NAME_MY_WEBSERVICE "MY_WEBSERVICE"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

class CConfigManager : public CEZObject
{
private:
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    //����������
    CConfigGeneral		m_configGeneral;	/*!< ��ͨ���� */
    CConfigTcpSocketServer			m_configTcpSocketServer;	/*!< dsd */
    CConfigDB			m_configDB;	/*!< dsd */
    CConfigWebService			m_configWebService;	/*!< WebService */

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CConfigManager();
    ~CConfigManager();

    typedef std::map<std::string, CConfigExchange*> CONFIG_MAP;
    typedef std::map<int, std::string> CONFIG_MAPR;
    typedef std::map<std::string, int> MAP_TRANSLATE;
    typedef std::map<std::string, int>::value_type valueType;

public:
    PATTERN_SINGLETON_DECLARE(CConfigManager);

    //! ��������
    // iDoubleCfgFile 0 �ر�˫���ù��ܣ� Ĭ�Ϲرգ� 1 ����˫���ù���
    void useDoubleCfgFile(int iDoubleCfgFile = 1);
    //! ��ʼ��
    void initialize(std::string mfile="", std::string sfile="");
    int recallConfig(const char* name, const char* user = NULL, int index = -1);
    int recallConfigAll(const char* user = NULL);
    //! �����ļ�
    void saveFile();
    //! ����Ĭ�����ã���GUI���������ģ�����
    int SetDefaultConfig(int iConfigType);

private:
    void setupConfig(const char* name, CConfigExchange& xchg);
    void onConfigChanged(CConfigExchange& xchg, int& ret);
    //CConfigTable& locate(CConfigTable& table, const char* name);
    const char* nameFromID(int id);
    CConfigExchange* xchgFromName(const char* name);
    int readConfig(const char* chPath, std::string& input);
    void onTimer(PARAM arg);
    //!�ָ�����˲��Ǳ����������ļ��е�����
    int setDefaultNetConfig();

private:
    CConfigTable m_configAll;				/*!< �����ܱ� */
    int m_changed;							/*!< ���ñ�仯�� */
#ifdef HAVE_ZLIB_ZFILE

    gzFile m_fileConfig;					/*!< �����ļ� */
#else

    CFile m_fileConfig;
#endif

    CONFIG_MAP m_map;						/*!< �������ƺ�������ָ��ӳ��� */
    CONFIG_MAPR m_mapReverse;				/*!< ����ID���������ƺ�ӳ��� */

    MAP_TRANSLATE m_mapTranslate;			/*!< �������ƺ���������ӳ���*/

    CEZTimer m_Timer;
    std::string m_stream;	// �ַ�����

    CSaveDelay				m_saveDelay;

    std::string m_strConfigFileFirst;
    std::string m_strconfigFileSecond;
    int m_iDoubleCfgFile;
};

#define g_Config (*CConfigManager::instance())

#endif //__CONFIG_MANAGER_H__
