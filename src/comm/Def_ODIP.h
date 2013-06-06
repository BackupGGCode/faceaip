/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * Def_ODIP.h - _explain_
 *
 * Copyright (C) 2012 WuJunjie(Joy.Woo@hotmail.com), All Rights Reserved.
 *
 * $Id: GeneralAgentDef.h 0001 2012-4-11 9:32:40Z WuJunjie $
 *
 *  Explain:
 *     -Common define-
 *
 *  Update:
 *     2012-4-11 9:32:49 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef _DEF_ODIP_H
#define _DEF_ODIP_H

//Э�������
#define INTERNAL_ODIP_DATA_PARSE 1

#ifdef WIN32
	#ifndef LITTLE_ENDIAN
		#define LITTLE_ENDIAN 1234
		#define BIG_ENDIAN 4321
		#define BYTE_ORDER LITTLE_ENDIAN 
	#endif
#else
	#include <endian.h>
#endif


//#define	DVRIP_VERSION	2
#define	DVRIP_VERSION	0 // single thread

typedef struct dvrip
{
	unsigned char	dvrip_cmd;		/* command  */
	unsigned char	dvrip_r0;		/* reserved */
	unsigned char	dvrip_r1;		/* reserved */
#if BYTE_ORDER == LITTLE_ENDIAN 
	unsigned char	dvrip_hl:4,		/* header length */
					dvrip_v :4;		/* version */
#endif
#if BYTE_ORDER == BIG_ENDIAN 
	unsigned char	dvrip_v :4,		/* version */
					dvrip_hl:4;		/* header length */
#endif
	unsigned int	dvrip_extlen;	/* ext data length */
	unsigned char	dvrip_p[24];	/* inter params */
}DVRIP;

typedef union
{
	struct dvrip	dvrip; /* struct def */
	unsigned char	c[32]; /* 1 byte def */
	unsigned short	s[16]; /* 2 byte def */
	unsigned int	l[8];  /* 4 byte def */
}ODIP_HEAD_T;
#define ODIP_HEAD_T_SIZE sizeof(ODIP_HEAD_T)
#define ZERO_ODIP_HEAD_T(X) memset((X), 0, ODIP_HEAD_T_SIZE);			\
							(X)->dvrip.dvrip_hl = ODIP_HEAD_T_SIZE/4;	\
							(X)->dvrip.dvrip_v = DVRIP_VERSION;




enum EnDVRCommand
{
    //����
    REQ_USER_LOGIN	= 0xa0,
    REQ_STAT_QUERY	= 0xa1,			//a1 ����״̬��ѯ
    REQ_CHAN_SWTCH	= 0x11,			//��Ƶ�л�

    //Ӧ��
    ACK_USER_LOGIN	= 0xb0,			//Ӧ���û���½
    ACK_STAT_QUERY	= 0xb1,			//Ӧ��״̬��ѯ
    ACK_CHAN_MONIT	= 0xbc,			//Ӧ��ͨ������

    REQ_CTRL_RECRD	= 0xc5,			//����¼�����
    REQ_INFO_SYSTM	= 0xa4,			//����ϵͳ��Ϣ
    ACK_INFO_SYSTM	= 0xb4,			//Ӧ��ϵͳ��Ϣ
};

typedef struct _snap_param_new
{
	int Channel;        //ץͼ��ͨ��
	int res[2];
	int mode;       //ץͼģʽ 0����ʾ����һ֡,1����ʾ��ʱ��������
	int res1;
	int CmdSerial;    //�������к�
	unsigned char  Opr;// 0: stop 1:start
	unsigned char  Res[3];
	int Reserved[3];
}snap_param_new;


/* �ط���32K */
#define MAX_DVRIP_PACKET_SIZE 32*1024*2

#endif // _DEF_ODIP_H
