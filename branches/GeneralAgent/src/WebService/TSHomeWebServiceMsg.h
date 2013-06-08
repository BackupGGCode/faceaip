/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * TSHomeWebServiceMsg.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: TSHomeWebServiceMsg.h 5884 2013-01-29 03:20:29Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-01-29 03:20:29  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _TSHomeWebServiceMsg_H
#define _TSHomeWebServiceMsg_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

//
// �򵥷�����Ϣ
//
class CCommonSimpleMsg
{
public:
	//0:��ʾ�ɹ�
    //1��ʧ��ԭ��μ�Reason
    //2��HTTP �������ʧ��ԭ��
    //�μ�Reason
    std::string result;
    std::string reason;

    CCommonSimpleMsg()
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
class CAuthResponse : public CCommonSimpleMsg
{
public:
    std::string sessionId;
    CAuthResponse()
    {
        sessionId = "";
    }
    void dump()
    {
        std::cout << "sessionId:" << sessionId << std::endl;
        CCommonSimpleMsg::dump();
    }
};

//
// WEB��ϵͳ��֤�ӿ�
//
class CIPC_requestAuth
{
public:
    std::string authUser 		;//string �û���
    std::string authPassword 	;//string �û�����

    CIPC_requestAuth()
    {
        authUser 	 = "";
        authPassword = "";
    }
    void dump()
    {
        std::cout << "authUser    :" << authUser   << std::endl;
        std::cout << "authPassword:" << authPassword << std::endl;
    }
};

//
//������֤�ӿ�
//
class CIPC_requestAuthByHost
{
public:
    std::string sessionId 		;//string WEB��ϵͳ��sessionId
    std::string homeIdStr 		;//string ��¼������homeId
    std::string userName 		;//string ��ͥ�û���
    std::string password 		;//string ��ͥ�û�����

    CIPC_requestAuthByHost()
    {
        sessionId 	 = "";
        homeIdStr 	 = "";
        userName 	 = "";
        password 	 = "";
    }
    void dump()
    {
        std::cout << "sessionId    :" 	<< sessionId   	<< std::endl;
        std::cout << "homeIdStr	   :" 	<< homeIdStr 	<< std::endl;
        std::cout << "userName     :" 	<< userName   	<< std::endl;
        std::cout << "password	   :" 	<< password 	<< std::endl;
    }
};

//
// ���ش���session����Ϣ
//
class CAuthHostResponse : public CCommonSimpleMsg
{
public:
    std::string sessionId2;
    CAuthHostResponse()
    {
        sessionId2 = "";
    }
    void dump()
    {
        std::cout << "sessionId2:" << sessionId2 << std::endl;
        CCommonSimpleMsg::dump();
    }
};

//
//WEB��ϵͳ��������ϵͳע��ӿڵ�ַ�Ľӿ�
//
class CIPC_registerURL
{
public:
    std::string sessionId ;//String 
    std::string baseURL ;//String 
    CIPC_registerURL()
    {
        sessionId = "" ;
        baseURL = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "baseURL :" <<  baseURL  << endl;
    }
}
;

//
//
//
class CIPC_registerURL_ACK : public CCommonSimpleMsg
{
}
;

class CIPC_heartbeat
{
public:
    std::string sessionId ;//String 
    CIPC_heartbeat()
    {
        sessionId = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
    }
}
;

//
//
//
class CIPC_heartbeat_ACK : public CCommonSimpleMsg
{
}
;


class CIPC_stopHostNetwork
{
public:
    std::string sessionId ;//String 
    std::string homeIdStr ;//String 
    std::string username ;//String 
    CIPC_stopHostNetwork()
    {
        sessionId = "" ;
        homeIdStr = "" ;
        username = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "homeIdStr :" <<  homeIdStr  << endl;
        cout << "username :" <<  username  << endl;
    }
}
;

//
//
//
class CIPC_stopHostNetwork_ACK : public CCommonSimpleMsg
{
}
;

class CIPC_checkUpdate
{
public:
    std::string sessionId ;//String 
    std::string homeIdStr ;//String 
    std::string lastTime ;//String 
    std::string aesKey ;//String 
    std::string queryFrom ;//String 
    std::string product ;//String 
    CIPC_checkUpdate()
    {
        sessionId = "" ;
        homeIdStr = "" ;
        lastTime = "" ;
        aesKey = "" ;
        queryFrom = "" ;
        product = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "homeIdStr :" <<  homeIdStr  << endl;
        cout << "lastTime :" <<  lastTime  << endl;
        cout << "aesKey :" <<  aesKey  << endl;
        cout << "queryFrom :" <<  queryFrom  << endl;
        cout << "product :" <<  product  << endl;
    }
}
;

//
//
//
class CIPC_checkUpdate_ACK : public CCommonSimpleMsg
{
public:
    std::string cfgForce;
    std::string sessionId ;//String 
    std::string homeIdStr ;//String 
    std::string cfgUrl ;//String 
    std::string softForce ;//String 
    std::string queryFrom ;//String 
    std::string softUrl ;//String 
    std::string md5 ;//String 
    CIPC_checkUpdate_ACK()
    {
        sessionId = "" ;
        homeIdStr = "" ;
        cfgForce = "" ;
        cfgUrl = "" ;
        queryFrom = "" ;
        softForce = "" ;
        softUrl = "" ;
        md5 = "" ;
    }
    void dump()
    {
        cout << "cfgForce:" << cfgForce << std::endl;
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "homeIdStr :" <<  homeIdStr  << endl;
        cout << "cfgForce :" <<  cfgForce  << endl;
        cout << "softForce :" <<  softForce  << endl;
        cout << "queryFrom :" <<  queryFrom  << endl;
        cout << "softUrl :" <<  softUrl  << endl;
        cout << "md5 :" <<  md5  << endl;
        CCommonSimpleMsg::dump();
    }
}
;

class CIPC_notifyUpdate
{
public:
    std::string sessionId ;//String 
    std::string homeIdStr ;//String 
    CIPC_notifyUpdate()
    {
        sessionId = "" ;
        homeIdStr = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "homeIdStr :" <<  homeIdStr  << endl;
    }
}
;

//
//
//
class CIPC_notifyUpdate_ACK : public CCommonSimpleMsg
{
}
;

class CIPC_reportDeviceStateList_operate
{
public:
    std::string dataPoint 		;//string ���ݵ�ID
    std::string dataLen 	;//string ����ֵ���ַ�������
    std::string data 	;//String ʵ�ʵ�����ֵ

    
    CIPC_reportDeviceStateList_operate()
	{
		dataPoint	   = "";
		dataLen   = "";
		data  = "";
	}
	void dump()
	{
		cout << "dataPoint :" <<  dataPoint  << endl;
		cout << "dataLen :" <<	dataLen  << endl;
		cout << "data :" <<  data  << endl;
	
	}

}
;
class CIPC_reportDeviceStateList
{
public:
    std::string homeIdStr ;//String 
    
    std::vector<CIPC_reportDeviceStateList_operate> m_operate;
    CIPC_reportDeviceStateList()
    {
        homeIdStr = "" ;
    }
    void dump()
    {
        cout << "homeIdStr :" <<  homeIdStr  << endl;
	}
}
;

class CIPC_reportDeviceStateList_ACK : public CCommonSimpleMsg
{
}
;

class CIPC_getDeviceStateList_operate
{
public:
    std::string dataPoint 		;//string ���ݵ�ID
    
    CIPC_getDeviceStateList_operate()
    {
        dataPoint      = "";
    }
    void dump()
    {
        cout << "dataPoint :" <<  dataPoint  << endl;

	}
}
;


//
// ��ȡ�豸״̬�ӿ�
//
class CIPC_getDeviceStateList
{
public:
    std::string sessionId ;//String
    std::string sessionId2 ;//String
    std::string homeIdStr ;//String 
    
    std::vector<CIPC_getDeviceStateList_operate> m_operate;
    CIPC_getDeviceStateList()
    {
        sessionId = "" ;
        sessionId2 = "" ;
        homeIdStr = "" ;
    }
    void dump()
    {
        cout << "sessionId :" <<  sessionId  << endl;
        cout << "sessionId2 :" <<  sessionId2  << endl;
        cout << "homeIdStr :" <<  homeIdStr  << endl;
	}
}
;


class CIPC_getDeviceStateList_ACK_operate
{
public:
    std::string dataPoint 		;//string ���ݵ�ID
    std::string dataLen 	;//string ����ֵ���ַ�������
    std::string data 	;//String ʵ�ʵ�����ֵ
    
    CIPC_getDeviceStateList_ACK_operate()
    {
        dataPoint      = "";
        dataLen   = "";
        data  = "";
    }
    void dump()
    {
        cout << "dataPoint :" <<  dataPoint  << endl;
        cout << "dataLen :" <<  dataLen  << endl;
        cout << "data :" <<  data  << endl;

	}
}
;
class CIPC_getDeviceStateList_ACK : public CCommonSimpleMsg
{
public:
    std::vector<CIPC_getDeviceStateList_ACK_operate> m_operate;

    CIPC_getDeviceStateList_ACK()
    {
        result 	= "";
        reason 	= "";
        m_operate.clear();
    }
    void dump()
    {}
}
;

//
// ��������ӿ� ��operate����
//
class CIPC_controlDevice_operate
{
public:
    std::string dataPoint 		;//String 
    std::string dataLen 	;//String 
    std::string data 	;//string 
    CIPC_controlDevice_operate()
    {
        dataPoint      = "";
        dataLen  = "";
        data    = "";
    }
    void dump()
    {
        cout << "dataPoint :" <<  dataPoint  << endl;
        cout << "dataLen :" <<  dataLen  << endl;
        cout << "data :" <<  data  << endl;
	}
}
;


//
// ��������ӿڵķ�����Ϣ - operate ����
//
class CIPC_controlDevice_ACK_Operate
{
public:
    std::string dataPoint 		;//String 
    std::string dataLen 	;//String 
    std::string data 	;//string 
    CIPC_controlDevice_ACK_Operate()
    {
        dataPoint  = "";
        dataLen  = "";
        data    = "";
    }
    void dump()
    {}
}
;

//
// ��������ӿ�
//
class CIPC_controlDevice
{
public:
    std::string sessionId 		;//String 
    std::string sessionId2 		;//String 
    std::string homeIdStr ;//String 
    std::vector<CIPC_controlDevice_operate> m_operate;
    CIPC_controlDevice()
    {
        sessionId = "";
        sessionId2 = "";
        homeIdStr = "";
        m_operate.clear();
    }
    void dump()
    {}
}
;


//
// ��������ӿڵķ�����Ϣ
//
class CIPC_controlDevice_ACK : public CCommonSimpleMsg
{
public:
    std::vector<CIPC_controlDevice_ACK_Operate> m_operate;
    //Operate ����������
    CIPC_controlDevice_ACK()
    {
    	result = "";
		reason = "";
        m_operate.clear();
    }
    void dump()
    {}
}
;
#endif /* _TSHomeWebServiceMsg_H */
