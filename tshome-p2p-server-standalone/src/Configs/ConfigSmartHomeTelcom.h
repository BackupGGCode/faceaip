/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigSmartHomeTelcom.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: ConfigSmartHomeTelcom.h 5884 2012-09-07 10:46:05Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-09-07 10:46:05  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/



#ifndef __ConfigSmartHomeTelcom_H__
#define __ConfigSmartHomeTelcom_H__

#include "ConfigBase.h"
//!
typedef struct tagConfigSmartHomeTelcom
{
    std::string strHost;// ƽ̨����
    int iPort;// �˿�
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    // ��������
    std::string ProductID 		;//String ���ұ�ţ����ǻ�E ��ƽ̨�����̷���
    std::string ProductName 	;//String ����������tiansu
    std::string AuthUser 		;//string ��Ȩ�û���
    std::string AuthPassword 	;//string ��Ȩ�û�����


    // Ŀǰ�����ϱ������豸��ʱ���ϱ��� ƽ̨,
    // ƽ̨�Լ��� strURLRegisterServerHost+strURL_register������
    std::string strURLRegisterServerHost							;
    // ���·������˿���ֻ�� iRegisterServerPort�� ���౸��
    int iRegisterServerPort;// ע��������˿�
    int iReportDeviceConfigParamServerPort;// �������˿�
    int iCtrlDeviceStateServerPort;// �������˿�
    int iDeviceStateList;// �������˿�
    int iControlDeviceServerPort;// �������˿�

    //this module �ϱ����url
    std::string strURL_requestAuth;
    std::string strURL_initDeviceConfigParam;
    std::string strURL_reportCtrlDeviceState;
    std::string strURL_reportDeviceStateList;
    std::string strURL_reportAlarm;

    // this module �յ���
    std::string strURL_register							;
    std::string strURL_requestReportDeviceConfigParam   ;
    std::string strURL_getCtrlDeviceState               ;
    std::string strURL_getDeviceStateList               ;
    std::string strURL_controlDevice                    ;

}
ConfigSmartHomeTelcom;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigSmartHomeTelcom, 1, 4> CConfigSmartHomeTelcom;

#endif //__ConfigSmartHomeTelcom_H__
