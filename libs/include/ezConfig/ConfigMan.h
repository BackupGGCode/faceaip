/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigMan.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigMan.h 5884 2012-07-02 09:15:02Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __CONFIG_MAN_H__
#define __CONFIG_MAN_H__

// ��zipѹ����ʽ���
// ����ע�ͣ� �Ա��ֹ��޸ģ� 2012-6-21 16:36:36 wujunjie
// #define HAVE_ZLIB_ZFILE

#include <string.h>
#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"
#include <json.h>
#include "ConfigBase.h"

//#ifdef HAVE_ZLIB_ZFILE
#include "zlib.h"
//#else
#include "File.h"
//#endif //HAVE_ZLIB_ZFILE

class CConfigMan;
class CSaveDelay : public CEZThread
{
public:
    CSaveDelay();
    ~CSaveDelay();
    int Start(CConfigMan * pCConfigMan);
    int Stop();
    void ThreadProc();
private:
    CConfigMan *m_pCConfigMan;
};
class CConfigMan : public CEZObject
{
public:
    CConfigMan();
    virtual ~CConfigMan();
    //! ��ʼ��
    // ��������ʹ��Ĭ�����֣���Ϊ���ļ���ʽ
    // ��һ������ʹ�ø����֣�Ϊ���ļ���ʽ
    // ����������ʹ�ø����֣�Ϊ˫�ļ���ʽ
    virtual int initialize(std::string mfile="", std::string sfile="");
    //! �����ļ�
    void saveFile();
    int recallConfig(const char* name, const char* user = NULL, int index = -1);
    int recallConfigAll(const char* user = NULL);

protected:
    typedef std::map<std::string, CConfigExchange*> CONFIG_MAP;
    typedef std::map<long, std::string> CONFIG_MAPR; // ID, Name, id use obj addr
    typedef std::map<std::string, long> MAP_TRANSLATE; // insert(valueType(CFG_NAME_General, CFG_IDX_General));
    //typedef std::map<std::string, int>::value_type valueType;// MAP_TRANSLATE val

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
    // �Ƿ�˫�ļ� 1�ǣ�Ĭ�ϲ���
    int m_iDoubleCfgFile;

    // װ������
    void setupConfig(const char* name, long cfgIDX, CConfigExchange& xchg);
    void onConfigChanged(CConfigExchange& xchg, int& ret);
    //CConfigTable& locate(CConfigTable& table, const char* name);
    const char* nameFromID(int id);
    CConfigExchange* xchgFromName(const char* name);
    int readConfig(const char* chPath, std::string& input);
    void onTimer(PARAM arg);
};

#endif //__CONFIG_MAN_H__
