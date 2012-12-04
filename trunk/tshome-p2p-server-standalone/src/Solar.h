/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Solar.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: Solar.h 5884 2012-05-16 09:17:46Z WuJunjie $
 *
 *  Explain:
 *     -
 *      ���ܼҾӷ�����
 *     -
 *
 *  Update:
 *     2012-05-16 09:17:46   WuJunjie  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __Solar_H__
#define __Solar_H__

#include "CommonInclude.h"

#include "EZThread.h"
#include "EZTimer.h"
#include "LifeDetectionTimer.h"

class CSolar : public CEZObject
{
public:
    PATTERN_SINGLETON_DECLARE(CSolar);

    CSolar();
    ~CSolar();

    void Initialize(int argc, char * argv[]);

    void Done();
    // kill(1)
    void Term();
    void haveException(int ExcNo=0, char *pMsg = NULL);
    void Cleanup(int iMode=0);
    void TimerProcDBLive();
    void TimerProcMaintain();

    const std::string &GetVersion();
    unsigned int GetRunPeriod();
    time_t GetStartTime();
private:
    //����ԭ������
    void StartPJTurnServer();
    int SetPlatform();
    int InitializeLogs();

    void ShowVersion();

    CEZTimer          m_TimerDBAlive;          //���ݿⱣ��
    CEZTimer          m_TimerMaintain;          //����ܶ�ʱ��
	LifeDetectionTimer lifeTimer;				//������ʱ�����

    //CEZMutex          m_Mutex;
    //BOOL m_bAlive;

    // �汾�ַ���
    std::string m_strVer;
    // ����ʱ��
    unsigned int m_iRunPeriod;

    time_t m_ttStartTime;
};

#define g_Solar (*CSolar::instance())

#endif //__Solar_H__
