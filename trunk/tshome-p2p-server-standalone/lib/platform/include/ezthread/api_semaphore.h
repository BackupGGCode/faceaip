/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * api_semaphore.h - _explain_
 *
 * Copyright (C) 2011 joy.woo@hotmail.com, All Rights Reserved.
 *
 * $Id: api_semaphore.h 5884 2012-05-16 09:16:25Z WuJunjie $
 *
 *  Explain:
 *     -�ź���API����װ��ͬ����ϵͳ���ź�������-
 *
 *  Update:
 *     2012-05-16 09:09:51   Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef __SEMAPHORE_API_H__
#define __SEMAPHORE_API_H__

#include "typedef_ezthread.h"

#ifdef __cplusplus
extern "C" {
#endif

/// \defgroup SemaphoreAPI API Semaphore
/// �ź���API����װ�˲�ͬ����ϵͳ���ź�������
///	\n ��������ͼ:
/// \code
///    ================================
///                |
///         SemaphoreCreate
///                |------------------+
///    SemaphorePost SemaphorePend    |
///                |------------------+
///        SemaphoreDestory
///                |
///    ================================
/// \endcode
/// @{


/// �����ź�����
/// 
/// \param [out] phSemaphore �ź��������ɹ�ʱ���������ź��������
/// \param [in] dwInitialCount �ź��������ĳ�ʼֵ��
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int SemaphoreCreate(EZ_HANDLE *phSemaphore, unsigned int dwInitialCount);


/// �����ź�����
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  ���ٳɹ�
/// \retval <0 ����ʧ��
int SemaphoreDestory(EZ_HANDLE hSemaphore);


/// �����ź���������
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int SemaphorePend(EZ_HANDLE hSemaphore);


/// �����ź���������
/// 
/// \param [in] hSemaphore �ź��������
/// \retval 0  �����ɹ�
/// \retval <0  ����ʧ��
int SemaphorePost(EZ_HANDLE hSemaphore);

/// @} end of group

#ifdef __cplusplus
}
#endif

#endif //__SEMAPHORE_API_H__

