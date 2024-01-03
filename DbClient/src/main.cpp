#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>


#include "QueryBuilder.h"

//#define IS_SQL_ERR !IS_SQL_OK
//#define IS_SQL_OK(res) (res == SQL_SUCCESS_WITH_INFO || res == SQL_SUCCESS)
//#define SAFE_STR(str) ((str == NULL) ? _T("") : str)
//
//class ODBCConnection
//{
//public:
//    bool Connect(LPCTSTR svSource)
//    {
//        int nConnect = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
//        if (nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
//        {
//            nConnect = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
//            if (nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
//            {
//                nConnect = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDBC);
//                if (nConnect == SQL_SUCCESS || nConnect == SQL_SUCCESS_WITH_INFO)
//                {
//                    SQLSetConnectOption(m_hDBC, SQL_LOGIN_TIMEOUT, 5);
//                    short shortResult = 0;
//                    SQLTCHAR szOutConnectString[1024];
//                    nConnect = SQLDriverConnect(m_hDBC, NULL, (SQLTCHAR*)svSource,
//                        _tcslen(svSource), szOutConnectString,
//                        sizeof(szOutConnectString), &shortResult,
//                        SQL_DRIVER_NOPROMPT);
//                    return IS_SQL_OK(nConnect);
//                }
//            }
//        }
//        if (m_hDBC != NULL)
//        {
//            m_nReturn = SQLDisconnect(m_hDBC);
//            m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
//        }
//        if (m_hEnv != NULL)
//            m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
//        m_hDBC = NULL;
//        m_hEnv = NULL;
//        m_nReturn = SQL_ERROR;
//        return (IS_SQL_OK(nConnect));
//    }
//    ODBCConnection()
//    {
//        m_hDBC = NULL;
//        m_hEnv = NULL;
//        m_nReturn = SQL_ERROR;
//    }
//    virtual ~ODBCConnection()
//    {
//        if (m_hDBC != NULL)
//        {
//            m_nReturn = SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
//        }
//        if (m_hEnv != NULL)
//            m_nReturn = SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
//    }
//    void Disconnect()
//    {
//        if (m_hDBC != NULL)
//        {
//            m_nReturn = SQLDisconnect(m_hDBC);
//            m_hDBC = NULL;
//        }
//    }
//public:
//    operator HDBC()
//    {
//        return m_hDBC;
//    }
//
//private:
//    SQLRETURN m_nReturn; // Internal SQL Error code
//    HENV m_hEnv;        // Handle to environment
//    HDBC m_hDBC;        // Handle to database connection
//};
//
//class MSSQLConnection : public ODBCConnection
//{
//public:
//    enum enumProtocols
//    {
//        protoNamedPipes,
//        protoWinSock,
//        protoIPX,
//        protoBanyan,
//        protoRPC
//    };
//
//    MSSQLConnection() {}
//    virtual ~MSSQLConnection() {}
//    BOOL ConnectWindowsAuth(LPCTSTR Server = _T("(localhost\\SQLEXPRESS)"), LPCTSTR Database = _T("Test"))
//    {
//        TCHAR str[512] = _T("");
//        _stprintf(str, _T("Driver={ODBC Driver 17 for SQL Server};Server=%s;Database=%s;Trusted_Connection=yes;"),
//            SAFE_STR(Server), SAFE_STR(Database));
//        return ODBCConnection::Connect(str);
//    }
//};
//
//int main()
//{
//    MSSQLConnection connection;
//
//    // Replace 'YourDatabaseName' with the actual database name.
//    BOOL code = connection.ConnectWindowsAuth();
//    if (code)
//    {
//        std::cout << "Connected successfully!" << std::endl;
//
//        // Now you can use the 'connection' object to execute SQL queries.
//        // ...
//
//        connection.Disconnect();
//    }
//    else
//    {
//        std::cerr << "Connection failed!" << std::endl;
//    }
//
//    return 0;
//}

void extract_error(
    std::string fn,
    SQLHANDLE handle,
    SQLSMALLINT type)
{
    SQLINTEGER   i = 0;
    SQLINTEGER   native;
    SQLWCHAR      state[7];
    SQLWCHAR      text[256];
    SQLSMALLINT  len;
    SQLRETURN    ret;

    std::cout << "\nThe driver reported the following diagnostics whilst running " << fn << "\n\n";

    do
    {
        ret = SQLGetDiagRecW(type, handle, ++i, state, &native, text,
            sizeof(text), &len);
        if (SQL_SUCCEEDED(ret))
        {
            state[6] = L'\0'; // Ensure null-termination
            wprintf(L"%ls:%ld:%ld:%ls\n", state, i, native, text);
        }
    } while (ret == SQL_SUCCESS);

}

#define TestDb

int main()
{
#ifdef TestDb


    std::cout << "Program had started.." << std::endl;

    SQLHENV env;
    SQLHDBC dbc;
    SQLWCHAR outstr[1024];
    SQLSMALLINT outstrlen;
    SQLHSTMT stmt;
    SQLRETURN queryReturn;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    SQLRETURN SR;

    std::cout << "Attempting Connection " << std::endl;
    SQLWCHAR sqlConnectionString[] = L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=localhost\\SQLEXPRESS;Database=Test;Trusted_Connection=yes;";
    SR = SQLDriverConnectW(dbc, NULL, sqlConnectionString, SQL_NTS,
        outstr, sizeof(outstr), &outstrlen,
        SQL_DRIVER_NOPROMPT);

    std::cout << "Connecting ... " << std::endl;
    extract_error("SQLDriverConnect", dbc, SQL_HANDLE_DBC);

    if (SR != SQL_SUCCESS && SR != SQL_SUCCESS_WITH_INFO)
    {
        std::cout << "fail to connect" << std::endl;
    }
    else
    {
        std::cout << "connected" << std::endl;
        std::cout << "Executing SQL query ..." << std::endl;

        SQLWCHAR query[] = L"SELECT * from tasks";
        SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);

        queryReturn = SQLExecDirect(stmt, query, SQL_NTS);

        if (SQL_SUCCEEDED(queryReturn)) {
            // Pobranie wyników z zapytania
            SQLSMALLINT columns;
            queryReturn = SQLNumResultCols(stmt, &columns);

            while (SQL_SUCCEEDED(SQLFetch(stmt))) {
                for (SQLSMALLINT col = 1; col <= columns; ++col) {
                    SQLWCHAR buffer[256];
                    SQLLEN indicator;

                    queryReturn = SQLGetData(stmt, col, SQL_C_WCHAR, buffer, sizeof(buffer), &indicator);

                    if (SQL_SUCCEEDED(queryReturn)) {
                        if (indicator == SQL_NULL_DATA) {
                            std::wcout << L"NULL";
                        }
                        else {
                            std::wcout << buffer;
                        }
                        std::wcout << L"\t";
                    }
                }
                std::wcout << std::endl;
            }
        }
        else {
            // Obs³uga b³êdu wykonania zapytania
            extract_error("SQLExecDirect", stmt, SQL_HANDLE_STMT);
        }
        SQLFreeHandle(SQL_HANDLE_STMT, stmt);
        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
    }
#endif // TestDb

    rgmc::SqlQueryBuilder builder;
    builder.select({ "name", "age", "salary" })
        .from("employees")
        .where("department", "IT")
        .where("name", "Marian", rgmc::SqlComparisonOperator::Like)
        .where("age", 22)
        .orderBy("age", true);

    std::string query = builder.get_select_query();
    std::cout << query << std::endl;

    return 0;
}