/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigGeneral.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: ConfigGeneral.h 5884 2012-07-02 09:15:15Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __CONFIGGENERAL_H__
#define __CONFIGGENERAL_H__

#include "ConfigBase.h"
//!	��ͨ����
typedef struct tagCONFIG_GENERAL
{	
	int iLocalNo;
	std::string  strMachineName;
	std::string strProductID;
	std::string strVendor;
	std::string strSerialNumber;
	std::string strVersion;
	// ��־�ļ�·�� ��Ҫ��ǰ����
	std::string strLogFilePath;
	// ��־�ļ��� �������Ϊ0�� ���������Ļ
	std::string strLogFileName;
	int maxFileSize;
	int maxBackupIndex;

	// ���Զ˿�
	int portConsoleOverTcp;
} CONFIG_GENERAL;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<CONFIG_GENERAL, 1, 4> CConfigGeneral;

#endif //__CONFIGGENERAL_H__
