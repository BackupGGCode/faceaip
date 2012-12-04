/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigDB.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: ConfigDB.h 5884 2012-07-02 09:15:22Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// ConfigDB.h
#ifndef __CONFIGDB_H__
#define __CONFIGDB_H__

#include "ConfigBase.h"

//!	http�ͻ���
typedef struct tagConfigDB
{	
    // �豸�ӿ�
    std::string strHost 		;
    int iPort 		;
    std::string strUser 		;
    std::string strPwd 		;
    std::string strDatabase 		;
} ConfigDB;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<tagConfigDB, 1, 4> CConfigDB;

#endif //__CONFIGDB_H__