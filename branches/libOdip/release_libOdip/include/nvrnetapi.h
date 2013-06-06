/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * nvrnetapi.h - _explain_
 *
 * Copyright (C) 2012 WuJunjie(Joy.Woo@hotmail.com), All Rights Reserved.
 *
 * $Id: nvrnetapi.h 5884 2012-11-23 02:37:21Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-11-23 02:37:21  Create 0.5
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _NVR_NET_API_H_
#define _NVR_NET_API_H_
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#if (defined(_WIN32)) //windows
	#define NET_DVR_API  extern "C"__declspec(dllimport)
#elif defined(__linux__) //linux
	#define NET_DVR_API extern "C"
	typedef     unsigned int    DWORD;
	typedef     unsigned short  WORD;
	typedef     unsigned short  USHORT;
	typedef     int            LONG;
	typedef  	unsigned char	BYTE ;
	typedef     int BOOL;
	typedef     unsigned int   	UINT;
	typedef 	void* 			LPVOID;
	typedef 	void* 			HANDLE;
	typedef     unsigned int*  LPDWORD; 
	typedef  unsigned long long UINT64;
	
	#ifndef    TRUE
	#define    TRUE	1
	#endif
	#ifndef    FALSE
	#define	   FALSE 0
	#endif
	#ifndef    NULL
	#define	   NULL 0
	#endif

#endif //  windows/linux ...

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
//�궨��
#define MAX_NAMELEN			    16		//DVR���ص�½��
#define MAX_RIGHT			    32		//�豸֧�ֵ�Ȩ�ޣ�1-12��ʾ����Ȩ�ޣ�13-32��ʾԶ��Ȩ�ޣ�
#define NAME_LEN			    32      //�û�������
#define PASSWD_LEN			    16      //���볤��
#define SERIALNO_LEN		    48      //���кų���
#define MACADDR_LEN			    6       //mac��ַ����
#define MAX_ETHERNET		    2       //�豸������̫����
#define MAX_NETWORK_CARD        4       //�豸�������������Ŀ
#define PATHNAME_LEN		    128     //·������
#define MAX_PRESET_V13          16      //Ԥ�õ�

#ifndef __trip
	#define __trip printf("-W-%d::%s(%d)\n", (int)time(NULL), __FILE__, __LINE__);
#endif
#ifndef __fline
	#define __fline printf("%s(%d)--", __FILE__, __LINE__);
#endif

//NET_DVR_Login_V30()�����ṹ
typedef struct
{
    BYTE sSerialNumber[SERIALNO_LEN];  //���к�
    BYTE byAlarmInPortNum;		        //�����������
    BYTE byAlarmOutPortNum;		        //�����������
    BYTE byDiskNum;				    //Ӳ�̸���
    BYTE byDVRType;				    //�豸����, 1:DVR 2:ATM DVR 3:DVS ......
    BYTE byChanNum;				    //ģ��ͨ������
    BYTE byStartChan;			        //��ʼͨ����,����DVS-1,DVR - 1
    BYTE byAudioChanNum;                //����ͨ����
    BYTE byIPChanNum;					//�������ͨ������  
	BYTE byZeroChanNum;			//��ͨ��������� //2010-01-16
	BYTE byMainProto;			//����������Э������ 0-private, 1-rtsp
	BYTE bySubProto;				//����������Э������0-private, 1-rtsp
    BYTE bySupport;        //������λ����Ϊ0��ʾ��֧�֣�1��ʾ֧�֣�
                            //bySupport & 0x1, ��ʾ�Ƿ�֧����������
                            //bySupport & 0x2, ��ʾ�Ƿ�֧�ֱ���
                            //bySupport & 0x4, ��ʾ�Ƿ�֧��ѹ������������ȡ
                            //bySupport & 0x8, ��ʾ�Ƿ�֧�ֶ�����
                            //bySupport & 0x10, ��ʾ֧��Զ��SADP
                            //bySupport & 0x20  ��ʾ֧��Raid������
                            //bySupport & 0x40  ��ʾ֧��IPSAN Ŀ¼����
    BYTE byRes1[20];					//����
}NET_DVR_DEVICEINFO_V30, *LPNET_DVR_DEVICEINFO_V30;
//�����Ԥ������
typedef struct
{
	LONG lChannel;//ͨ����
	LONG lLinkMode;//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ�ӣ�0��30λ��ʾ�������ӷ�ʽ: 0��TCP��ʽ,1��UDP��ʽ,2���ಥ��ʽ,3 - RTP��ʽ��4-����Ƶ�ֿ�(TCP)
	//HWND hPlayWnd;//���Ŵ��ڵľ��,ΪNULL��ʾ������ͼ��
	char* sMultiCastIP;//�ಥ���ַ
}NET_DVR_CLIENTINFO, *LPNET_DVR_CLIENTINFO;

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define __stdcall 
#define CALLBACK  
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

NET_DVR_API BOOL __stdcall NET_DVR_Init();
NET_DVR_API BOOL __stdcall NET_DVR_Cleanup();

NET_DVR_API BOOL __stdcall NET_DVR_SetConnectTime(DWORD dwWaitTime = 3000, DWORD dwTryTimes = 3);
NET_DVR_API BOOL __stdcall NET_DVR_SetReconnect(DWORD dwInterval = 30000, BOOL bEnableRecon = TRUE);

NET_DVR_API LONG __stdcall NET_DVR_Login_V30(char *sDVRIP, WORD wDVRPort, char *sUserName, char *sPassword, LPNET_DVR_DEVICEINFO_V30 lpDeviceInfo);
NET_DVR_API BOOL __stdcall NET_DVR_Logout_V30(LONG lUserID);

NET_DVR_API DWORD __stdcall NET_DVR_GetLastError();
NET_DVR_API char* __stdcall NET_DVR_GetErrorMsg(LONG *pErrorNo = NULL);


NET_DVR_API LONG __stdcall NET_DVR_RealPlay_V30(LONG lUserID, LPNET_DVR_CLIENTINFO lpClientInfo, void(CALLBACK *fRealDataCallBack_V30) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser) = NULL, void* pUser = NULL, BOOL bBlocked = FALSE);
NET_DVR_API BOOL __stdcall NET_DVR_SetRealDataCallBack(LONG lRealHandle,void(CALLBACK *fRealDataCallBack) (LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD dwBufSize,DWORD dwUser),DWORD dwUser);
NET_DVR_API BOOL __stdcall NET_DVR_StopRealPlay(LONG lRealHandle);

#endif //  _NVR_NET_API_H_
