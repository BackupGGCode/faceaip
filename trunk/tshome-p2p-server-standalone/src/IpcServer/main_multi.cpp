/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgent.cpp - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralAgent.cpp 0001 2012-04-06 09:58:43Z WuJunjie $
 *
 *  Explain:
 *     -GeneralAgent �Դ����̣߳� ��ʹ��main�߳�-
 *
 *  Update:
 *     2011-10-13 22:00:11 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include "CommonInclude.h"

#include "GeneralAgent.h"

int main(int argc, char *argv[])
{
   g_GeneralAgentTSIPC.Start(NULL);

    // main�̵߳ȴ�
    while (1)
    {
        sleep(1);
    }

    return 0;
}
