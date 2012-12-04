/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigManager.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
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
    if ((iConfigType > CFG_IDX_ALL) || (iConfigType < CFG_IDX_GENERAL))
    {
        tracepoint();
        return CONFIG_APPLY_FILE_ERROR;
    }

    switch(iConfigType)
    {
        case CFG_IDX_GENERAL:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_GENERAL);

            break;
        }
        case CFG_IDX_IPC:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_IPC);

            break;
        }
        case CFG_IDX_P2P:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_P2P);

            break;
        }
        case CFG_IDX_DB:
        {
            //��ͨ����
            iRet |= recallConfig(CFG_NAME_DB);

            break;
        }
#ifdef SMARTHOME_AGENT_JSTELECOM
        case CFG_IDX_SMARTHOMETELCOM:
        {
            //���յ����ǻ�e�� ����
            iRet |= recallConfig(CFG_NAME_SMARTHOME_JSTELECOM);

            break;
        }
#endif
        case CFG_IDX_ALL:
        {
            //�ָ�ȫ��,�����
            trace("Recall all \n");
            iRet |= recallConfigAll();
            //����˵����ûָ�Ĭ��
            iRet |= setDefaultNetConfig();
            break;
        }
        default:
        trace("###########default \n");
        break;
    }

    return iRet;
}
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//�Ƿ��� g log
//#define HAVE_LOG_SAVER

//#define MAX_PATH 256

#define configFileFirst "./config_1.json"
#define configFileSecond "./config_2.json"
// ������󳤶���ʱ�̶����Ժ��ٶ�̬���ڡ�������ó��ȳ���configStreamSize��д���û������
const int configStreamSize = (512 * 1024);

PATTERN_SINGLETON_IMPLEMENT(CConfigManager);

CConfigManager::CConfigManager()
{
    m_strConfigFileFirst = "";
    m_strconfigFileSecond = "";
    m_iDoubleCfgFile = 0;
}
CConfigManager::~CConfigManager()
{
    ;
}
// ��Ҫ���ã�
// ʹ��˫�����ļ����ƣ� ĳ���𻵻�Ϊ����
void CConfigManager::useDoubleCfgFile(int iDoubleCfgFile)
{
    m_iDoubleCfgFile = iDoubleCfgFile;
}

void CConfigManager::initialize(std::string mfile, std::string sfile)
{
    m_strConfigFileFirst = mfile.size()==0?configFileFirst:mfile;
    m_strconfigFileSecond = sfile.size()==0?configFileSecond:sfile;

    m_changed = FALSE;

    CConfigReader reader;

    m_stream.reserve(configStreamSize);

    int bToRead2ndFile = TRUE;

    if(readConfig(m_strConfigFileFirst.c_str(), m_stream))
    {
        // ���Լ��ļ�������У��
        bToRead2ndFile = !reader.parse(m_stream, m_configAll);
    }
    else
    {
        trace("readConfig file1 failed\n");
    }
    //#ifdef USE_DOUBLE_CFG_FILE
    if (1 == m_iDoubleCfgFile)
    {
        if (bToRead2ndFile)
        {
            trace("CConfigManager::initialize() first config file parsing failed.\n");

            if(readConfig(m_strconfigFileSecond.c_str(), m_stream))
            {
                // ���Լ��ļ�������У��

                bToRead2ndFile = !reader.parse(m_stream, m_configAll);
            }
            else
            {
                trace("readConfig file2 failed\n");
            }
        }

        if (bToRead2ndFile)
        {
            trace("CConfigManager::initialize() second config file parsing failed too.\n");
        }
    }// if (1 == m_iDoubleCfgFile)
    //#endif //USE_DOUBLE_CFG_FILE

    //��ʼ���������ƺ���������ӳ���
    m_mapTranslate.clear();

 /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
   // ���ͨ������
    m_mapTranslate.insert(valueType(CFG_NAME_GENERAL, CFG_IDX_GENERAL));
    setupConfig(CFG_NAME_GENERAL, m_configGeneral);

    m_mapTranslate.insert(valueType(CFG_NAME_IPC, CFG_IDX_IPC));
    setupConfig(CFG_NAME_IPC, m_configIPC);
    
    m_mapTranslate.insert(valueType(CFG_NAME_P2P, CFG_IDX_P2P));
    setupConfig(CFG_NAME_P2P, m_configP2P);
    
    m_mapTranslate.insert(valueType(CFG_NAME_DB, CFG_IDX_DB));
    setupConfig(CFG_NAME_DB, m_configDB);
#ifdef SMARTHOME_AGENT_JSTELECOM
    m_mapTranslate.insert(valueType(CFG_NAME_SMARTHOME_JSTELECOM, CFG_IDX_SMARTHOMETELCOM));
    setupConfig(CFG_NAME_SMARTHOME_JSTELECOM, m_configSmartHomeTelcom);
#endif
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
}

// װ������
void CConfigManager::setupConfig(const char* name, CConfigExchange& xchg)
{
    m_map.insert(CONFIG_MAP::value_type(name, &xchg));
    m_mapReverse.insert(CONFIG_MAPR::value_type(xchg.getID(), name));

    xchg.setTable(m_configAll[name]);
    if(xchg.getUsed() == 0 || !xchg.validate())
    {
        xchg.setUsed(xchg.getNumber()); // ��ʹ��������ʼ��Ϊ���鳤��
        trace("CConfigManager::setupConfig, recall config : '%s'\n", name);
        xchg.recall();
    }
    xchg.getTable(m_configAll[name]); //����У��������
    m_changed = TRUE;
    xchg.commit(NULL, -1, CONFIG_APPLY_NOT_SAVE); // ����������ľ�̬��Ա-���ýṹ������
    xchg.attach(this, (TCONFIG_PROC)&CConfigManager::onConfigChanged); //���ע�ᣬ������ʼ��ʱ������ڵ�д�����ļ�
}

//������������ȡ�������ñ��ܹ����ֵ�ַ��ţ��漰�����õķּ����ԣ���δʵ��
/*CConfigTable& CConfigManager::locate(CConfigTable& table, const char* name)
{
}*/

// �����ļ�
void CConfigManager::saveFile()
{
    static CEZMutex fileMutex;
    CEZGuard l_CEZGuard(fileMutex);
    CConfigWriter writer(m_stream);

    if(!m_changed)
    {
        return;
    }
    m_changed = FALSE;

    m_stream = "";
    writer.write(m_configAll);

    if (1 == m_iDoubleCfgFile)
    {
        remove(m_strconfigFileSecond.c_str());
        rename(m_strConfigFileFirst.c_str(), m_strconfigFileSecond.c_str());
    }

#ifdef HAVE_ZLIB_ZFILE

    m_fileConfig = gzopen(m_strConfigFileFirst.c_str(), "wb");
#else

    m_fileConfig.Open(m_strConfigFileFirst.c_str(),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate);
    //__fline;
    //printf("m_fileConfig.Open:%s\n", m_strConfigFileFirst.c_str());
#endif

#ifdef HAVE_ZLIB_ZFILE

    if((int)m_stream.size() != gzwrite(m_fileConfig, (char*)m_stream.c_str(), m_stream.size()))
#else

    unsigned long  ntmp;
    if((unsigned long)m_stream.size() != (ntmp = m_fileConfig.Write((char*)m_stream.c_str(), m_stream.size())))
#endif

    {
        trace("write config file failed!\n");
    }

    //__fline;
    //printf("m_fileConfig.Write(%d):%s\n", m_stream.size(), (char*)m_stream.c_str());
#ifdef HAVE_ZLIB_ZFILE
    gzflush(m_fileConfig, Z_FINISH);
    gzclose(m_fileConfig);
#else

    m_fileConfig.Flush();
    m_fileConfig.Close();
#endif
}

// ���ñ仯ʱ��ͳһ����
void CConfigManager::onConfigChanged(CConfigExchange& xchg, int& ret)
{
    const char *name = nameFromID(xchg.getID());
    MAP_TRANSLATE::iterator it;

    if(!name)
        return;

    //����ȫ�ֱ��
    xchg.getTable(m_configAll[name]);
    m_changed = TRUE;

    //�����ļ�,���ȴ����ӳٱ���
    if (ret & CONFIG_APPLY_DELAY_SAVE)
    {
        m_Timer.Start(this, (TIMERPROC)&CConfigManager::onTimer,1000,0);
    }
    else
    {
        if (!(ret & CONFIG_APPLY_NOT_SAVE))
        {
            saveFile();
        }
    }

    it = m_mapTranslate.find(name);
    if (it != m_mapTranslate.end())
    {
        //��¼��־
#ifdef HAVE_LOG_SAVER
        g_Log.Append(LOG_CONFSAVE, (*it).second);
#endif

    }
    //��¼��־��ע�Ᵽ������������Ƶ�ָ�룬��Ҫת��
    //g_Log.Append(LOG_CONFSAVE, 0, (void*)name, 16/*, xchg->getUser()*/);
}

// ������ID�õ���������
const char* CConfigManager::nameFromID(int id)
{
    CONFIG_MAPR::iterator pi;
    pi = m_mapReverse.find(id);
    if(pi != m_mapReverse.end())
    {
        return (*pi).second.c_str();
    }
    else
    {
        trace("CConfigManager::nameFromID(%d) failed.\n", id);
        return NULL;
    }
}

// ���������Ƶõ�������ָ��
CConfigExchange* CConfigManager::xchgFromName(const char* name)
{
    CONFIG_MAP::iterator pi;
    pi = m_map.find(name);
    if(pi != m_map.end())
    {
        return (*pi).second;
    }
    else
    {
        trace("CConfigManager::xchgFromName('%s') failed.\n", name);
        return NULL;
    }
}

int CConfigManager::readConfig(const char* chPath, std::string& input)
{
#ifdef HAVE_ZLIB_ZFILE
    m_fileConfig = gzopen(chPath, "rb");
    if(!m_fileConfig)
        return FALSE;
#else

    m_fileConfig.Open(chPath, CFile::modeRead);
    //__fline;
    //printf("m_fileConfig.Open:%s\n", chPath);
#endif

    const int size = 32*1024;
    char* buf = new char[size + 1];

    input = "";

    while (1)
    {
#ifdef HAVE_ZLIB_ZFILE
        int nLen = 	gzread(m_fileConfig, buf, size);
#else

        int nLen = 	m_fileConfig.Read(buf, size);
#endif

        if(nLen <=0 )
            break;
        buf[nLen] = 0;
        input += buf;
    }
    input += '\0';
#ifdef HAVE_ZLIB_ZFILE

    gzclose(m_fileConfig);
#else

    m_fileConfig.Close();
#endif
    //input = buf;
    delete []buf;

    return TRUE;
}

int CConfigManager::recallConfig(const char* name, const char* user /* = NULL */, int index /*= -1*/)
{
    CConfigExchange* xchg = xchgFromName(name);

    if(!xchg)
    {
        return CONFIG_APPLY_NOT_EXSIST;
    }

    // ����һ�����ã���Ҫ�Ǵ���һЩ����Ҫ���µ�����
    xchg->update(index);
    xchg->recall(index);
    return xchg->commit(user, index, CONFIG_APPLY_NOT_SAVE);
}

int CConfigManager::recallConfigAll(const char* user /* = NULL */)
{
    int iRet = 0;
    CONFIG_MAP::iterator pi;

    for(pi = m_map.begin(); pi != m_map.end(); pi++)
    {
        (*pi).second->recall();
        iRet |= (*pi).second->commit(user, -1, CONFIG_APPLY_NOT_SAVE);
    }

    if(iRet)
    {
        saveFile();
    }

    return iRet;
}
void CConfigManager::onTimer(PARAM arg)
{
    //saveFile();
    m_saveDelay.Start();
}
int CConfigManager::setDefaultNetConfig()
{
    int iRet = 0;

    return iRet;
}

CSaveDelay::CSaveDelay() : CEZThread("SaveDelay", TP_DEFAULT)
{}

CSaveDelay::~CSaveDelay()
{}

int CSaveDelay::Start()
{
    return CreateThread();
}

int CSaveDelay::Stop()
{
    return DestroyThread();
}

void CSaveDelay::ThreadProc()
{
    g_Config.saveFile();
    Stop();
}
