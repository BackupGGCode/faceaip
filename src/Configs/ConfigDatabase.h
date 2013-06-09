/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigDatabase.h - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigDatabase.h 5884 2012-07-02 09:15:22Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2012-06-25 10:17:36
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
// ConfigDatabase.h
#ifndef __ConfigDatabase_H__
#define __ConfigDatabase_H__

#include "ConfigBase.h"

//!	http�ͻ���
typedef struct tagConfigDatabase
{	
    // �豸�ӿ�
    std::string strHost 		;
    int iPort 		;
    std::string strUser 		;
    std::string strPwd 		;
    std::string strDatabase 		;
} ConfigDatabase;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<tagConfigDatabase, 1, 4> CConfigDatabase;

#endif //__ConfigDatabase_H__
