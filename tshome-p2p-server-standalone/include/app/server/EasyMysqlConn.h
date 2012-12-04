/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*
 * EasyMysqlConn.h - _explain_
 *
 * 
 *  Explain:
 *     -�ػ���-
 *
 *  Update:
 *     2012-11-15 09:09:51   Create
 */
/*-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef __EASYMYSQLCONN_H__
#define __EASYMYSQLCONN_H__

#include "mysqlpool.h"

class EasyMysqlConn
{
public:
	inline EasyMysqlConn()
	{
		conn = pool_getConn();
	};

	MysqlConn getConnection()
	{
		return conn;
	};
	
	inline ~EasyMysqlConn()
	{
		pool_recycle(conn);
	};
protected:
private:
	MysqlConn conn;
};


#endif //__EASYMYSQLCONN_H__