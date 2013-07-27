/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * TeleSerialDef.h - _explain_
 *
 * Copyright (C) 2012 ezlibs.com, All Rights Reserved.
 *
 * $Id: TeleSerialDef.h 0001 2012-4-11 9:32:40Z WuJunjie $
 *
 *  Explain:
 *     -Common define-
 *
 *  Update:
 *     2012-4-11 9:32:49 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _TeleSerialDef_H
#define _TeleSerialDef_H

//#define TeleSerial_FUNC_DEFAULT_NEXT_HOST "192.168.10.24"
#define TeleSerial_FUNC_DEFAULT_NEXT_HOST "192.168.10.157"
#define TeleSerial_FUNC_DEFAULT_NEXT_HOST_PORT 60000

#define TeleSerial_FUNC_DEFAULT_NEXT_HTTP_HOST "192.168.10.160"
#define TeleSerial_FUNC_DEFAULT_NEXT_HTTP_PORT 80

#define TeleSerial_FUNC_THE_TCPSVR_AGENT_PORT 60000

#define TeleSerial_FUNC_THE_HTTP_SVR_AGENT_PORT 60080

#define TeleSerial_FUNC_THE_UDPSVR_AGENT_PORT 60000

#define TeleSerial_FUNC_THE_STATUS_AGENT_PORT 19199

// ���Զ˿�
#define TeleSerial_FUNC_PORT_GENERAL_CONSOLE_OVER_TCP 19023

//// ����ʱ�޸�
//#define TeleSerial_FUNC_CONSOLE_OVERTCP          // #Զ�̵��Խӿ�
//#define TeleSerial_FUNC_STATUS_AGENT             // #tcp״̬������
//#define TeleSerial_FUNC_HTTPKEEPCONNECTOR   // #httpЭ��Ŀͻ���
//#define TeleSerial_FUNC_DATAPARSE_EXAMPLE               // #���ݽ���ʾ��
//#define TeleSerial_FUNC_TCPSOCKETSERVER     // #TCP�����
//#define TeleSerial_FUNC_TCPSOCKETCONNECTOR  // #tcp�ͻ���
//#define TeleSerial_FUNC_UDPSOCKETSERVER     // #UDP�����
//#define TeleSerial_FUNC_SOCKETPROCESSOR          // #ʹ�õ����Ĵ����߳�
//#define TeleSerial_FUNC_HTTPSERVER          // #HTTP�����
//#define TeleSerial_FUNC_SIGNAL_ONDATA                   // #ʹ�����ݻص�����
//#define TeleSerial_FUNC_WEBSERVICE                   // #webservice
//#define TeleSerial_FUNC_USE_LOG4CPP							// #

/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#endif // _TeleSerialDef_H
