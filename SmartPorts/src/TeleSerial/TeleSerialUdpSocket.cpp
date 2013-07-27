/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentUdpSocket.cpp - _explain_
 *
 * Copyright (C) 2011 ezlibs.com, All Rights Reserved.
 *
 * $Id: GeneralAgentUdpSocket.cpp 5884 2012-07-19 04:21:23Z WuJunjie $
 *
 *  Explain:
 *     -udpʵ��-
 *
 *  Update:
 *     2012-4-11 9:32:49 WuJunjie Create
 *     2012-7-19 16:19:02 ֧�������̷߳�����Ϣ��ģ��
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#include <stdio.h>
#include <Utility.h>

#ifdef TeleSerial_FUNC_SOCKETPROCESSOR
#include "GeneralSocketProcessor.h"
#endif //TeleSerial_FUNC_SOCKETPROCESSOR

#include "GeneralAgentUdpSocket.h"

CTeleSerialUdpSocket::CTeleSerialUdpSocket(ISocketHandler& h, std::string strSocketName)
        :UdpSocket(h)
{
	SetSockName(strSocketName);
}

void CTeleSerialUdpSocket::OnRawData(const char *p,size_t l,struct sockaddr *sa_from,socklen_t sa_len)
{
	//�����̷߳�����Ϣ
#ifdef TeleSerial_FUNC_SOCKETPROCESSOR
    g_GeneralSocketProcessor.SendSocketMsg(0, this, p, l, sa_from, sa_len);
#else
#if 1
    char buf[1024];
    struct sockaddr_in sa;
    memcpy(&sa,sa_from,sa_len);
    ipaddr_t a;
    memcpy(&a,&sa.sin_addr,4);
    std::string ip;
    Utility::l2ip(a,ip);

    snprintf(buf, sizeof(buf), "Received %d bytes from: %s:%d\n", l,ip.c_str(),ntohs(sa.sin_port));
    printf(buf);

    printf("%s\n",static_cast<std::string>(p).substr(0,l).c_str());

    sendto(this->GetSocket(), buf, strlen(buf), 0, sa_from, sa_len);
    sendto(this->GetSocket(), static_cast<std::string>(p).substr(0,l).c_str(), static_cast<std::string>(p).substr(0,l).size(), 0, sa_from, sa_len);
#endif

#endif
}

/** Outgoing traffic counter. */
uint64_t CTeleSerialUdpSocket::GetBytesSent(bool clear )
{
    return m_BytesSent;
}

/** Incoming traffic counter. */
uint64_t CTeleSerialUdpSocket::GetBytesReceived(bool clear )
{
    return m_BytesReceived;
}
