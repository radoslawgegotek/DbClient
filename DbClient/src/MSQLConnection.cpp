#define _CRT_SECURE_NO_WARNINGS
#include "MSQLConnection.h"

rgmc::MSQLConnection::MSQLConnection(const wchar_t* server, const wchar_t* database)
	: m_server(server), m_database(database)
{
	swprintf_s(m_connectionStr, CONN_STR_SIZE, L"DRIVER={ODBC Driver 17 for SQL Server};SERVER=%s;Database=%s;Trusted_Connection=yes;", m_server, m_database);
}