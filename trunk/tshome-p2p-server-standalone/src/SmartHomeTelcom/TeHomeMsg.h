/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * TeHomeMsg.h - _explain_
 *
 * Copyright (C) 2012 tiansu-china.com, All Rights Reserved.
 *
 * $Id: TeHomeMsg.h 0001 2012-5-8 14:38:29Z wu_junjie $
 *
 *  Explain:
 *     TeHomeMsg -- Telecom eHome Msg 2.0
 *
 *  Update:
 *     2012-5-8 14:38:33 WuJunjie 549 Create
 *
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _TEHOMEMSG_H
#define _TEHOMEMSG_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//
// �򵥷�����Ϣ
//
class CAckMsgSimple
{
public:
    std::string result;
    std::string reason;

    CAckMsgSimple()
    {
        result = "";
        reason = "";
    }

    void dump()
    {
        std::cout << "result:" << result << std::endl;
        std::cout << "reason:" << reason << std::endl;
    }
};

//
// ���ش���session����Ϣ
//
class CAckMsgSession : public CAckMsgSimple
{
public:
    std::string SessionId;
    CAckMsgSession()
    {
        SessionId = "";
    }
    void dump()
    {
        std::cout << "SessionId:" << result << std::endl;
        CAckMsgSimple::dump();
    }
};

//
// ����������֤�ӿ�
//
class CAgentMsg_requestAuth
{
public:
    // ������
    std::string ProductID 		;//String ���ұ�ţ����ǻ�E ��ƽ̨�����̷���
    // ������
    std::string ProductName 	;//String ����������tiansu
    // ������
    std::string AuthUser 		;//string ��Ȩ�û���
    // ������
    std::string AuthPassword 	;//string ��Ȩ�û�����

    CAgentMsg_requestAuth()
    {
        ProductID 	 = "";
        ProductName  = "";
        AuthUser 	 = "";
        AuthPassword = "";
    }
    void dump()
    {
        std::cout << "ProductID   :" << ProductID  << std::endl;
        std::cout << "ProductName :" << ProductName  << std::endl;
        std::cout << "AuthUser    :" << AuthUser   << std::endl;
        std::cout << "AuthPassword:" << AuthPassword << std::endl;
    }
};


//
// �ͻ������豸���ؼ�����֤�ӿ�
//
class CAgentMsg_certificationActivation
{
public:

    // ��ȡ������ ��������
    std::string SessionId 	;//string
    // ������
    std::string UserId 		;//String �û����      CAgentMsg_certificationActivation()
    // ������
    std::string DeviceId 	;//String �豸���    {

    CAgentMsg_certificationActivation()
    {
        SessionId 	 = "";
        UserId       = "";
        DeviceId 	 = "";
    }
    void dump()
    {
        cout << "SessionId:"<< SessionId<< endl;
        cout << "UserId   :"<< UserId   << endl;
        cout << "DeviceId :"<< DeviceId << endl;
    }
};

////////////////////////////////
//
// �����ϱ��ӿ�
//
class CAgentMsg_initDeviceConfigParam_Seat_deploy_Operate
{
public:
    // ��Ҫ��ģ�� ��ȡ
    std::string operate_id 		;//String �豸������ţ����豸�Ŀ��غ�ƥ�䣻
    // ��Ҫ��ģ�� ��ȡ
    std::string operate_ranage 	;//String �豸�ܿ�����Сֵ�����ֵ����	�磺1-33
    // ��Ҫ��ģ�� ��ȡ
    std::string operate_type 	;//String �豸�������ͣ�0: ���ر�־��1��
    //���ȱ�־����ΧΪ0-999�� 2��
    //�̶�ֵ��ʽ�����¶�Ϊ16,26 �ȣ�
    // ��Ҫ��ģ�� ��ȡ
    std::string operate_value 	;//String �豸�������͵Ľ��ֵ�� ��
    //operate-type ���ʹ�á���
    //operate-type=0 ʱ��
    //operate-value=1 ��ʾ����
    //operate-value=2 ��ʾ��
    //��operate-type=1 ʱ��
    //operate-value ��ʾ����ķ�Χ
    //֮���ֵ����operate-value=325
    //��operate-type=2 ʱ��
    //operate-value ��ʾ�����ֵ��
    //��operate-value=16
    // ��Ҫ��ģ�� ��ȡ
    std::string operate_explain ;//string ����˵������

    CAgentMsg_initDeviceConfigParam_Seat_deploy_Operate()
    {
        operate_id 		= "";//String �豸������ţ����豸�Ŀ��غ�ƥ�䣻
        operate_ranage 	= "";//String �豸�ܿ�����Сֵ�����ֵ����	�磺1-33
        operate_type 	= "";//String �豸�������ͣ�0: ���ر�־��1��
        operate_value 	= "";//String �豸�������͵Ľ��ֵ�� ��
        operate_explain = "";//string ����˵������
    }
    void dump()
    {
        cout << "operate_id     :" << operate_id     << endl;
        cout << "operate_ranage :" << operate_ranage << endl;
        cout << "operate_type   :" << operate_type   << endl;
        cout << "operate_value  :" << operate_value  << endl;
        cout << "operate_explain:" << operate_explain<< endl;
    }
};

//
// �����ϱ��ӿ�
//
class CAgentMsg_initDeviceConfigParam_Seat_deploy
{
public:
    // ��Ҫ��ģ�� ��ȡ
    std::string deploy_name 	;//String ����
    // ��Ҫ��ģ�� ��ȡ
    std::string deploy_type 	;//String �豸�����ͣ�1�������豸��2���Ҿ��豸��
    // ��Ҫ��ģ�� ��ȡ
    std::string deviceType 		;//String ���ݲ�������
    //1������
    //2���յ�
    //3������
    //4������
    //5������
    //6������
    //7����ˮ
    //0������
    // ��Ҫ��ģ�� ��ȡ
    std::string device_model	;//String �豸�ͺ�

    // ��Ҫ��ģ�� ��ȡ�� CAgentMsg_initDeviceConfigParam_Seat_deploy_Operate ����
    std::vector<CAgentMsg_initDeviceConfigParam_Seat_deploy_Operate> m_Operate;
    CAgentMsg_initDeviceConfigParam_Seat_deploy()
    {
        deploy_name 	= "";//String ����
        deploy_type 	= "";//String �豸�����ͣ�1�������豸��2���Ҿ��豸��
        deviceType 		= "";//String ���ݲ�������
        device_model	= "";//String �豸�ͺ�
    }
    void dump()
    {
        cout << "deploy_name :" <<  deploy_name  << endl;
        cout << "deploy_type :" <<  deploy_type  << endl;
        cout << "deviceType  :" <<  deviceType   << endl;
        cout << "device_model:" <<  device_model << endl;
        for (vector<CAgentMsg_initDeviceConfigParam_Seat_deploy_Operate>::iterator iter=m_Operate.begin()
                ; iter!=m_Operate.end()
             ; ++iter)
        {
            iter->dump();
        }
    }
};

//
// �����ϱ��ӿ�
//
class CAgentMsg_initDeviceConfigParam_Seat
{
public:
    // ��Ҫ��ģ�� ��ȡ
    std::string seat_name 		;//String ���ִ󻷾������ƣ������
    // ��Ҫ��ģ�� ��ȡ�� �������ݼ�CAgentMsg_initDeviceConfigParam_Seat_deploy ����
    // 2012-9-5 17:22:35 ����ͬ���ŵĹ�ͨ����� ֻ��һ�����ϴ�������Ŀǰֻ��Ϊ 1
    std::vector<CAgentMsg_initDeviceConfigParam_Seat_deploy> m_deploy;
    CAgentMsg_initDeviceConfigParam_Seat()
    {
        seat_name = "";
    }
    void dump()
    {
        cout << "seat_name:" <<  seat_name << endl;
        for (vector<CAgentMsg_initDeviceConfigParam_Seat_deploy>::iterator iter=m_deploy.begin()
                ; iter!=m_deploy.end()
             ; ++iter)
        {
            iter->dump();
        }
    }
}
;
////////////////////////////////
//
// �����ϱ��ӿ�
//
class CAgentMsg_initDeviceConfigParam
{
public:
    std::string requestMark 	;//String �����ʾ������ֵΪ��
    //add��update��delete
    //(add��update��delete ��������
    //��deviceID ��operate_id Ϊ�ж�����)

    // �Ѷ���
    std::string sessionId 		;//String
    // �Ѷ���
    std::string productID 		;//String ���ұ��ID
    // �Ѷ���
    std::string userID 			;//String �û����
    // �Ѷ���
    std::string deviceID 		;//string �豸���

    // ��Ҫ��ģ�� ��ȡ�� ����� CAgentMsg_initDeviceConfigParam_Seat �еĶ���
    CAgentMsg_initDeviceConfigParam_Seat m_Seat;

    CAgentMsg_initDeviceConfigParam()
    {
        requestMark 	= "";//String
        sessionId 		= "";//String
        productID 		= "";//String ���ұ��ID
        userID 			= "";//String �û����
        deviceID 		= "";//string �豸���
    }
    void dump()
    {
            cout << "requestMark :" <<  requestMark  << endl;
            cout << "sessionId :" <<  sessionId  << endl;
            cout << "productID :" <<  productID  << endl;
            cout << "userID :" <<  userID  << endl;
            cout << "deviceID :" <<  deviceID  << endl;

        m_Seat.dump();
    }
}
;

//
// ����豸�ӿ�
//
class CAgentMsg_addDeviceConfigParam
{
public:
 	//deviceName, description, room, permission, deviceType, subType, gateway, zone, startRow, startColumn, rowCount, columnCount
 	//device, ctrlType, datatype, grouptype, t_data_point.value
 	//dataPoint, enoceanId, choiceType, funcType, appType, rocker, autoLearned, totalTime, totalSteps
    std::string deviceName 	;

    std::string description 	;//String
    
    std::string room 		;//room ID
    
    std::string permission 		;//permission value (int)
    
    std::string deviceType 		;//string �豸���
    
    std::string subType 	;
    
    std::string gateway 		;//room ID
    
    std::string zone 		;//permission value (int)
    
    std::string startRow 		;//string �豸���

	std::string startColumn 	;

    std::string rowCount 	;//String
    
    std::string columnCount 		;//room ID
    
    std::string deviceId 		;
    
    std::string ctrlType 		;

	std::string datatype 	;

    std::string grouptype 	;//String
    
    std::string value 		;//room ID
    
    std::string dataPoint 		;//permission value (int)
    
    std::string enoceanId 		;//string �豸���
    
    std::string choiceType 	;

    std::string funcType 	;//String
    
    std::string appType 		;//room ID
    
    std::string rocker 		;//permission value (int)
    
    std::string autoLearned 		;//string �豸���

	std::string totalTime 	;

    std::string totalSteps 	;


    CAgentMsg_addDeviceConfigParam()
    {
        
    }
    void dump()
    {
    	cout << endl;
        cout << "deviceName:" << deviceName << endl;

	    cout << " description:" << description << endl 	;//String
	    
	    cout << " room:" << room << endl 		;//room ID
	    
	    cout << " permission:" << permission << endl 		;//permission value (int)
	    
	    cout << " deviceType:" << deviceType << endl 		;//string �豸���
	    
	    cout << " subType:" << subType << endl 	;
	    
	    cout << " gateway:" << gateway << endl 		;//room ID
	    
	    cout << " zone :" << zone << endl		;//permission value (int)
	    
	    cout << " startRow :" << startRow << endl		;//string �豸���

		cout << " startColumn:" << startColumn << endl 	;

	    cout << " rowCount:" << rowCount << endl 	;//String
	    
	    cout << " columnCount:" << columnCount << endl 		;//room ID
	    
	    cout << " deviceId:" << deviceId << endl 		;
	    
	    cout << " ctrlType:" << ctrlType << endl 		;

		cout << " datatype:" << datatype << endl 	;

	    cout << " grouptype:" << grouptype << endl 	;//String
	    
	    cout << " value:" << value << endl 		;//room ID
	    
	    cout << " dataPoint:" << dataPoint << endl 		;//permission value (int)
	    
	    cout << " enoceanId:" << enoceanId << endl 		;//string �豸���
	    
	    cout << " choiceType:" << choiceType << endl 	;

	    cout << " funcType:" << funcType << endl 	;//String
	    
	    cout << " appType:" << appType << endl 		;//room ID
	    
	    cout << " rocker:" << rocker << endl 		;//permission value (int)
	    
	    cout << " autoLearned:" << autoLearned << endl 		;//string �豸���

		cout << " totalTime :" << totalTime << endl	;

	    cout << " totalSteps:" << totalSteps << endl 	;
    }
}
;


//
// �ϱ�����״̬�ӿ�
//
class CAgentMsg_ReportCtrlDeviceState
{
public:
    std::string SessionId 	;//String
    std::string ProductID 	;//String ���̱��(ID)
    std::string DeviceID 	;//string �豸���
    std::string State 		;//string ����/������
    std::string StateTime 	;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    std::string strIP 	;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    //		HH:NN:SS����-��-��ʱ���֣��룩

    CAgentMsg_ReportCtrlDeviceState()
    {
        SessionId 	= "";
        ProductID 	= "";
        DeviceID 	= "";
        State       = "";
        StateTime 	= "";

        strIP = "";
    }
    void dump()
    {
        cout << endl;
        cout << "SessionId:" << SessionId << endl;
        cout << "ProductID:" << ProductID << endl;
        cout << "DeviceID :" << DeviceID  << endl;
        cout << "State    :" << State     << endl;
        cout << "StateTime:" << StateTime << endl;
        cout << "strIP:" << strIP << endl;
    }
}
;

//
//
//
class CAgentMsg_reportDeviceStateList_operate
{
public:
    std::string operate_id 		;//string �豸������ţ����豸�Ŀ��غ�
    //			��ƥ��
    std::string operate_value 	;//string operate-value���豸�������͵�
    //		���ֵ����operate-type ���ʹ
    //		�á���operate-type=0 ʱ��
    //		operate-value=1 ��ʾ����
    //		operate-value=2 ��ʾ��
    //		��operate-type=1 ʱ��
    //		operate-value ��ʾ����ķ�Χ
    //	֮���ֵ����operate-value=325
    //	��operate-type=2 ʱ��
    //	operate-value ��ʾ�����ֵ��
    //	��operate-value=16
    std::string operate_ranage 	;//String �豸�ܿ�����Сֵ�����ֵ����
    //			�磺1-33
    std::string operate_type 	;//string operate-type���豸�������ͣ�0:
    //	���ر�־��1�����ȱ�־����ΧΪ
    //	0-999�� 2���̶�ֵ��ʽ�����¶�
    //	Ϊ16,26 �ȣ�
    std::string operate_explain ;//string ˵��
    std::string value_time 		;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    //	HH:NN:SS����-��-��ʱ���֣��룩

    CAgentMsg_reportDeviceStateList_operate()
    {
        operate_id      = "";//string �豸������ţ����豸�Ŀ��غ�
        operate_value   = "";//string operate-value���豸�������͵�
        operate_ranage  = "";//String �豸�ܿ�����Сֵ�����ֵ����
        operate_type    = "";//string operate-type���豸�������ͣ�0:
        operate_explain = "";//string ˵��
        value_time      = "";//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    }
    void dump()
    {}
}
;

//
// �ϱ��豸״̬�ӿ�
//
class CAgentMsg_reportDeviceStateList
{
public:
    std::string sessionId;//String
    std::string productID;//string ����ID
    std::string deviceID ;//string �豸���
    //operate �����豸��
    std::vector<CAgentMsg_reportDeviceStateList_operate> m_operate;

    CAgentMsg_reportDeviceStateList()
    {

        sessionId 	= "";
        productID 	= "";
        deviceID 	= "";
    }
    void dump()
    {
        cout << "sessionId:" << sessionId << endl;
        cout << "productID:" << productID << endl;
        cout << "deviceID :" << deviceID  << endl;
        cout << "m_operate.size():" << m_operate.size() << endl;
    }
}
;

//
// �澯�ϱ��ӿ�
//
class CAgentMsg_reportAlarm
{
public:
    std::string sessionId;//String
    std::string deviceID ;//string �豸���
    //operate �����豸��
    std::vector<CAgentMsg_reportDeviceStateList_operate> m_operate;

    CAgentMsg_reportAlarm()
    {

        sessionId 	= "";
        deviceID 	= "";
    }
    void dump()
    {
        cout << "sessionId:" << sessionId << endl;
        cout << "deviceID :" << deviceID  << endl;
    }
}
;

// ͬ CAgentMsg_ReportCtrlDeviceState�� �����ٶ��壬 ��ֹƽ̨�б仯
//
// ���ؽ���״̬�ӿ�
//
class CAgentMsg_reportGatewayState
{
public:
    std::string SessionId 	;//String
    std::string ProductID 	;//String ���̱��(ID)
    std::string DeviceID 	;//string �豸���
    std::string State 		;//string ����/������
    std::string StateTime 	;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    //		HH:NN:SS����-��-��ʱ���֣��룩

    CAgentMsg_reportGatewayState()
    {
        SessionId   = "";
        ProductID   = "";
        DeviceID    = "";
        State       = "";
        StateTime 	= "";
    }
    void dump()
    {}
}
;
//
// �豸�澯��Ϣ�ӿ�
//
class CAgentMsg_reportDeviceAlarm
{
public:
    std::string SessionId 	;//String
    std::string ProductID 	;//String ���̱��(ID)
    std::string DeviceID 	;//string �豸���
    std::string Operate_id 		;//string ����/������
    std::string Explain 		;//string ����/������
    std::string StateTime 	;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    //		HH:NN:SS����-��-��ʱ���֣��룩

    CAgentMsg_reportDeviceAlarm()
    {
        SessionId 	= "";
        ProductID 	= "";
        DeviceID 	= "";
        Operate_id 		= "";
        Explain 		= "";
        StateTime 	= "";
    }
    void dump()
    {}
}
;

//
// ����������Ϣ�ӿ�
//
class CAgentMsg_requestDeviceConfig
{
public:
    std::string SessionId 	;//String
    std::string ProductID 	;//String ���̱��(ID)

    CAgentMsg_requestDeviceConfig()
    {
        SessionId 	= "";
        ProductID 	= "";
    }
    void dump()
    {}
}
;


//
// ����������Ϣ�ӿ� - ack config
//
class CAgentMsg_requestDeviceConfig_ACK_Config
{
public:
    std::string Room ;//String ������
    //�����ǻ�E �ҽӿڹ淶V0.1
    //- 48 -
    std::string DeviceType ;//String �豸����
    std::string number ;//String �豸����

    CAgentMsg_requestDeviceConfig_ACK_Config()
    {
        Room 	= "";
        DeviceType 	= "";
        number 	= "";
    }
    void dump()
    {}
}
;


//
// ����������Ϣ�ӿ� - ack
//
class CAgentMsg_requestDeviceConfig_ACK
{
public:
    std::string Result 	;//String
    std::string Reason 	;//String ���̱��(ID)
    std::string Pointnum 	;//String Ƶ���
    std::vector<CAgentMsg_requestDeviceConfig_ACK_Config> m_config;
    CAgentMsg_requestDeviceConfig_ACK()
    {
        Result 	= "";
        Reason 	= "";
        Pointnum 	= "";
        m_config.clear();
    }
    void dump()
    {}
}
;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

//
// ���������ϱ��ӿ�
//
class CeHomeMsg_requestReportDeviceConfigParam
{
public:
    std::string UserId ;//String �û����
    std::string DeviceId ;//String �豸���

    CeHomeMsg_requestReportDeviceConfigParam()
    {
        UserId = "";
        DeviceId = "";
    }
    void dump()
    {}
}
;

//
// ���������ϱ��ӿڵķ�����Ϣ
//
class CeHomeMsg_requestReportDeviceConfigParam_ACK
{
public:
    std::string Result 		;//string ���
    std::string Reason 		;//string ʧ��ԭ��
    std::string productID 	;//String ���ұ��ID
    std::string userID 		;//String �û����
    std::string deviceID 	;//string �豸���
    CAgentMsg_initDeviceConfigParam_Seat m_Seat;

    CeHomeMsg_requestReportDeviceConfigParam_ACK()
    {
        Result 		="";
        Reason 		="";
        productID 	="";
        userID 		="";
        deviceID 	="";
    }
    void dump()
    {}
}
;


//
// ��ȡ����״̬�ӿ�
//
class CeHomeMsg_getCtrlDeviceState
{
public:
    std::string DeviceID ;//String �豸���
    CeHomeMsg_getCtrlDeviceState()
    {
        DeviceID = "" ;
    }
    void dump()
    {
        cout << "DeviceID:" << DeviceID << endl;
    }
}
;


//
//
//
class CeHomeMsg_getCtrlDeviceState_ACK
{
public:
    std::string Result 	;//string ���
    //0:��ʾ�ɹ�
    //1��ʧ��ԭ��μ�Reason
    //2��HTTP �������ʧ��ԭ��
    //�μ�Reason
    std::string Reason 	;//string ʧ��ԭ��
    std::string State 	;//string ����/������
    CeHomeMsg_getCtrlDeviceState_ACK()
    {
        Result = "" ;
        Reason = "" ;
        State = "" ;
    }
    void dump()
    {}
}
;

class CeHomeMsg_register
{
public:
    std::string UserID ;//String 
    std::string DeviceID ;//String 
    CeHomeMsg_register()
    {
        UserID = "" ;
        DeviceID = "" ;
    }
    void dump()
    {
        cout << "UserID :" <<  UserID  << endl;
        cout << "DeviceID :" <<  DeviceID  << endl;
    }
}
;

//
//
//
class CeHomeMsg_register_ACK
{
public:
    std::string Result 	;//string ���
    //0:��ʾ�ɹ�
    //1��ʧ��ԭ��μ�Reason
    //2��HTTP �������ʧ��ԭ��
    //�μ�Reason
    std::string Reason 	;//string ʧ��ԭ��
    CeHomeMsg_register_ACK()
    {
        Result = "" ;
        Reason = "" ;
    }
    void dump()
    {
        cout << "Result :" <<  Result  << endl;
        cout << "Reason :" <<  Reason  << endl;
    }
}
;

//
// ��ȡ�豸״̬�ӿ�
//
class CeHomeMsg_getDeviceStateList
{
public:
    std::string DeviceID ;//String �豸���
    CeHomeMsg_getDeviceStateList()
    {
        DeviceID = "" ;
    }
    void dump()
    {}
}
;

class CeHomeMsg_getDeviceStateList_ACK_operate
{
public:
    std::string operate_id 		;//string �豸������ţ����豸�Ŀ��غ�
    //			��ƥ��
    std::string operate_value 	;//string operate-value���豸�������͵�
    //		���ֵ����operate-type ���ʹ
    //		�á���operate-type=0 ʱ��
    //		operate-value=1 ��ʾ����
    //		operate-value=2 ��ʾ��
    //		��operate-type=1 ʱ��
    //		operate-value ��ʾ����ķ�Χ
    //	֮���ֵ����operate-value=325
    //	��operate-type=2 ʱ��
    //	operate-value ��ʾ�����ֵ��
    //	��operate-value=16
    std::string operate_ranage 	;//String �豸�ܿ�����Сֵ�����ֵ����
    //			�磺1-33
    std::string operate_type 	;//string operate-type���豸�������ͣ�0:
    //	���ر�־��1�����ȱ�־����ΧΪ
    //	0-999�� 2���̶�ֵ��ʽ�����¶�
    //	Ϊ16,26 �ȣ�
    std::string operate_explain ;//string ˵��
    std::string value_time 		;//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    //	HH:NN:SS����-��-��ʱ���֣��룩

    CeHomeMsg_getDeviceStateList_ACK_operate()
    {
        operate_id      = "";//string �豸������ţ����豸�Ŀ��غ�
        operate_value   = "";//string operate-value���豸�������͵�
        operate_ranage  = "";//String �豸�ܿ�����Сֵ�����ֵ����
        operate_type    = "";//string operate-type���豸�������ͣ�0:
        operate_explain = "";//string ˵��
        value_time      = "";//string ״̬ʱ�䣬��ʽΪYYYY-MM-DD
    }
    void dump()
    {}
}
;
class CeHomeMsg_getDeviceStateList_ACK
{
public:
    std::string result;//String
    std::string reason;//string ����ID

    //operate �����豸��
    std::vector<CeHomeMsg_getDeviceStateList_ACK_operate> m_operate;

    CeHomeMsg_getDeviceStateList_ACK()
    {

        result 	= "";
        reason 	= "";
    }
    void dump()
    {}
}
;

//
// ��������ӿ� ��operate����
//
class CeHomeMsg_controlDevice_operate
{
public:
    std::string operate_id 		;//String �豸��ַ��
    std::string operate_ranage 	;//String �豸�ܿ�����Сֵ�����ֵ�����磺
    std::string operate_type 	;//string �豸����
    std::string orderId 		;//string ������:
    std::string extendpara;
    //orderId :
    //1.����
    //2.����
    CeHomeMsg_controlDevice_operate()
    {
        operate_id      = "";
        operate_ranage  = "";
        operate_type    = "";
        orderId         = "";
        extendpara      = "";
    }
    void dump()
    {}
}
;


//
// ��������ӿ�
//
class CeHomeMsg_controlDevice
{
public:
    std::string DeviceID 		;//String �豸��ţ��������ţ�ָ��ͥ�п�
    //���豸Ψһ��ţ�
    std::vector<CeHomeMsg_controlDevice_operate> m_operate;
    //Operate ����������
    CeHomeMsg_controlDevice()
    {
        DeviceID = "";
        m_operate.clear();
    }
    void dump()
    {}
}
;


//
// ��������ӿڵķ�����Ϣ - operate ����
//
class CeHomeMsg_controlDevice_ACK_Operate
{
public:
    std::string operate_id 		;//String �������
    std::string resultState 	;//String �������ؽ��
    std::string stateTime 		;//string �������ʱ�䣬 ��ʽΪ
    //YYYY-MM-DD HH:NN:SS����
    //-��-��ʱ���֣��룩
    CeHomeMsg_controlDevice_ACK_Operate()
    {
        operate_id 	= "";
        resultState = "";
        stateTime 	= "";
    }
    void dump()
    {}
}
;
//
// ��������ӿڵķ�����Ϣ
//
class CeHomeMsg_controlDevice_ACK
{
public:

    std::string Result 		;//string 	���
    //	0:��ʾ�ɹ�
    //	1��ʧ�ܣ�ʧ��ԭ��μ�Reason
    //	2��HTTP �������ʧ��ԭ��
    //	�μ�Reason
    std::string Reason 		;//string 	ʧ��ԭ��
    std::string deviceID 	;//String �豸���
    std::vector<CeHomeMsg_controlDevice_ACK_Operate> m_operate;
    //Operate ����������
    CeHomeMsg_controlDevice_ACK()
    {
        Result     = "";
        Reason     = "";
        deviceID   = "";

        m_operate.clear();
    }
    void dump()
    {}
}
;

//{"deviceid":"0x010189e5","operate":"1","value":"1"}
//
// ���������صĿ�����Ϣ
//
#define INVALID_OPERATE 10000
class CTSHomeHostHttpMsg_Operate
{
public:

    std::string deviceid 		;
    std::string operate 		;
    std::string value 	;
    std::string devicetype;
    	
    long long m_ideviceid 	;
    int reserved_1;
    int reserved_2;
    int reserved_3;
    int reserved_4;
    int m_ioperate 		;
    int m_ivalue 	    ;
    int m_idevicetype;
    CTSHomeHostHttpMsg_Operate()
    {
        deviceid     = "";
        operate     = "";
        value   = "";
        devicetype   = "";

        m_ideviceid = 0	;
        m_ioperate 	= INVALID_OPERATE	;
        m_ivalue 	= 0 ;
        m_idevicetype = 0;
    }
    void dump()
    {
        cout << "CTSHomeHostHttpMsg_Operate::dump" << endl;
        cout << "\tdeviceid:" << deviceid << endl;
        cout << "\toperate :" << operate  << endl;
        cout << "\tvalue   :" << value    << endl;
        cout << "\tdevicetype   :" << devicetype    << endl;

        cout << "\tm_ideviceid:" << m_ideviceid << endl;
        cout << "\tm_ioperate :" << m_ioperate  << endl;
        cout << "\tm_ivalue   :" << m_ivalue    << endl;
        cout << "\tm_idevicetype   :" << m_idevicetype    << endl;
        //printf("\t\t(%#x,%d,%d)\n", m_ideviceid, m_ioperate, m_ivalue);
    }
}
;
#endif /* _TEHOMEMSG_H */
