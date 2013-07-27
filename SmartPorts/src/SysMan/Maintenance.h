/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Maintenance.h - brief
 *
 * Copyright (C) 2013 ezlibs.com, All Rights Reserved.
 *
 * $Id: Maintenance.h 5884 2013-06-25 05:11:31Z WuJunjie $
 *
 *  Notes:
 *     -
 *      1. ϵͳ��ʼ�����趨
 *      2. ϵͳ����ά��
 *     -
 *
 *  Update:
 *     2013-06-25 05:11:31  WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __Maintenance_H__
#define __Maintenance_H__

#include "EZThread.h"
#include "EZTimer.h"
//#include "ez_socket.h"
#include "net_work_interface.h"

#include "../Configs/ConfigGeneral.h"
#include "../Configs/ConfigNet.h"
#include "../Configs/ConfigNetworkInterface.h"
#include "../Configs/ConfigNTPClient.h"

class CMaintenance : public CEZObject
{
public:
    PATTERN_SINGLETON_DECLARE(CMaintenance);

    CMaintenance();
    ~CMaintenance();

    void Start(const bool bUseCfgAtStartForce=false, const bool bMaintenanceAuto = false);

    void Stop();


#ifdef FUNCTION_DATETIME_AND_NTP

    time_t GetLastUpdateFailed();
    time_t GetLastUpdate();

    unsigned int GetRunTimerProcSendNTPClient_Succ();
    unsigned int GetRunTimerProcSendNTPClient_Failed();
#endif //FUNCTION_DATETIME_AND_NTP

    void MarkOnInternet();
    void MarkOnIntranet();

    bool IsOnInternet();
    const char * GetNatIP();

    bool IsOnIntranet();

    void TimerProcSendGratuitousArp();

private:
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // ����
    int MaintenanceAtStart(bool bForce=true);

    int EthMaintenance(CONFIG_NETWORKINTERFACE_T *pCfgIF);
    int NetMaintenance();

#ifndef STANDLONE_UTIL

    CONFIG_NET m_LastConfigNet; // OLD DNS

    void OnChangedConfigNet(CConfigNet* pConfigNet, int &ret);
    void OnChangedConfigNetworkInterface(CConfigNetworkInterface* pConfig, int &ret);
    CConfigNet	m_CCfgNet;
    CConfigNetworkInterface	m_CCfgNetworkInterface;
#endif //STANDLONE_UTIL
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    bool m_bMaintenanceAuto;


    CEZTimer          m_TimerSendGratuitousArp;          //����ܶ�ʱ��

#ifdef FUNCTION_DATETIME_AND_NTP

    CConfigNTPClient m_CCfgNTPClient;
    void OnChangedConfigNTPClient(CConfigNTPClient* pConfigNTPClient, int &ret);
    CEZTimer          m_TimerNTPClient;          //����ܶ�ʱ��
    void TimerProcSendNTPClient();
    ConfigNTPClient m_LastNTPClient;
    time_t m_ttLastUpdateFailed;// ����ʱ�� ���߸���ʱ��
    time_t m_ttLastUpdate;// ����ʱ�� ���߸���ʱ��

    unsigned int iRunTimerProcSendNTPClient_Succ;
    unsigned int iRunTimerProcSendNTPClient_Failed;

#endif //FUNCTION_DATETIME_AND_NTP

};

#define g_Maintenance (*CMaintenance::instance())

#endif //__Maintenance_H__
