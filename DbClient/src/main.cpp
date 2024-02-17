#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>
#include "QueryBuilder.h"

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
    
//    SQLWCHAR sqlConnectionString[] = L"Server=DESKTOP-11SJP2F;Database=BazaProjektPipao;Trusted_Connection=True;Trust Server Certificate=true";
    SQLWCHAR sqlConnectionString[] = L"DRIVER = { ODBC Driver 17 for SQL Server }; SERVER = DESKTOP-11SJP2F; Database = BazaProjektPipao; Trusted_Connection = yes;";
    
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

        SQLWCHAR query[] = L"SELECT * from Dane";
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
    //builder.select({ "name"})
    //    .from("employees")
    //    .where("department", "IT")
    //    .where("name", "Marian", rgmc::SqlComparisonOperator::Like)
    //    .where("age", 22)
    //    .orderBy("age", true);

    //std::string query = builder.get_select_query();
    //std::cout << query << std::endl;
  

                                        //TWORZENIE TABELI
    //builder.createTableArg({
    //    {"id", "INT PRIMARY KEY"},
    //    {"name", "VARCHAR(255)"},
    //    {"age", "INT"}
    //    });

    //builder.createTable("new_table");
    
                                        //INSERT
                                        

    //builder.insert_into("new_table", { "kolumna1", "kolumna2" }, { "w1", "w2" });


                                        //UPDATE
    //builder.update("pracownicy").set("stanowisko", "Senior Programista").where("id", 1);
    //std::cout << builder.get_update_query() << std::endl;

                                        //JOIN
    //builder.update("pracownicy")
    //    .set("stanowisko", "Senior Programista")
    //    .join("dzialy d", "p.dzial_id = d.id")
    //    .where("id", "1");
    //std::cout << builder.get_update_query() << std::endl;

    //                                    //AlterTable
    //builder.alterTable("table_name").addColumn("new_column", "INT");
    //builder.getQuery();
    //builder.alterTable("table_name").dropColumn("column_to_drop");
    //builder.getQuery();
    //builder.alterTable("table_name").alterColumn("existing_column", "VARCHAR(255)");
    //builder.getQuery();

    
    return 0;
}