/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentHttpKeepConnector.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralAgentHttpKeepConnector.h 5884 2012-07-10 04:10:20Z WuJunjie $
 *
 *  Explain:
 *     -
 *      http ��������
 *      �������ӣ��Ͽ����Զ��ٴ�����
 *     -
 *
 *  Update:
 *     2012-07-10 04:10:20  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/


#ifndef _GeneralAgentHttpKeepConnector_H
#define _GeneralAgentHttpKeepConnector_H

#include "SmartHomeTelcomDef.h"

#include "TcpSocket.h"
#include <ISocketHandler.h>
/*ezutil/http_parser.h*/
#include <http_parser.h>

#include "EZThread.h"
#include "EZTimer.h"
#include "EZSignals.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "TeHomeIF.h"

// �߳�����
//#define CGENERALAGENTHTTPKEEPCONNECTOR_THREAD_ATTR "CHttpKeepConnectorSmartHomeTelcom", TP_DEFAULT, 100

#define CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME "GeneralAgentHttpKeepConnector"
#define HTTP_USER_AGENT "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/22.0.1207.1 Safari/537.1"

typedef enum __EnCommunicationStatus_t {
    EnCommunicationStatus_Initialize =  0,	// ��ʼ��
    EnCommunicationStatus_Connecting,	// ����
    EnCommunicationStatus_Connected,	// �Ѿ�����
    EnCommunicationStatus_FactoryAuthing,	// ��֤
    EnCommunicationStatus_FactoryAuthed,	// ��֤�ɹ�

    EnCommunicationStatus_initDeviceConfigParamed		,
    EnCommunicationStatus_initDeviceConfigParaming    ,
    EnCommunicationStatus_reportCtrlDeviceStateed     ,
    EnCommunicationStatus_reportCtrlDeviceStateing    ,
    EnCommunicationStatus_reportDeviceStateListed     ,
    EnCommunicationStatus_reportDeviceStateListing    ,
    EnCommunicationStatus_reportAlarmed               ,
    EnCommunicationStatus_reportAlarming              ,

    EnCommunicationStatus_Waite,	// ��ͨ
}EN_COMMUNICATION_STATUS_T;


class CHttpKeepConnectorSmartHomeTelcom : public TcpSocket//,public CEZThread
{
public:
    CHttpKeepConnectorSmartHomeTelcom(ISocketHandler&, const std::string &strSocketName=CGENERALAGENTHTTPKEEPCONNECTOR_SOCKET_NAME);
    ~CHttpKeepConnectorSmartHomeTelcom();

    void SetCloseAndDelete();
    void OnConnectFailed();
    void OnDelete();

    // �� TcpSocket::OnRead(); �е���
    virtual void OnRawData(const char *buf,size_t len);

    /////////////////////////////
    // �����������
    virtual void OnConnect();
    virtual void OnReconnect();
    virtual void OnDisconnect();
    virtual void OnConnectTimeout();
    virtual void OnBody(char *pCmdData, unsigned int ulCmdDataLen);
    virtual void OnURL(std::string strURL);
    const std::string& GetUrl();
    virtual void OnCommand(char *pCmdData, unsigned int ulCmdDataLen);

    /////////////////////////////

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    void Update();
    int SetCommunicationStatus(int CommunicationStatus);

    // �����˴�����Ҫ������֤
    int IniStatus();

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // ͨѶ�ӿ�
    int requestAuth();
    int initDeviceConfigParam();
    // �ϱ����ݵ� �����ݵ��¼ӻ���ɾ����ʱ����ô˽ӿ�
    // UpDown 1 ���� 0 ����
    int reportDeviceList(CAgentMsg_initDeviceConfigParam &Device, int UpDown=1);

    // �ϱ�����״̬
    // OnOFF 1 �� 0 ��
    int reportCtrlDeviceState(std::string strDeviceID, int OnOFF);
    // �ϱ��豸״̬��Ϣ
    int reportDeviceStateList(std::string strDeviceID	\
                    , std::string strOperate_id	\
                    , std::string strOperate_value	\
                    , std::string strOperate_ranage	\
                    , std::string strOperate_type	\
                    , std::string strOperate_explain	\
                   );
    // �ϱ� ����״̬��Ϣ
    int reportAlarm(std::string strDeviceID	\
                    , std::string strOperate_id	\
                    , std::string strOperate_value	\
                    , std::string strOperate_ranage	\
                    , std::string strOperate_type	\
                    , std::string strOperate_explain	\
                   );

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    int reportDeviceStateListExample(); // == reportAlarm
    int reportCtrlDeviceState();
    int reportAlarmExample();
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

private:
    CHttpKeepConnectorSmartHomeTelcom(const CHttpKeepConnectorSmartHomeTelcom& s) : TcpSocket(s)//,CEZThread(CHttpKeepConnectorSmartHomeTelcom_THREAD_ATTR)
    {} // copy constructor
    CHttpKeepConnectorSmartHomeTelcom& operator=(const CHttpKeepConnectorSmartHomeTelcom& )
    {
        return *this;
    } // assignment operator
    /** Create a new instance and reconnect */
    CHttpKeepConnectorSmartHomeTelcom *Reconnect();
    ssize_t SendPostStr(const std::string &page, const std::string &poststr);

    bool m_b_connected;
    time_t m_tOnData; ///< last Time in seconds when this socket was on data

    /////////////
    http_parser *m_httpParser;
    http_parser_settings *m_httpSettings;

    // ״̬, �����EN_COMMUNICATION_STATUS_T�Ķ���
    int m_iCommunicationStatus;

    CEZMutex m_mutexCommunicationStatus;

    std::string m_strSessionId;
    time_t m_ttLastSendData;

    //
    unsigned int m_iInitDeviceConfigParamNo;
    CEZMutex m_mutexInitDeviceConfigParamNo;

    std::vector<CAgentMsg_initDeviceConfigParam> deviceVec;
    // �����豸����
    //���reload = 0��ʾ����Ҫ�����ݿ⣬�����ز����ݿ�
    ssize_t getDeviceNum(int reload=0);
    //��ȡ�豸, ���noΪ-1�� ��Ϊ˳��ȡ��һ���豸
    //���no >= 0,��Ϊȡָ��λ�õ�ֵ��
    // return: 0- �ɹ� -1-δȡ�� -2 - �����no������Χ
    int getDevice(CAgentMsg_initDeviceConfigParam &DeviceCfg, int No=-1);

    std::string m_strCurrentURL;

    // this module �ϱ����url
    std::string m_strURL_requestAuth					;
    std::string m_strURL_initDeviceConfigParam			;
    std::string m_strURL_reportCtrlDeviceState			;
    std::string m_strURL_reportDeviceStateList			;
    std::string m_strURL_reportAlarm					;

    std::string m_strURL_register						  ;
    std::string m_strURL_requestReportDeviceConfigParam   ;
    std::string m_strURL_getCtrlDeviceState               ;
    std::string m_strURL_getDeviceStateList               ;
    std::string m_strURL_controlDevice                    ;

	int m_iConnTimes;
};

#endif // _GeneralAgentHttpKeepConnector_H
