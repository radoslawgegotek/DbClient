#include "ODBCConnection.h"

rgmc::ODBCConnection::ODBCConnection()
{
	m_hEnv = nullptr;
	m_hDbc = nullptr;
	m_hStmt = nullptr;
	m_errorCode = 0;
}

rgmc::QueryResult rgmc::ODBCConnection::fetchRows()
{
	QueryResult result;
	SQLNumResultCols(m_hStmt, &result.m_columns);
	result.m_columnNames.reserve(result.m_columns);

	for (int i = 1; i <= result.m_columns; ++i)
	{
		SQLWCHAR buffer[256];
		SQLSMALLINT indicator;
		SQLColAttribute(m_hStmt, i, SQL_DESC_NAME, buffer, sizeof(buffer), &indicator, NULL);
		result.m_columnNames.emplace_back(buffer);
	}

	std::vector<std::pair<std::wstring, std::wstring>> row;
	while (SQL_SUCCEEDED(SQLFetch(m_hStmt)))
	{
		row.clear();
		for (SQLSMALLINT col = 1; col <= result.m_columns; ++col)
		{
			std::pair<std::wstring, std::wstring> columnData;
			SQLWCHAR buffer[256];
			SQLLEN indicator;

			m_errorCode = SQLGetData(m_hStmt, col, SQL_C_WCHAR, buffer, sizeof(buffer), &indicator);
			if (SQL_SUCCEEDED(m_errorCode))
			{
				if (indicator == SQL_NULL_DATA)
					columnData = std::make_pair(L"NULL", L"NULL");
				else
					columnData = std::make_pair(result.m_columnNames[col - 1], buffer);
			}
			row.emplace_back(columnData);
		}
		result.m_data.emplace_back(row);
	}
	return result;
}

rgmc::ODBCConnection::~ODBCConnection()
{
	close();
}

bool rgmc::ODBCConnection::open()
{
	if (m_hEnv || m_hDbc || m_hStmt) close();


	m_errorCode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (m_errorCode != SQL_SUCCESS && m_errorCode != SQL_SUCCESS_WITH_INFO)
		throw std::runtime_error("Failed to allocate environment handle");

	m_errorCode = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (m_errorCode != SQL_SUCCESS && m_errorCode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		throw std::runtime_error("Failed to set environment attribute");
	}

	m_errorCode = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
	if (m_errorCode != SQL_SUCCESS && m_errorCode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		throw std::runtime_error("Failed to allocate database connection handle");
	}

	SQLDriverConnect(m_hDbc, NULL, m_connectionStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

	if (m_errorCode != SQL_SUCCESS && m_errorCode != SQL_SUCCESS_WITH_INFO)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		throw std::runtime_error("Failed to connect to database");
	}

	m_errorCode = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	if (m_errorCode != SQL_SUCCESS && m_errorCode != SQL_SUCCESS_WITH_INFO)
	{
		close();
		throw std::runtime_error("Failed to allocate statement handle");
	}

	return m_errorCode == SQL_SUCCESS;
}

rgmc::QueryResult rgmc::ODBCConnection::executeQuery(const wchar_t* query)
{
	if (!m_hDbc) throw std::runtime_error("Database connection is not open");

	SQLRETURN retcode;

	retcode = SQLExecDirect(m_hStmt, (SQLWCHAR*)query, SQL_NTS);

	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
	{
		SQLSMALLINT iRec = 0;
		SQLWCHAR wszMessage[SQL_MAX_MESSAGE_LENGTH];
		SQLWCHAR wszState[SQL_SQLSTATE_SIZE + 1];
		SQLINTEGER pfNativeError;
		SQLSMALLINT cbMessage;
		while (SQLGetDiagRec(SQL_HANDLE_STMT, m_hStmt, ++iRec, wszState, &pfNativeError,
			wszMessage, sizeof(wszMessage), &cbMessage) == SQL_SUCCESS) {
			std::wcerr << L"SQL Error: " << wszState << L" " << wszMessage << std::endl;
		}
	}

	QueryResult result = fetchRows();
	SQLFreeStmt(m_hStmt, SQL_CLOSE);

	return result;
}

void rgmc::ODBCConnection::close()
{
	if (m_hDbc)
	{
		SQLDisconnect(m_hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		m_hDbc = nullptr;
	}

	if (m_hEnv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = nullptr;
	}

	if (m_hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = nullptr;
	}
}