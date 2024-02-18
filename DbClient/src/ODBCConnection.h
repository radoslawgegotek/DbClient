#pragma once

#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>
#include <stdexcept>
#include <iostream>

#include "QueryResult.h"

namespace rgmc
{
	class ODBCConnection
	{
	public:
		virtual ~ODBCConnection();
		bool open();
		void close();
		QueryResult executeQuery(std::wstring query);


	protected:
		ODBCConnection();
		static const int CONN_STR_SIZE = 1024;
		SQLWCHAR m_connectionStr[CONN_STR_SIZE];

	private:
		SQLHENV m_hEnv;				// Handle to environment
		SQLHDBC m_hDbc;				// Handle to database connection
		SQLHSTMT m_hStmt;			// Handle to sql statement
		SQLRETURN m_errorCode;		// SQL Error code

		//helper
		QueryResult fetchRows();
	};
}


