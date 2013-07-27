/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * ConfigSmartWebs.h - _explain_
 *
 * Copyright (C) 2013 ezlibs.com, All Rights Reserved.
 *
 * $Id: ConfigSmartWebs.h 5884 2013-06-05 05:11:14Z WuJunjie $
 *
 *  Explain:
 *     -explain-
 *
 *  Update:
 *     2013-06-05 05:11:14  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __ConfigSmartWebs_H__
#define __ConfigSmartWebs_H__

#include "ConfigBase.h"
//!	
typedef struct tagConfigSmartWebs
{
    int SPort;// �˿�
    int iRetries;// �˿���̽����
 
    // �ն�״̬ά��/ �������
    // 0- �����
    // >0 ��
    int CheckPeriod;
    // ��ʱʱ�� 
    int PeerTimeout;

// ��������
    std::string strSName;
}
ConfigSmartWebs;

//1 -�ṹ��Ŀ
//4 -�۲��������Ŀ
typedef TConfig<ConfigSmartWebs, 1, 4> CConfigSmartWebs;

#endif //__ConfigSmartWebs_H__
