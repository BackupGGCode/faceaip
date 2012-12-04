/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralAgentUdpSocket.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralAgentUdpSocket.h 5884 2012-07-19 04:20:52Z WuJunjie $
 *
 *  Explain:
 *     -udpʵ��-
 *
 *  Update:
 *     2012-4-11 9:32:49 WuJunjie Create
 *     2012-7-19 16:19:02 ֧�������̷߳�����Ϣ��ģ��
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _GENERALAGENTUDPSOCKET_H
#define _GENERALAGENTUDPSOCKET_H

#include <UdpSocket.h>

#define CGENERALAGENTUDPSOCKET_SOCKET_NAME "CGeneralAgentUdpSocket"

// �ͻ���ʧЧʱ�� 30����
#define INVALID_PEER_TIMEOUT 60*30
// ��Ԫ�����ã� 20s
//#define INVALID_PEER_TIMEOUT 20
class IPCPEER_T
{
public:
	std::string strName;// ��ʱ����
	struct sockaddr PeerAddr;
	socklen_t PeerAddrLen;
	time_t ttLastOnData;
	IPCPEER_T()
	{
		strName = "";
		memset(&PeerAddr, 0, sizeof(struct sockaddr));
		PeerAddrLen = sizeof(struct sockaddr);
		
		ttLastOnData = 0;
	}
	~IPCPEER_T()
	{;}
	
};


class CGeneralAgentUdpSocket : public UdpSocket
{
public:
	CGeneralAgentUdpSocket(ISocketHandler&, std::string strSocketName=CGENERALAGENTUDPSOCKET_SOCKET_NAME);
	// �ڲ�ά������
	void Update();

	void OnRawData(const char *,size_t,struct sockaddr *,socklen_t);

	// ��������֪�ͻ��˷�����Ϣ
	int SendtoAllPeer( const char *pData, size_t lDataLen);

	// ret >= 0 peer num; <0 error
	int GetPeer(std::vector<IPCPEER_T> &dstPeer);
	//int GetPeer(IPCPEER_T &dstPeer, std::string strHostID);
	//int GetPeer(std::vector<IPCPEER_T> &dstPeer, std::string strHost, unsigned short usPort);
	
 	/** Outgoing traffic counter. */
	virtual uint64_t GetBytesSent(bool clear = false);

	/** Incoming traffic counter. */
	virtual uint64_t GetBytesReceived(bool clear = false);

private:
	void OnPeer(struct sockaddr *pPeer=NULL, socklen_t iSaLen=0);

	// ���ͬipc ͨѶ�����пͻ���
	std::vector<IPCPEER_T> m_IpcPeer;
	std::vector<IPCPEER_T>::iterator it;

	uint64_t m_BytesSent;
	uint64_t m_BytesReceived;
};

#endif // _GENERALAGENTUDPSOCKET_H
