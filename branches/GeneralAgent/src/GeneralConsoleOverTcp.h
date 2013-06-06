/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * GeneralConsoleOverTcp.h - _explain_
 *
 * Copyright (C) 2011 tiansu-china.com, All Rights Reserved.
 *
 * $Id: GeneralConsoleOverTcp.h 0001 2012-04-28 09:58:37Z WuJunjie $
 *
 *  Explain:
 *     -self dbg platform-
 *
 *  Update:
 *     2012-04-28 09:58:37 WuJunjie Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef _GeneralConsoleOverTcp_H
#define _GeneralConsoleOverTcp_H

#include <TcpSocket.h>
#include <ISocketHandler.h>

#define LoginStatusNotlogin

enum    EnLoginStatus
{
    LoginStatusNotLogin =  0,
    LoginStatusWaitPwd,
    LoginStatusLoginUser,
    LoginStatusLoginAdmin,
};

class CGeneralConsoleOverTcp : public TcpSocket
{
public:
	CGeneralConsoleOverTcp(ISocketHandler&, std::string strSocketName="GeneralConsoleOverTcp");

	void OnAccept();
	void OnLine(const std::string& );
private:
	void tprintf(const char *format, ...);
	
	EnLoginStatus m_enLoginStatus;
	std::string m_strUName;
	std::string m_strUPwd;
};

#endif // _GeneralConsoleOverTcp_H
