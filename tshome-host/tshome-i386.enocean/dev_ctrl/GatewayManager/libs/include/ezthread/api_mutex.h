/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * api_mutex.h - _explain_
 *
 * Copyright (C) 2011 joy.woo@hotmail.com, All Rights Reserved.
 *
 * $Id: api_mutex.h 5884 2012-05-16 09:16:35Z WuJunjie $
 *
 *  Explain:
 *     -��������װ-
 *
 *  Update:
 *     2012-05-16 09:09:51   Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __API_MUTEX_H__
#define __API_MUTEX_H__

#include "typedef_ezthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup MutexAPI API Mutex
/// ������API����װ�˲�ͬ����ϵͳ�Ļ���������
///	\n ��������ͼ:
/// \code
///    ===========================
///                |
///           MutexCreate
///                |------------+
///      MutexEnter MutexLeave  |
///                |------------+
///           MutexDestory
///                |
///    ===========================
/// \endcode
/// @{

/// ����������
enum mutex_type
{
	/// ���ٻ�����������ٽ����Ѿ���һ���̻߳�ȡ�������̻߳��߸��߳��ٴλ�ȡʱ��
	/// �����߳�ֱ�ӱ�����
	MUTEX_FAST = 0,

	/// �ݹ黥�������Ѿ���ȡ�ٽ������߳̿��Զ�λ�ȡ�������������ڲ�Ӧ�л�ȡ����
	/// ���������Ҫ��ȫ�ͷ��ٽ�����һ��Ҫ��֤��ȡ�������ͷŴ���һ�¡�
	MUTEX_RECURSIVE,
};


/// ������������
/// 
/// \param [out] phMutex �����������ɹ�ʱ�������Ļ����������
/// \param [in] nType �����������ͣ�ȡmutex_typeö������ֵ
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexCreate(EZ_HANDLE *phMutex, int nType);


/// ���ٻ�������
/// 
/// \param [in] hMutex �����������
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
int MutexDestory(EZ_HANDLE hMutex);


/// �����ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexEnter(EZ_HANDLE hMutex);


/// �뿪�ٽ�����
/// 
/// \param [in] hMutex �����������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int MutexLeave(EZ_HANDLE hMutex);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__API_MUTEX_H__

