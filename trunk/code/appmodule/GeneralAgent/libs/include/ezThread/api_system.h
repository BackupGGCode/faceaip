/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * api_system.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: api_system.h 5884 2012-05-16 09:16:13Z WuJunjie $
 *
 *  Explain:
 *     -ϵͳʱ�����ã��ػ���������ϵͳ��Դ״����������API-
 *
 *  Update:
 *     2012-05-16 09:09:51   Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __SYSTEMM_API_H__
#define __SYSTEMM_API_H__

#include "typedef_ezthread.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/// \defgroup SystemAPI API System
	/// ϵͳʱ�����ã��ػ���������ϵͳ��Դ״����������API��
	/// @{

	/// ϵͳʱ��ṹ
	typedef struct SYSTEM_TIME
	{
		int  year;///< �ꡣ
		int  month;///< �£�January = 1, February = 2, and so on.
		int  day;///< �ա�
		int  wday;///< ���ڣ�Sunday = 0, Monday = 1, and so on
		int  hour;///< ʱ��
		int  minute;///< �֡�
		int  second;///< �롣
		int  isdst;///< ����ʱ��ʶ��
	}
	SYSTEM_TIME;

	/// ϵͳ���Խṹ
	typedef struct SYSTEM_CAPS
	{
		/// �ܹ��ڴ��С����Kilo-BytesΪ��λ��
		unsigned int MemoryTotal;

		/// ʣ���ڴ��С����Kilo-BytesΪ��λ������ϵ��Ӧ�ó�����ڴ�ʹ�ò��ԡ�
		unsigned int MemoryLeft;
	}
	SYSTEM_CAPS;

	/// �����汾����
	/*
	typedef struct SYSTEM_CAPS_EX
	{
		unsigned int ForNRT;			///< �Ƿ�Ϊ��ʵʱ
		unsigned char ForATM;			///< �Ƿ�ΪATM��
		unsigned char HasAudioBoard;		///< ����Ƶ��
		unsigned char HasLoopBoard;		///< ����ͨ��
		unsigned char HasMatrixBoard;	///< �������
	}SYSTEM_CAPS_EX;
	*/
	typedef struct tagSYSTEM_CAPS_EX
	{
		unsigned int ForNRT;   //�Ƿ�Ϊ��ʵʱ, ��λ��ʾ����ͨ���Ƿ���Ϊ��ʵʱ.��0xffff��ʾǰ16·���Ƿ�ʵʱ.
		unsigned char ForATM;   //�Ƿ�ΪATM��, 1��ʾ����Һ��ATM,2��ʾ��Һ��ATM
		unsigned char HasAudioBoard;  //����Ƶ��
		unsigned char HasLoopBoard;  //����ͨ��
		unsigned char HasMatrixBoard; //�������
		unsigned char HasPtzBoard;           //����̨��
		unsigned char HasWlanBoard;        //�����߰�
		unsigned char reserved[2];
	}
	SYSTEM_CAPS_EX;

	/// ϵͳ����״̬
	enum system_upgrade_state
	{
	    SYSTEM_UPGRADE_OVER = 0,
	    SYSTEM_UPGRADE_INIT = 1,
	    SYSTEM_UPGRADE_RUNNING = 2
	};

	/* һ�η��仺������ */
	#define  PRE_MALLOC_IMG_BUF_SIZE  5*1024*1024

	/// �õ���ǰϵͳʱ�䡣����������ܵ��õĺ�Ƶ��, ���е���ʱӦ���Ǵ��ڴ���ȡ��
	/// ʱ��,������ֱ�Ӵ�Ӳ��, �ڶ�ʱ��������ڴ��е�ֵ.
	///
	/// \param [out] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemGetCurrentTime(SYSTEM_TIME * pTime);


	/// ���õ�ǰϵͳʱ�䡣
	///
	/// \param [in] pTime ָ��ϵͳʱ��ṹSYSTEM_TIME��ָ�롣
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemSetCurrentTime(SYSTEM_TIME * pTime);
	//int SystemGetTimeSecond(int *pTime);


	/// �õ�ϵͳ�ĺ�����������ε���֮��ļ��ʱ����ǵõ��ļ����Ĳ�ֵ��
	///
	/// \param ��
	/// \return ��ǰ������
	/// \note һ������£�49.7���õ������������ʹ�õ��ĵط�Ҫ���������
	unsigned int SystemGetMSCount(void);


	/// �õ�ϵͳ��΢�����, �˺�����Ҫ����ģ�����ܵĲ��ԡ�����ĸ��졣
	///
	/// \param ��
	/// \return ��ǰ������
	unsigned int SystemGetUSCount(void);


	/// �õ�CPUռ���ʡ�
	///
	/// \param ��
	/// \return CPUռ�ðٷֱȡ�
	int SystemGetCPUUsage(void);


	/// �õ�ϵͳ���ԡ�
	///
	/// \param [out] pCaps ָ��ϵͳ���ԽṹSYSTEM_CAPS��ָ�롣
	/// \retval 0  ��ȡ�ɹ���
	/// \retval <0  ��ȡʧ�ܡ�
	int SystemGetCaps(SYSTEM_CAPS * pCaps);



	///
	int SystemGetCapsEx(SYSTEM_CAPS_EX * pCaps);


	/// ����ǰ�̣߳�ʱ�䵽���ٻ��ѡ�
	///
	/// \param [in] dwMilliSeconds �����ʱ�䡣
	void SystemSleep(unsigned int dwMilliSeconds);


	///
	void SystemUSleep(unsigned int dwMicroSeconds);


	/// ��ϵͳ��������������������������ͬ��ģʽ��ʱ�䵽�ŷ��ء�
	///
	/// \param [in] dwFrequence ����Ƶ�ʡ�
	/// \param [in] dwDuration ������ʱ�䡣
	void SystemBeep(unsigned int dwFrequence, unsigned int dwDuration );


	/// ϵͳ��λ��
	void SystemReboot(void);


	/// ϵͳ�رա�
	void SystemShutdown(void);

	enum EnUpgrade_VerifyMode
	{
	    EnUpgrade_VerifyMode_None = 0 ,  /* ����У�� */
	    EnUpgrade_VerifyMode_Length ,  /* ֻ���ļ�����У�� */
	    EnUpgrade_VerifyMode_CrcOnly ,  /* ֻ��CrcУ�� */
	    EnUpgrade_VerifyMode_Full=50,	 /* ��ȫУ�� */
	};

	/// �յ��������ݺ���ϵͳ��ѹ����ͬ������ģʽ��ֱ����ѹ��ɲŷ��ء�
	/// \param [in] pData ���ݰ����ڴ��е�ָ�롣
	/// \param [in] dwLength ���ݳ��ȡ�
	/// \param [in] pProgress ��ǰ���Ȱٷֱȣ�����ִ�й����ж�ʱ���£����ô�ʹ��
	/// \param [in] iVerifyMode У�鷽ʽ�� ������ֵ�ο���EnUpgrade_VerifyMode
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	/// \retval -1  --  data check error
	/// \retval -2  --  version error
	/// \retval -3  --  date error
	/// \retval -4  --  vendor error
	/// \retval -10  --  write flash error
	/// \retval -11  --  fail when update(other error)
	/// \note ��ͬ��ϵͳ���ݴ���ĸ�ʽ��ͬ����Ӧ��������ݴ���ͽ�ѹ�ĵĳ���Ҳ��ͬ��
	int SystemUpgrade(unsigned char* pData, unsigned int dwLength, unsigned int *pProgress, unsigned char iVerifyMode);

	//pData ���ݸ�ʽ
	typedef struct burning_fheader_t
	{
		unsigned char ver;		/*   */
		unsigned char pkt_type;	/* packet type */
		unsigned char parts;	/*  �����������������֣�*/
		unsigned char Rev2;	/* */
		unsigned int crc;		/* ���ȫ�����ݵ�crcУ���� */
		unsigned int head_len;	/* header length */
		unsigned int total_len;	/* packet length */
	}
	BURNING_FHEADER_T;
	// �ֲ���ͷ
	typedef struct burning_part_header_t
	{
		char partname[32];		/*   */
		unsigned int offset_start;	/* ���ִӻ����ʼλ�ÿ�ʼ����������ʼ��ƫ�� */
		unsigned int offset_end;	/* ���ִӻ����ʼλ�ÿ�ʼ��������ĩβ��ƫ�� */
	}
	BURNING_PART_HEADER_T;

	/// ϵͳ��־��ʼ����ϵͳ��־������¼Ӧ�ó�������ʱ���еı�׼�����
	///
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemLogInit(void);


	/// ��¼ϵͳ��־�� ͬʱ������д������׼������塣
	///
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemLogWrite(char * format, ...);


	#define SERIAL_NUMBER_STRING_LEN 32

	/// ��ȡϵͳ���к�
	/// \param [out] pData ��Ʒ���к��ַ���
	/// \param [in] len ���泤��
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemGetSerialNumber(char *pData, int len);

	typedef struct __nvr_product_info_t
	{
	    unsigned int ProductCode; // ��Ʒ����
	    unsigned int VendorCode;  // �ͻ�����
	}NVR_PRODUCT_INFO_T, *pNVR_PRODUCT_INFO_T;

    #define PROD_D8004	21111 // 8004�� 2.2�汾����
    #define PROD_D8004B	21112 // 8004�� 3.0�汾����
    #define PROD_D8004_E	21113 // 8004-e�� 2.2�汾����
    #define PROD_D8004B_E	21114 // 8004-e�� 3.0�汾����

    #define PROD_D8104	21113
    #define PROD_D8008_H	21211
    #define PROD_D8008	21212
    #define PROD_D8016	21311
    #define PROD_D8404	22111

    #define PROD_D8408	22112
    #define PROD_D8416	22113

    #define PROD_D8808	23111
    #define PROD_D8816	23112
    #define PROD_D9004	26111
	/// ��ȡ��Ʒ�ͺ�
	/// \param [out] pProductInfo ָ���Ʒ�ͺ�NVR_PRODUCT_INFO_T��ָ�롣
	///
	/// \retval 0  �ɹ�
	/// \retval <0  ʧ��
	int SystemGetProductInfo(NVR_PRODUCT_INFO_T *pProductInfo);

	/// �Ƿ�Ϸ���Ȩ��
	///
	/// \retval 1  �Ϸ�
	/// \retval 0  �Ƿ�
	int SystemValidate(void);

	/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__SYSTEMM_API_H__

