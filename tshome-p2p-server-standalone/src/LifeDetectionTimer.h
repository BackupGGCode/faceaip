/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * LifeDetectionTimer.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: Solar.h 5884 2012-10-24 09:17:46Z sunzq $
 *
 *  Explain:
 *     -
 *      ������ʱ�����
 *     -
 *
 *  Update:
 *     2012-10-24 09:17:46Z sunzq  Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __LIFEDETECTIONTIMER_H__
#define __LIFEDETECTIONTIMER_H__

#include "CommonInclude.h"

#include "EZThread.h"
#include "EZTimer.h"

class LifeDetectionTimer : public CEZObject
{
	// ״̬ά��/ �������
	int hostPeriod;
	int termPeriod;
	//������ʱʱ��
	int hostTimeout;
	// �ն˳�ʱʱ��
	int termTimeout;

public:
    LifeDetectionTimer();
    ~LifeDetectionTimer();
	void start(int argc, char * argv[]);
	
    void TimerProcHost();
	void TimerProcTerm();
	void TimerProcWeb();

	void setHostPeriod(int period);

	int getHostPeriod();

	void setTermPeriod(int period);

	int getTermPeriod();

	int getHostWarningLimit();

	int getTermWarningLimit();
private:
	CEZTimer          m_detectionHostTimer;
	CEZTimer          m_detectionTermTimer;
	CEZTimer          m_detectionWebTimer;
};


#endif //__LIFEDETECTIONTIMER_H__

