/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * CmdsTypedef.h - _explain_
 *
 * Copyright (C) 2005 QiYang Technologies, All Rights Reserved.
 *
 * $Id: CmdsTypedef.h, v 1.0.0.1 2005-03-18 14:23:45 wjj Exp $
 *
 * _detail_explain_.
 * 
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _CMDSTYPEDEF_H
#define _CMDSTYPEDEF_H

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "../System/BaseTypedef.h"
#include "../Include/config-x.h"

#define STR_CFG "config"
#define STR_VDP "vdpara"
#define STR_TIM "timing"

#define ENCRYPT_DES	1
// des����
#define ENCRYPT_DES_KEY "shouldtodo"
#define ENCRYPT_DES_KEY_MOBILE "mobileview"

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+  ����״̬  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/* ������״̬ */
enum    EnTxStatus
{
    TxStInit  =  0,
    TxStWait  =  1,
    TxStNext  =  2,
    TxStTimeOut  =  3,
    TxStEnd   = -100,
};

/* ��������״̬��Ԥ���ģ�*/
enum    EnOpStatus
{
    OpStBegin =  101,
    OpStBeginDataSafe,
    /* ��֤ͨ�������� */
    OpStEndDataSafe,
    /* ���������Ƿ�Ϸ� */
    OpStCheckValid,
    /* �����Ū������Щ */
    OpStEnd = -1,
};

/*
 * �Զ����������״̬
 * (����1-100�� ��ҪŪ�� 0 ֮�������ֵ)
 */
enum EnLoginOpStatus
{
    OpStLoginDo = 1,
};

enum EnNetWorkMonitorStatus
{
    /* ��Ƶ�л� */
    OpStChanSwtch = 1,
    /* �໭��Ԥ�� */
    OpStMMonitor,
};

enum EnCmdsRecDownloadStatus
{
    OpStRecordFile = 1,
};

enum EnOnAudioDataStatus
{
    /* ��Ƶ�л� */
    OpStOnAudioData = 1,
};

//
enum EnDefaultOperationOpStatus
{
    OpStDefaultOperationDo = 1,
};

//
enum EnSendData
{
    OpStSendBackData = 1,
};

enum EnInfoProcStatus
{
    OpStInfoProcDo = 1,
};

#ifdef WTN
#	define DEFAULT_PREVIEW_QUALITY 6
#else
#	define DEFAULT_PREVIEW_QUALITY 6
#endif
#define DEFAULT_PREVIEW_FRAME 4
#define DEFAULT_PREVIEW_SIZE 3

enum EnSendPacket
{
    OpStSendMonitorPacket = 1,
    OpStSendMonitorPacketIFrameOnly,
    OpStSendMonitorPacketAdapter,
    OpStSendRecordPacket,
    OpStSendAudioPacket,
    OpStSendSnapPacket,
    OpStSendMuticastPacket,
};
//
enum EnMonitorSendMode
{
    EnMonitorSendMode_Packet = OpStSendMonitorPacket,
    EnMonitorSendMode_IFrameOnly,
};

//
enum EnSysUpdateOpStatus
{
    OpStSysUpdate = 1,
};

enum EnUserManagementStatus
{
    OpStQueryAuth = 1,

    OpStAddGroup	,
    OpStDelGroup	,
    OpStModGroup	,
    OpStQueryGroup	,

    OpStAddUser		,
    OpStDelUser		,
    OpStModUser		,
    OpStQueryUser	,
    OpStModPwd		,
};

//
// ͸�����ڲ���״̬
//
enum EnShareComStatus
{
    OpStSetCom = 1		,
    OpStSendData2Com	,
    OpStSendBackComData	,
    OpStBookExtIO,
    OpStQueryExtIO,
    OpStSetExtIO,
};

enum EnNetKeyBoardStatus
{
    OpStParseData = 1,
};
//0x60�豸����
enum EnDeviceCtrl
{
    OpStReboot = 1,
    OpStShutdown,
    OpStHdd,
    OpStTestDevice=5,
    OpStSd =7,
    /* Begin:add by Nirenzhuo(10770) for task.NO.N/A 20071029 */
    OpSpringSnap=8,
    /* End:add by Nirenzhuo(10770) for task.NO.N/A */
    OpDelLog = 9,//��־���
    OpStRecall = 10,	//�ָ��豸��Ĭ�����á�2007-11-28
};

//
// �������ͱ�������
//
enum EnAlarmToCenter
{
    OpStAlarmToCenterConn = 1	,
    OpStAlarmToCenterSend	,
};
//��������
enum EnAlarmOpr
{
    OpStQueryAlarmInfo = 1,        //��ѯ
    OpStBookAlarmInfo     ,        //����
    OpStCanselAlarmInfo   ,        //ȡ��
};
//����ͼƬ����
enum EnAlarmOprPic
{
    OpStQueryAlarmPic = 1,        //��ѯ
    OpStBookAlarmPic     ,        //����
    OpStCanselAlarmPic   ,        //ȡ��
};

typedef int        TOpState;       //��������״̬

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

enum EnDVRCommand
{
    //����
    REQ_UPDATE_LOGIN= 0x10,			//�����ͻ��˵�¼
    REQ_CHAN_SWTCH	= 0x11,			//��Ƶ�л�
    REQ_CTRL_PANTZ	= 0x12,			//��̨����, Ԥ�õ�����, �ƹ����
    REQ_MODE_FOCUS	= 0x13,
    REQ_CTRL_FOCUS	= 0x14,
    REQ_CTRL_ZOOM	= 0x15,
    REQ_CTRL_IMAGE	= 0x16,
    REQ_CTRL_MONSW	= 0x17,			//������(�ָ�)�л�
    REQ_CTRL_IRIS	= 0x18,
    REQ_CTRL_PRESET	= 0x19,
    REQ_CTRL_ALARM	= 0x1a,
    REQ_CTRL_LAMP	= 0x1b,
    REQ_CTRL_MENU	= 0x1c,
    REQ_SEND_AUDIO	= 0x1d,			//Ҫ������Ƶ
    REQ_MODE_XCHGE	= 0x1e,			//���ô��䷽ʽ
    REQ_SEND_TXMOD	= 0x1f,			//���ʹ��䷽ʽ
    REQ_COMM_SEND	= 0x20,			//͸�����ڴ���
    REQ_CTRL_RESET	= 0x21,
    REQ_DISK_PARAM	= 0x22,			//Ҫ����Ӳ��״̬��Ϣ
    REQ_ALARM_INFO	= 0x23,			//���󱨾��������״̬
    REQ_SET_DTIME	= 0x24,			//DVRʱ������

    REQ_NET_KEYBOARD_CTRL = 0x98,   /*!< �µ�������̿��� */
    REQ_BOARD_CTRL = 0x99,			//���̿���

    REQ_USER_LOGIN	= 0xa0,
    REQ_STAT_QUERY	= 0xa1,			//a1 ����״̬��ѯ
    REQ_LOGS_PARAM	= 0xa2,			//������־��Ϣ
    REQ_CONF_PARAM	= 0xa3,			//����ϵͳ����
    REQ_INFO_SYSTM	= 0xa4,			//����ϵͳ��Ϣ
    REQ_RECD_QUERY	= 0xa5,			//��ѯ¼���ļ�
    REQ_USER_PARAM	= 0xa6,			//��ѯ�û���Ϣ
    REQ_FILE_ALARM	= 0xa7,			//��ѯ�����ļ�
    REQ_CHAN_TITLE	= 0xa8,			//�����ͺ���
    REQ_CTRL_SWTCH	= 0xa9,			//����ı����Ȩ

    REQ_AUDIO_DATA	= 0xc0,			//�Խ�����
    ACK_REQ_AUDIO_DATA	= 0x1d,			//�Խ�����

    REQ_CONF_UPDAT	= 0xc1,			//�������ø���
    REQ_RECD_PLAY	= 0xc2,			//¼���ļ��ط�
    REQ_PLAY_ALERT	= 0xc3,			//���ű����ļ�
    REQ_USER_UPDAT	= 0xc4,			//�����û�����
    REQ_CTRL_RECRD	= 0xc5,			//����¼�����
    REQ_CHAN_UPDAT	= 0xc6,			//����ͨ������
    REQ_BIOS_UPDAT	= 0xc7,			//ˢ��BIOS
    REQ_FONT_UPDAT	= 0xc8,			//�����ֿ����
    REQ_PLAY_STOP	= 0xc9,			//ֹͣ�����ļ�
    REQ_CTRL_PLAY	= 0xca,			//¼���ļ�ʱ�����
    REQ_RECD_DOWN	= 0xcb,			//¼���ļ�����
    REQ_PLAY_PAUSE	= 0xcc,			//¼��ط���ͣ
    REQ_PROT_TITLE	= 0xcd,			//����Э�����

    REQ_CHAN_COLOR	= 0xcf,			//����ͨ����ɫ

    REQ_MODIFY_MAC = 0xd0,			//�����޸�Mac��ַ
    ACK_MODIFY_MAC = 0xd0,			//Ӧ���޸�Mac��ַ

    REQ_DEVICE_CTRL    = 0x60,              //�����豸����,added,2005/11/2,wangk
    ACK_DEVICE_CTRL    = 0x60,              //Ӧ���豸��������,added,2005/11/2,wangk
    ACK_SEND_MESSAGE   = 0x66,              //����¼����Ϣ,added,2006/04/12,wangk

    REQ_TEST_DEVICE	= 0x62,			//�����豸, ��9�ֽ�Ϊ��������, ��EnTestDeviceType
    REQ_INFO_PROC   = 0x61,			//��Ϣ����, ������POS������Ʒ��Ϣ���ӵ���Ƶ��
    ACK_INFO_PROC   = 0x61,			//��Ϣ����, ������POS������Ʒ��Ϣ���ӵ���Ƶ��

    REQ_DEVICE_QUERY= 0x82,         //�豸��Ѳ
    ACK_DEVICE_QUERY= 0x82,         //�����豸״̬��ͨ��״̬

    REQ_USER_FORCEIFRAME = 0x80,    //ǿ��I-FRAME�Ĺ���
    ACK_USER_FORCEIFRAME = 0x80,
    REQ_USER_SETNETRATE  = 0x81,    //������������
    ACK_USER_SETNETRATE  = 0x81,

    ACK_INFO_NORMAL      = 0x64,    //��ͨӦ����Ϣ
    REQ_SYSTEM_OPTQUERY  = 0x83,    //��ѯͨ����ѡ������
    ACK_SYSTEM_OPTQUERY  = 0x83,    //Ӧ��ͨ����ѡ������

    REQ_ALARM_STAT  = 0x68,         //������Ϣ����
    ACK_ALARM_STAT  = 0x69,         //������ϢӦ��

    //Ӧ��
    ACK_USER_LOGIN	= 0xb0,			//Ӧ���û���½
    ACK_STAT_QUERY	= 0xb1,			//Ӧ��״̬��ѯ
    ACK_LOGS_PARAM	= 0xb2,
    ACK_CONF_PARAM	= 0xb3,			//Ӧ��ϵͳ����
    ACK_INFO_SYSTM	= 0xb4,			//Ӧ��ϵͳ��Ϣ
    ACK_USER_PARAM	= 0xb5,			//Ӧ���û�����
    ACK_FILE_RECRD	= 0xb6,
    ACK_FILE_ALARM	= 0xb7,			//Ӧ�𱨾��ļ�
    ACK_CHAN_TITLE	= 0xb8,			//Ӧ��ͨ������
    ACK_FONT_UPDAT	= 0xb9,			//Ӧ���ֿ����
    ACK_BIOS_UPDAT	= 0xba,
    ACK_RECD_PLAY	= 0xbb,			//�������غͻطŽ���������
    ACK_CHAN_MONIT	= 0xbc,			//Ӧ��ͨ������
    ACK_SOCK_TEST	= 0xbd,			//���ڲ���
    ACK_CTRL_SWTCH	= 0xbe,			//���Ŀ���Ȩ
    ACK_CTRL_ROBED	= 0xbf,			//ǿ�л�ÿ���Ȩ
    ACK_SEND_AUDIO	= 0xc0,			//Ӧ������Ƶ
    ACK_CHAN_COLOR	= 0xCF,			//����ͨ����ɫ
    ACK_SEND_TXMOD	= 0xe0,			//���ͷ�ʽ
    ACK_DISK_PARAM	= 0xe1,			//Ӧ����Ӳ��״̬��Ϣ
    ACK_FILE_PARAM	= 0xe2,			//�����ļ���Ϣ
    ACK_STAT_ALARM	= 0xe3,			//��������״̬
    ACK_PROT_TITLE	= 0xe4,			//Ӧ��Э�����

    ACK_USER_UPDAT	= 0xe5,

    ACK_ERROR_RET	= 0xee,			// Ӧ��һ�������Ϣ

    ACK_USER_STATE	= 0xf0,			//Ӧ���û�״̬��ѯ

    REQ_SUB_CONN      = 0Xf1,                //����������
    ACK_CHAN_MONIT_EX = 0xf2,//�໭��Ԥ���������ڲ�����
    REQ_EXT_LOGIN      = 0Xf6,                //
    ACK_EXT_LOGIN      = 0Xf6,                //
    REQ_TEST_ONLY	= 0xfe,			//������
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    /*-+-+-+-+-+-+-+-+         ģ��DVR��         +-+-+-+-+-+-+-+-+-+-+-+-*/
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    SIMULATE_FILE_DOWNLOAD,			// �ļ����أ�ģ��DVRר��

    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    /*-+-+-+-+-+-+-+-+         �ڲ�����         -+-+-+-+-+-+-+-+-+-+-+-+-*/
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
    INTERNAL_DEFAULT_OPERATION	= 0xff01,	// Ĭ�ϲ�����
    INTERNAL_RECORD_DOWNLOAD			,	// ��¼�������أ��ط�&�������أ�
    INTERNAL_CONNECTION_BROKEN			,	// ���ӶϿ�����
    INTERNAL_SYSTEM_END_TASK			,	//�߳̽���������

    INTERNAL_ALARM_TO_CENTER_SDK		,	//�����ϴ�������Ϣ��sdk
    /*-+-+-+-+-+-+-+-   ��������Ϊ������������   +-+-+-+-+-+-+-+-+-+-+-+-*/
    /* ���治������������˳��Ҳ���ܱ�.
     *		ԭ��μ�: CNetWorkService::OnCommand
     */
    INTERNAL_SEND_PACKET		= 0xffa3,	// ����Packet����(��Ƶ)
    INTERNAL_SEND_DATA					,	// ��������
#ifdef XJSX_MODULE
    REQ_REGISTER_XJSX = 0x90,			//ע������
    ACK_REGISTER_XJSX = 0x91,			//ע����Ӧ
    REQ_KEEP_ALIVE_XJSX =0x92,			//��������
    ACK_KEEP_ALIVE_XJSX = 0x93,			//������Ӧ
#endif
    /*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
};

enum EnFrontKey
{
    NKEY_POWER = 0,
    NKEY_ENTER,
    NKEY_ESC,
    NKEY_UP,
    NKEY_DOWN,
    NKEY_LEFT,
    NKEY_RIGHT,
    NKEY_0,
    NKEY_1,
    NKEY_2,
    NKEY_3,
    NKEY_4,
    NKEY_5,
    NKEY_6,
    NKEY_7,
    NKEY_8,
    NKEY_9,
    NKEY_10,
    NKEY_11,
    NKEY_12,
    NKEY_13,
    NKEY_14,
    NKEY_15,
    NKEY_16,
    NKEY_SPLT,
    NKEY_DIV1,
    NKEY_DIV9,
    NKEY_ADDR,
    NKEY_INFO,
    NKEY_REC,
    NKEY_FN1,
    NKEY_FN2,
    NKEY_PLAY,
    NKEY_STOP,
    NKEY_SLOW,
    NKEY_FAST,
    NKEY_PREV,
    NKEY_NEXT,
    NKEY_JMPUP,
    NKEY_JMPDN,
    NKEY_10PLUS,
    NKEY_SHIFT,
    NKEY_BACK,
};

//
// ��ѯϵͳ������Ϣ
// ע��˳���ܸı�
//
typedef enum __cfg_index_t {
    CFG_GENERAL =  0,	// ��ͨ
    CFG_COMM   		,	// ����
    CFG_NET    		,	// ����
    CFG_RECORD 		,	// ¼��		/*!< 2.42�Ժ�������ã�ʹ��CFG_NEWRECORD */
    CFG_CAPTURE		,	// ͼ������	/*!< 2.42�Ժ�������ã�ʹ��CFG_CURRENTCAPTURE */
    CFG_PTZ    		,	// ��̨
    CFG_DETECT 		,	// ��̬���	/*!< 2.42�Ժ�������ã�ʹ��CFG_EVENT */
    CFG_ALARM  		,	// ����		/*!< 2.42�Ժ�������ã�ʹ��CFG_EVENT */
    CFG_DISPLAY		,	// ��ʾ
    CFG_TITLE  		= 10,	   // ͨ������
    CFG_MAIL  		= 11,	   // �ʼ�����
    CFG_EXCAPTURE	= 12,	   // Ԥ��ͼ������
    CFG_PPPOE	    = 13,	   // pppoe����
    CFG_NET_ARP	    = 14,	   // ARP����
    CFG_SNIFFER     = 15,	   // ������Ӳ�������
    CFG_DSPINFO     = 16,      // ����������Ϣ
    CFG_VIDEOCOVER = 34,	//�����ڵ�
    CFG_FTP         = 37,//17,       //FTP����
    CFG_AUTOMAINTAIN= 18,      //�Զ�ά��
    CFG_NET_NTP=19, /*ntp����-- ok 2010-12-6 11:51:38*/
    CFG_DSPBITRATE   = 24,    // DSP�������
    CFG_DHCP  = 35,
    CFG_WEB          = 36,
    CFG_WLANIP	= 39,	//����ģ��IP����
    CFG_DNS = 40,
    CFG_STORAGE = 41,//�洢λ��
#if defined(CAM_IPC) || defined(CAM_A6)
    CFG_CAM_CONTROL	 = 38,
#endif
    CFG_RECDDOWNLOAD = 42,
    CFG_AUDIOFORMAT	 = 43,		//��Ƶ�����ʽ
#ifdef DVR_PTZ
	CFG_PTZFUNC =44, 	//borui ptz functions
	CFG_PTZFPRESET = 45,
	CFG_PTZFTOUR =46,
	CFG_PTZFPATTERN =47,
#endif
    CFG_NEWRECORD	= 123,		/*!< �µ�¼������ */
    CFG_EVENT		= 124,	   //�¼�����
    CFG_WORKSHEET   = 125,     //ʱ���������Ϣ
    CFG_COLOR       = 126,     // ��ɫ������Ϣ
    CFG_CURRENTCAPTURE = 127,  // ��ǰ��������,Ϊ����˫�������ܺ����ӵ�
    CFG_NEWCAPTURE     = 128,  // �����µ�ͼ�����ýṹ
    CFG_WLAN	= 131,	//wifi config
    CFG_TRANSFER_POLICY	=	133,//���紫���������
    CFG_WI_ALARM  = 134,  //��ѯ��������
    CFG_WLANSCAN = 135, //wifi scanning
    CFG_NEWDDNS = 140,		//�µ�ddns
	CFG_MOUSE = 141,//���ʹ��ϰ������
    CFG_VSP            = 190,   //��������������
    CFG_CAMIPC         = 200,   //CAMIPC����
    CFG_LOCATION	   = 201,	/*!< �������� */
    CFG_GUISET		   = 202,	/*!< GUI���� */
    CFG_RECWORKSHEET   = 203, 	/*!< ¼���������� */
    CFG_PTZALARM	   = 204,	/*!< ��̨�����豸Э������ */
    CFG_NETALARM	   = 205,	/*!< ���籨�� */
    CFG_ALMWORKSHEET   = 206,	/*!< �������������� */
    CFG_MTDWORKSHEET   = 207,	/*!< ���칤�������� */
    CFG_PRESETNAME	   = 208,	/*!< ��̨Ԥ֪���� */
    CFG_PTZTOUR		   = 209,	/*!< Ѳ������ */
    CFG_LOSS		   = 210,	/*!< ��Ƶ��ʧ */
    CFG_BLIND		   = 211,   /*!< ��Ƶ�ڵ� */
    CFG_PLAY		   = 212,	/*!< �ط����� */
    CFG_USER		   = 213,	/*!< �û����� */
    CFG_GROUP		   = 214,	/*!< �û������� */
    CFG_MONITORTOUR	   = 215,	/*!< ������ѵ */
    CFG_TVADJUST	   = 216,	/*!< TV���� */
    CFG_ATM	   		   = 217,	/*!< ATM���� */
    CFG_STNOTEXIST	   = 218,	/*!< ��Ӳ�� */
    CFG_STFAILURE	   = 219,	/*!< Ӳ�̳��� */
    CFG_STLOWSPACE	   = 220,	/*!< Ӳ�̿ռ䲻�� */
    CFG_NETABORT	   = 221,	/*!< �����¼����� */
    CFG_VIDEOWIDGET	   = 222,	/*!< ��Ƶװ�� */
    CFG_VIDEOMATRIX	   = 223,	/*!< ��Ƶ���� */
#ifdef LOG_SUPPORT_CDJF
    CFG_SYSTEMTIME = 224,	//ϵͳʱ��
#endif//LOG_SUPPORT_CDJF
#ifdef CAM_A6
    CFG_INFRAREDALARMWORKSHEET = 225,//���߱���������
#endif
    CFG_NET_COMMON = 230, /*ͨ����������*/
    CFG_NET_FTPSERVER , /*FTP����������*/
    CFG_NET_FTPAPPLICATION , /*ftpӦ������*/
    CFG_NET_IPFILTER , /*IP��������*/
    CFG_NET_MULTICAST , /*�鲥����*/
    CFG_NET_PPPOE , /*pppoe����*/
#ifdef FUNC_DDNS_DYNDNS
    CFG_NET_DDNS , /*ddns����*/
#endif
    CFG_NET_ALARMSERVER , /*������������*/
    CFG_NET_VLAN = 238,
    CFG_NET_EMAIL , /*email����*/
    CFG_NET_SNIFFER , /*ץ������*/
#ifdef CDJF
    CFG_NET_CDJF,/*�ɶ��ѷ�������*/
    CFG_ALARM_CDJF,/*�ɶ��ѷ��ı���ͨ������*/
#endif

#if defined(TRANS_FILE_CFG)
    CFG_TRANS_FILE,
#endif

#if defined(XINGWANG_CFG)
    CFG_NET_XINGWANG,
#endif
#if defined(NET_MEYE_CFG)
    CFG_NET_MEYE,
#endif

    CFG_NET_DHCP_INT,

#if defined(COM_SUPPORT_DAGUANGMING)
    CFG_DAGUANGMING_LASTUPFILE ,
#endif

#if defined(_FUNC_UPNP_MOUDLE)
    CFG_NET_UPNP,
#endif

    CFG_RECState = 245, /*��������¼��״̬*/
    /* Begin:Added by ChenChengBin 10782, 2008/2/21 PN: ShangHai Bell Alcatel */
#ifdef SHBELL
    CFG_NET_SHBELL,/* �Ϻ�BELL���ŵ�������Ϣ */
    CFG_NET_SHBELL_PRESET,/* �Ϻ�BELL���ŵ�Ԥ�õ�������Ϣ */
    CFG_NET_SHBELL_TOUR,/* �Ϻ�BELL���ŵ�Ѳ��������Ϣ */
    CFG_CONFIG_PTZREGRESS, /* Ԥ�õ�ع���Ϣ */
    /* Begin:Added by ChenChengBin, 2008/10/28 PN: BELL_PICSHOOT */
#ifdef __BELL_PICSHOOT__
    CFG_CONFIG_UPLOAD_PICTURE, /* ����ͼƬ�ϴ�������Ϣ */
#endif
    /* End:Added by ChenChengBin, 2008/10/28 PN: BELL_PICSHOOT */
#endif
    /* End:Added by ChenChengBin 10782, 2008/2/21 PN: ShangHai Bell Alcatel */

    /* Begin:Added by ChenChengBin, 2008/7/28 PN: HZHS */
#ifdef HZHS
    CFG_NET_HZHS, /* ���ݻ�������������Ϣ */
    CFG_MOTIONZONE_HZHS,
#endif
    /* End:Added by ChenChengBin, 2008/7/28 PN: HZHS */

#ifdef FUNC_IVIEWER_GLOBAL
    CFG_IVIEWER_GLOBAL,
#if defined(SZHW_TR069)
    CFG_NSS_TR069,
#endif
#endif
#ifdef SUPPORT_3G_MODULE 
	CFG_3G_MODULE,
#endif
#ifdef OEM_HANGZHENG
	CFG_HANGZHENG_PLATE,
#endif

#ifdef OEM_BORUI_A3
	CFG_BORUI_A3_GSM,//���A3����,GSM������Ϣ
#endif

#if defined(MEGAEYES_MODULE)
    CFG_NET_MEGA,
    CFG_MEGA_FTP,
    CFG_MEGA_MOTION_ZONE,
    CFG_NET_MEGAEYES_PRESET,/* ���Ż�ͨ��Ԥ�õ�������Ϣ */
    CFG_NET_MEGAEYES_TOUR,/* ���Ż�ͨ��Ѳ��������Ϣ */
#endif

#if defined(SKD_MODULE) || defined(KEDA_MODULE)
    CFG_NET_KEDA , /*����������keda����*/
    CFG_NET_KEDA_MARTIX,
    CFG_NET_KEDA_MOTION_KEDA,
    CFG_NET_KEDA_TIMELYREC_INFO,
#endif

#ifdef ZTE_NETVIEW
    CFG_NET_NETVIEW,/*������ά���������*/
    CFG_OSD_NETVIEW,/*������ά����OSD������*/
    CFG_NET_NETVIEW_PRESET,/*������ά��Ԥ�õ�������Ϣ */
    CFG_NET_NETVIEW_TOUR,/* ������ά��Ѳ��������Ϣ */
#if defined(_NETVIEW_EXTEND_)
    CFG_ELSE_NETVIEW,
#endif
	#if defined(CNC_SJZ)
    CFG_PPWORKSHEET,
#endif
#endif
#if defined(H3C)
    CFG_NET_H3C , /*����������keda����*/
#endif
#if defined(LTM)
    CFG_NET_LTM , /*����������LTM����*/
#endif
#if defined(_ZTE_APP_)
    CFG_NET_ZTENJ,		/*������������������������*/
    CFG_NET_ZTENJ_TR069,
    CFG_ZTE_ALARMIN,
    CFG_ZTE_ALARMOUT,
    CFG_ZTE_CHANNEL,
    CFG_BLDWORKSHEET,
#if defined(_WANG_TEST_)
    CFG_ELSE_ZTENJ,
#endif
#endif

#ifdef _USE_ZJGX_MODULE
    CFG_NET_ZJGX,//�㽭����
#endif
#ifdef XTREAM
    CFG_NET_BJXF,
    CFG_NET_BJXF_PRESET,
    CFG_NET_BJXF_TOUR,
#endif
#ifdef _XWZX_MODULE
    CFG_NET_XWZX,
    CFG_GENERAL_XWZX,
#endif
#ifdef _ZJGX300_
    CFG_NET_ZJGX300,
    CFG_NET_ZJGX300_PRESET,/*Ԥ�õ�������Ϣ */
    CFG_NET_ZJGX300_TOUR,/* Ѳ��������Ϣ */
#endif
#ifdef XJSX_MODULE
    CFG_NET_XJSX,
#endif

#ifdef _BeijingSX_MODULE
    CFG_NET_BJSX,
    CFG_NET_BJSX_SYSTEM,
#endif

#if defined(_USE_WJJH_MODULE)
    CFG_NET_WJJH,
    CFG_SNMP_WJJH,
    CFG_NET_WJJH_PRESET,/*Ԥ�õ�������Ϣ */
    CFG_NET_WJJH_TOUR,/* Ѳ��������Ϣ */
#endif
    CFG_REG_SERVER = 241,/*����ע�����������*/
    CFG_INFO_VERSION =242,//�����ͺ���Ϣ����    addby guoq 2011.2.24
    CFG_BRA8  = 243,//����A8 ����������
    CFG_ANVISI  = 244,//��ά˼����������

}CFG_INDEX;
typedef enum _wiAlarm_type_
{
    WIALARM_OUT = 1,
    WIALARM_REMOTE =2
};

//Begin yuxy_070418,��ʱ����
typedef enum _event_type_
{
    EVENT_ALARM = 1,	//���ر����¼�
    EVENT_NETALARM,		//���籨���¼�
    EVENT_DECALARM,		//�����������¼�
    EVENT_MOTION,		//��̬����¼�
    EVENT_BLIND,		//�ڵ�����¼�
    EVENT_LOSS,			//��Ƶ��ʧ�¼�
    EVENT_NOHDD,		//��Ӳ���¼�
    EVENT_ERRHDD,		//Ӳ�̳����¼�
    EVENT_NOSPACEHDD,	//Ӳ�̿ռ䲻��
    EVENT_NETABORT, 	//�����¼�
    EVENT_CODER_ALARM,	//�������¼�
    EVENT_INFRARED_ALARM,//���ⱨ���¼�
    EVNET_ALL,			//ȷ���������
};
//End yuxy_070418

//����������
typedef enum _worksheet_type_
{
    WORKSHEET_TYPE_RECORD = 1,	//��ͨ¼��
    WORKSHEET_TYPE_ALARM,		//���ر���
    WORKSHEET_TYPE_NETALARM,	//���籨��
    WORKSHEET_TYPE_MOTION,		//��̬���
    WORKSHEET_TYPE_BLIND,		//�ڵ����
    WORKSHEET_TYPE_LOSS,		//��Ƶ��ʧ
    WORKSHEET_TYPE_NOHDD,		//��Ӳ��
    WORKSHEET_TYPE_ERRHDD,		//Ӳ�̳���
    WORKSHEET_TYPE_NOSPACEHDD,	//Ӳ���޿ռ�
    WORKSHEET_TYPE_NETABORT,	//����
    WORKSHEET_TYPE_DECALARM,	//����������
    WORKSHEET_TYPE_CODINGALARM,  //����������
    WORKSHEET_TYPE_InfraredAlarm, //���߱���
    WORKSHEET_TYPE_ALL,			//һ�㹤������������֮ǰ���
};

//!��������
typedef enum _alarm_type_t
{
    ALARM_IN     = 1,    //�ⲿ����
    ALARM_MOTION    ,    //��̬��ⱨ��
    ALARM_LOSS      ,    //��Ƶ��ʧ����
    ALARM_BLIND     ,    //��Ƶ�ڵ�
    ALARM_AUDIO     ,    //��Ƶ����
    ALARM_DISKLOW   ,    //Ӳ�̵���������
    ALARM_DISKERROR ,    //Ӳ�̴��󱨾�
    ALARM_NODISK    ,    //��Ӳ�̱���
    ALARM_DECODER   ,    //����������
    ALARM_ENCODER,       //����������
    ALARM_EMERGENCY,	 //���������¼�
    ALARM_INFRARED = 12, //���ⱨ���¼�

#ifdef OEM_BORUI_A3
	ALARM_POWERCUT = 13,  //�ϵ�
	ALARM_BATTERYFAILURE = 14, //��ع���
	ALARM_GSMBAD = 15,//GSM����
	ALARM_GSMABORT = 16,//��������Ͽ�
	ALARM_GSMARP = 17,//�ڲ�ip��ͻ
#endif

    ALARM_ALL,    //һ�㱨�������Ҫ��ӣ���ǰ���

    //����������ı�������151��ʼ
    ALARM_COMM  = 151,  //���ڹ���
};

//!���ڱ���
typedef struct __com_alm_info
{
	U8 para[128];
}
COM_ALM_INFO;

//!ץͼ����
typedef struct _snap_param
{
	U32 Ver;            //ץͼ��ͨ��
	U32 SnapSecond;     //ʱ�� - ��
	U32 SnapMSecond;    //ʱ�� - ����
	U32 SnapQuality;    //����1~6
	U32 Reserved[4];
}
SNAP_PARAMS;

//!Ӧ�������Ƿ�ɹ�
typedef enum __cfg_app_ret_t {
    CFG_APP_FAILED = -100,		//Ӧ������ʧ��
    CFG_APP_SAVECFG_FAILED = -99,	//��������ʧ��

    CFG_APP_SUCCESSFUL = 0,		//Ӧ�����óɹ�

    CFG_NET_GATEWAY_FAILED,	//��������ʧ��,��Ȼ����Ϊ�ɹ�
    CFG_APP_NEED_RESTART,	//����Ӧ�ó�������Ӧ��
    CFG_APP_NEED_REBOOT,	//����ϵͳ�����Ӧ��
};

//!��ѯϵͳ��Ϣ
typedef enum __sys_info_t {
    SYSINFO_RECFILESYS =  0	,	// DVR¼���ļ�ϵͳ��Ϣ
    SYSINFO_SPLIT 		,	// �ָ�
    SYSINFO_CAPTURE   	,	// ����
    SYSINFO_EXCAPTURE   ,	// �໭�沶��
    SYSINFO_MOTIONDETECT,	// ��̬���
    SYSINFO_LOSSDETECT 	,	// ��Ƶ��ʧ
    SYSINFO_BINDDETECT 	,	// �������
}SYS_INFO;

//!��Ʒ���
enum product_t {
    /* �Ӻ� */
    DH_DVR_H	=		0,
    /* D8104 */
    DEVICE_TYPE_D8104	=		1,
    /* D8008 */
    DEVICE_TYPE_D8008	=		2,
    /* D8404 */
    DEVICE_TYPE_D8404	=		3,
    /* ATM���� */
    DH_DVR_ATM	=		4,
    /* ������Ƶ������ */
    DH_NVS		=		5,
    /* ��Ƶ������0404e */
    DH_NVS_E	=		6,
    /*Bϵ��NVS*/
    DH_NVS_B	=		10,
    /*Hϵ��NVS*/
    DH_NVS_C	=		11,

    DH_CAM_SDIP	=		50,
    DH_CAM_IPC	=		51,
    DH_HISI_IPC	=		55,

    /*A6����*/
    DH_DVR_A6   =       60,
};

enum EnLoginResult
{
    EnLoginResult_Succeeded = 0,	// login succeeded
    EnLoginResult_Failed = 1,		// login failed
    EnLoginResult_Logined = 3,		// login yet
};

//!�����豸
enum EnTestDeviceType
{
    EnTestDevice_Null = 0,
    EnTestDevice_Smtp = 1,			// ����Smtp����
};

#define MAX_FILE_NUM_ONCE		16		//�����ѯ��һ�����16���ļ�
#ifdef _USE_NET_MODULE
#define MAX_CODETYPE			CHL_FUNCTION_NUM
#define INVALID_CODEVALUE       CHL_FUNCTION_NUM+1
#else
#define MAX_CODETYPE			1
#define INVALID_CODEVALUE       2
#endif

//!ͨ������
#ifdef _USE_NET_MODULE
	#define TOTAL_CHANNEL_NUM N_SYS_CH
	#define TOTAL_AUDIO_IN_NUM N_AUDIO_IN
#else
	#define TOTAL_CHANNEL_NUM 16
	#define TOTAL_AUDIO_IN_NUM 1
#endif

extern int g_nAlarmIn;
extern int g_nAlarmOut;
extern int g_nCapture;
extern int g_nPlay;

//!ͨ�������л�
enum NetworkMonitorStatus
{
    CHANNEL_MONITOR_OFF=0,
    CHANNEL_MONITOR_ON=1,
    CHANNEL_MONITOR_NOCHANGE=2,
};

//!��Ƶ���ݻ�ȡ
enum GetAudioDataStatus
{
    CHANNEL_AUDIO_DATA_OFF=0,
    CHANNEL_AUDIO_DATA_ON=1,
};

enum EnReplayStatus
{
    OnReplay = 1,
    OnPause,
    OnStop,
    OnOthers,
};

enum EnVideoStandard
{
    VideoStandard_PAL = 0x01,
    VideoStandard_NTSC = 0x00,
};

enum EnCtrlErrorList		//����Ȩ��
{
    NO_CTRL_POWER = 0x10,	//�޿���Ȩ��
    NO_PRIVILEGE,			//Ȩ�޲���
    NO_RESOURCE,			//��Դ����
    USERNUMEXCEED,			//�û�������
    USERINEXISTENCE,		//�û��Ѵ���
    PERMITLAND,				//�����½
    OWNER,					//����Ȩӵ����
    BUSYNESS,				//�����æ
    INCLAIMING,				//��������
    SUCCESS,				//�ɹ�
    CLAIM,					//Ҫ�����Ȩ
    REFUSE,					//�ܾ�
    BEREFT,					//������
    INUDPWATCHING,			//���û���UDP����
    FREEBOOTING,			//ǿ�л�ÿ���Ȩ
    RELEASE,				//�ͷſ���Ȩ
    POWERSTATUS,			//Ȩ��״̬ 20
    TCPONLYONE,				//ÿһ·ֻ��һ���û�����
    MONITORBEREFT,			//��ͨ�����ӱ�����
};


//!��ͨ��Ϣ������ ��Ӧ���� 0x64
enum GeneralMsgRetCode
{
    GeneralMsgRetCode_Success = 0, // �޴�
    GeneralMsgRetCode_Busy = 1, // ϵͳæ
    GeneralMsgRetCode_NoAuth = 2, // ��Ȩ��
    GeneralMsgRetCode_ConnFull = 3, // ������
    GeneralMsgRetCode_IpNotAllowed = 4, // IP ��������
};

//!���õ���������
enum SetBitRate
{
    BitRate1 = 64,
    BitRate2 = 128,
    BitRate3 = 256,
    BitRate4 = 512,
    BitRate5 = 768,
    BitRate6 = 1024,
    BitRate7 = 65536,
};

#define CHECK_NETUSER_AUTH(X)				\
	if (false == GetOperator(m_pMsg->iDlgNo)->isValidAuthority((X)))	\
	{		\
		trace("!!! [%d] has not the auth(DVRG_AUTHORITY_ACCOUNT).\n",		\
		      GetOperator(m_pMsg->iDlgNo)->GetUID() );		\
		\
		SetState(OpStEnd, iThrdIdx);		\
		return OpStEnd;		\
	}

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ��Ӧ����: 0xa4
 */
//!��ѯϵͳ������Ϣ
typedef struct __sysattr_t
{
	unsigned char iVerMaj;          // BOIS���汾��(���)
	unsigned char iVerSub;          // BOIS�ΰ汾��(���)
	unsigned char iVideoInCaps;     // ��Ƶ����ӿ�����
	unsigned char iAudioInCaps;     // ��Ƶ����ӿ�����
	unsigned char iAlarmInCaps;     // ��������ӿ���
	unsigned char iAlarmOutCaps;    // ��������ӿ���
	unsigned char iIdePortNum;      // Ӳ�̽ӿ���
	unsigned char iAetherNetPortNum;// ����ӿ���
	unsigned char iUsbPortNum;      // USB�ӿ���
	unsigned char iComPortNum;      // ������
	unsigned char iParallelPortNum; // ���ڿ���
	unsigned char iReserved_NULL0;  // ���ֽ�, Ϊ�˶���
	unsigned short year;            // ��������: ��
	unsigned char  month;           // ��������: ��
	unsigned char  day;             // ��������: ��
	unsigned char iVerUser;         // BOIS�û��汾��(���)
	unsigned char iVerModify;       // BOIS�޸İ汾��(���)
	unsigned char iWebVersion[4];   // WEB�汾
	unsigned char WebYear;          // Ԥ��
	unsigned char WebMonth;         // Ԥ��
	unsigned char WebDay;           // Ԥ��
	unsigned char iDecodeChanNum;     // ���ؽ���(�ط�)·��
	unsigned char iSpeechInCaps;     // �Խ�����ӿ�����
	unsigned char iSpeechOutCaps;     // �Խ�����ӿ�����
	unsigned char iVGAPortNum;     // VGA�ӿ�����
	unsigned char iTVOutPortNum;     // TV����ӿ�����
	unsigned char iIsMutiEthIf;     // ��������֧�� 0:������ 1 ������
	unsigned char iReserved_07;     // Ԥ��
}
SYSATTR_T;

//!��ѯ��Ƶ������Ϣ
typedef struct __sysattr_video_caps
{
	unsigned int iCifCaps; 		// ��С
	unsigned int iTypeCaps;		// ����
	unsigned int iFrameCaps;	// ֡��
	unsigned int iQualityCaps;	// ����
	unsigned char iCifFrames;	// ÿ��оƬ������֡�� ��λ��cif/s
	unsigned char _iRev_00;		// ����
	unsigned char _iRev_01;		// ����
	unsigned char _iRev_02;		// ����
	unsigned int iReserved_01;	// ����
	unsigned int iReserved_02;	// ����
	unsigned int iReserved_03;	// ����
	unsigned char iStandard;	// ��ʽ
	unsigned char iRev_00;		// ����
	unsigned char iRev_01;		// ����
	unsigned char iRev_02;		// ����
}
SYSATTR_VIDEO_CAPS_T;
//!iFrameCaps��Ӧ����
typedef enum capture_frame_t
{
    cap_frame_01,
    cap_frame_02,
    cap_frame_03,
    cap_frame_06,
    cap_frame_12,
    cap_frame_20,
    cap_frame_25,
    cap_frame_30,
};

//! ��ѯ�����Խ�����
typedef struct __dialog_caps_t
{
	// ֧�ֵ���Ƶ���ݸ���������
	U8 iAudioTypeNum;		/*!< �Ƿ�֧�������Խ� */
	// ����λ
	U8 iRev[31];
}
DIALOG_CAPS;

typedef struct __cam_caps
{
	U8 	iBrightnessEn;	// ���� �ɵ� 1 �ɣ� 0����
	U8  iContrastEn;		// �Աȶ� �ɵ�
	U8  iColorEn;		// ɫ�� �ɵ�
	U8  iGainEn;			// ���� �ɵ�
	U8  iSaturationEn;	// ���Ͷ� �ɵ�
	U8  iBacklightEn;	// ���ⲹ�� �ɵ�
	U8  iExposureEn;		// �ع�ѡ�� �ɵ�
	U8  iColorConvEn;	// �Զ��ʺ�ת�� �ɵ�
	U8  iAttrEn;			// ����ѡ��  1 �ɣ� 0����
	U8  iMirrorEn;	     // ����  1 ֧�֣� 0��֧��
	U8  iFlipEn;	          // ��ת   1 ֧�֣� 0��֧��
	U8  iRev[125];		// ����
}
CAM_CAPS;

/// ����ͷ���ƽṹ
typedef struct cam_control
{
	U8  Exposure; ///< �ع�ģʽ 1-6:�ֶ��ع�ȼ�; 0:�Զ��ع�
	U8  Backlight; ///< ���ⲹ�� true:�򿪲���; false:ֹͣ����
	U8  AutoColor2BW; ///< �Զ��ʺ�ת�� true:��ת��; false:ֹͣת��
	U8  Mirror;	     //< ����  1 ���� 0��
	U8  Flip;	          //< ��ת  1 ���� 0��
	U8    iRev[123];
}
CAM_CONTROL;

typedef struct __audio_attr_t
{
	// ֧�ֵı�������
	U16 iAudioType;			// 1:PCM, 2:G711a
	// λ
	U16 iAudioBit;			// ��ʵ�ʵ�ֵ��ʾ�� ��8λ ����ֵΪ8
	// ֧�ֵĲ�����
	U32 dwSampleRate;			// ��ʵ�ʵ�ֵ��ʾ�� ��8k ����ֵΪ8000
	// ����λ
	U8	iRev[64];
}
AUDIO_ATTR_T;

typedef enum audio_forat_t
{
    AUDIO_FORMAT_PCM8 = 1,
    AUDIO_FORMAT_G711a = 2,
    AUDIO_FORMAT_AMR = 3,
    AUDIO_FORMAT_G711u = 4,
    AUDIO_FORMAT_G726 = 5,
    AUDIO_FORMAT_IMAADPCM = 6
};

//!iQualityCaps��Ӧ����
typedef enum capture_quality_t
{
    cap_quality_00,
    cap_quality_01,
    cap_quality_02,
    cap_quality_03,
    cap_quality_04,
    cap_quality_05,
};

//!��ѯ�ַ���������Ϣ
typedef struct __sys_string_support
{
	U8  Version;		// �����õİ汾��Ϣ
	U32 Type;       	// ֧�ֵ����ͣ� �����ʾ�� �ӵ͵�������ʾ���Զ���|UTF-8
	U8  iReserved_00;	// Ԥ��
	U8  iReserved_01;	// Ԥ��
	U8  iReserved_02;	// Ԥ��
	U8  iReserved_03;	// Ԥ��
	U8  iReserved_04;	// Ԥ��
	U8  iReserved_05;	// Ԥ��
	U8  iReserved_06;	// Ԥ��
	U8  iReserved_07;	// Ԥ��
}
SYS_STRING_SUPPORT;

enum EnEncodingType
{
    ENCODING_UTF8 = 0x00000001,
};

/* ��½���� */
typedef struct __login_attribute
{
	/* �Ƿ�����½�豸����ʹ�ã� 0��Ҫ��½��1�����ص�½ */
	U8	iShouldLogin;
	/* ֧�ֵĵ�½�û�����󳤶� */
	U8	iUNameMaxLen;
	/* ֧�ֵ�������󳤶� */
	U8	iUPwdMaxLen;
	/* ֧�ֵļ��ܷ�ʽ */
	U8	iEncryptType;/* �ӵ͵��ߣ���λ��ʾ���ܷ�ʽ��0-3des, */
	U8  iReserved[28];/* ���� */
}
LOGIN_ATTRIBUTE;


#if defined(FUNC_GDYT_ATM) || defined(FUNC_HZDS_ATM) || defined (FUNC_TJYL_ATM)
enum EnATMTradeType
{
    ATM_TRADE_TYPE_QUERY = 0x0101,
    ATM_TRADE_TYPE_WITHDRAW = 0x0102,
    ATM_TRADE_TYPE_MODPWD = 0x0103,
    ATM_TRADE_TYPE_TRANSFER = 0x0104,
    ATM_TRADE_TYPE_DEPOSIT = 0x0105,
};
#endif

//! Ԥ��ͼ������
typedef struct __config_ex_capture_t
{
	U8		 iQuality;		// ����
	U8		 iReserved[31];	// ����
}
CONFIG_EX_CAPTURE_T;

//!��̨��������, REQ_PROT_TITLE, ��9�ֽ�Ϊ0
typedef struct _net_ptz_name
{
	char		ProtocolName[12];	// 12�ֽڵ�Э������
	U32			BaudRate; 			// 4�ֽڵĲ�����
	U8			DataBits; 			// 1�ֽڵ�����λ
	U8			StopBits; 			// 1�ֽڵ�ֹͣλ
	U8			Parity; 			// 1�ֽڵ���żУ��
	U8			Resove;				// 1�ֽڱ���
}
NET_PTZ_NAME;

//!������������, REQ_PROT_TITLE, ��9�ֽ�Ϊ1
typedef struct _net_comm_name
{
	char		ProtocolName[12];	// 12�ֽڵ�Э������
	U32			BaudRate; 			// 4�ֽڵĲ�����
	U8			DataBits; 			// 1�ֽڵ�����λ
	U8			StopBits; 			// 1�ֽڵ�ֹͣλ
	U8			Parity; 			// 1�ֽڵ���żУ��
	U8			Resove;				// 1�ֽڱ���
}
NET_COMM_NAME;

//!�豸��ѯ��Ϣ
typedef struct _device_query_info
{
	U8          Devicestat;
	U8          InAlarmCaps;
	U8          OutAlarmCaps;
	U8          iReserv;
}
DEVICE_QUERY_INFO;

//!ͨ����Ѳ��Ϣ
typedef struct _channel_state
{
	U8 ChannleLoss[32];
	U8 DspState[32];
}
CHANNEL_STATE;

//! �ڲ�ʹ�ã�
// for : INTERNAL_ALARM_TO_CENTER_SDK
typedef struct __config_alarm_info_t
{
	// ��������
	U32	 iAlarmType;
	// ��������
	int	iAlarmMask;
}
CONFIG_ALARM_INFO_T;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/**
* ֡��λ��Ϣ
*/

#define FRAME_LOCATION_INFO_NUM 50
#define FRAME_TYPE_I	1
#define FRAME_TYPE_P	0
#define FRAME_TYPE_B	2
#define FRAME_TYPE_AUDIO	3
#define FRAME_TYPE_OTHERS	4
#define FRAME_TYPE_NULL	0xFF

//! ֡��λ��Ϣ
typedef struct __frame_location_info_t
{
	U8 iFrameType;				/*!< ֡���� */
	U8 iFrameFlag;				/*!< ֡ͷβ�ı�ʶ */
	U16 iFramePos;				/*!< ֡����ʼλ�� */
	U32 iFrameLength;			/*!< ֡�ĳ��ȣ����ܿ�� */
	U16 iDataLength;				/*!< ֡�ڱ����еĳ��� */
	U8 iReserve[2];				/*!< ���� */
}
FRAME_LOCATION_INFO_T;

//! ��ͷ����Ϣ����
typedef struct __packet_head_t
{
	FRAME_LOCATION_INFO_T fliFrameInfo[FRAME_LOCATION_INFO_NUM];		/*!< ����֡��Ϣ */
	U8			iPacketInfo;							/*!< �����Ϣ */
	U8			iReserve[7];							/*!< ���� */
}
PACKET_HEAD_T;

//
#if 0
enum EnMonitorSendMode
{
    // ���������Ч�����ȣ������ڿ������������ƵС��16·�����
    EnMonitorSendMode_WidthBandHighPerformance = 1,
    // ����������ӳ�С�������ڿ������·���ӣ�������Ϊ�豸���������������ӳ�
    EnMonitorSendMode_WidthBandRealtime,
    // ������ڲ��Զ��������ӳٺ�������ƽ��
    EnMonitorSendMode_WidthBandAuto,
    // խ��, �����෢������
    EnMonitorSendMode_NarrowBandHighPerformance,
    // խ��, �������ٻ����ӳ�
    EnMonitorSendMode_NarrowBandRealtime,
    // խ��, �ڲ��Զ��������ӳٺ�������ƽ��
    EnMonitorSendMode_NarrowBandAuto,
    // ֻ��I֡, һ�����ڵ���
    EnMonitorSendMode_IFrameOnly,
    // ֻ��I��P֡, һ�����ڵ���
    EnMonitorSendMode_IAndPFrame,
    // �Զ�����
    EnMonitorSendMode_Auto,
};
#endif
typedef struct __para_req_media_udpmode
{
	IPDEF 		tRemoteIp;		//
	LISTENPORT 	tRemotePort;	// 0-65535
	U8			iChannel;		// 0-n
	U8			iOpr;			// 0-Stop, 1-Start
	U8          iSteamType;
	U8			iReserved[31];	// reserved
}
PARA_REQ_MEDIA_UDPMODE;

#include "../Include/DVRDEF.H"

/// ����������֧�����Խṹ
typedef struct NET_CAPTURE_EXT_STREAM
{
	DWORD ExtraStream;						///< ��channel_t��λ����ʾ֧�ֵĹ��ܡ�
	DWORD CaptureSizeMask[64];	///< ÿһ��ֵ��ʾ��Ӧ�ֱ���֧�ֵĸ���������
}
NET_CAPTURE_EXT_STREAM;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifdef __cplusplus
extern "C"
{
#endif

	extern int *fps_jw;

	//! ���뷽ʽ
	int ExCaptureSetEncode(int iChannel, int iEncode);

	//! ��С
	int ExCaptureSetSize(int iChannel, int iSize);

	//! ����  	1-6 ��
	int ExCaptureSetQuality(int iChannel, int iQuality);

	//! ֡��  	1 2 3 6 12 25
	int ExCaptureSetFrame(int iChannel, int iFrame);

	/*! ��������IP
	 * \param pHostIp ����IP
	 * \param pNetmask ��������
	 */
	int NetSetHostIP(const char *pHostIp,
	                 const char *pNetmask);
	/*! ��������
	 * \param pGageway ����
	 */
	int NetSetGateway(const char *pGageway);
	/*! ȡ����IP
	 * \param pHostIp ����IP
	 * \param iHLen ����IP����
	 * \param pNetmask ��������
	 * \param iNLen �������볤��
	 */
	int NetGetHostIP(char *pHostIp,
	                 const int iHLen,
	                 char *pNetmask,
	                 const int iNLen);
	/*! ȡ���ص�ַ
	 * \param pGageway ����
	 * \param iGLen ���س���
	 */
	int NetGetGateway(char *pGageway,
	                  const int iGLen);

#ifdef __cplusplus
}
#endif

#endif // _CMDSTYPEDEF_H
