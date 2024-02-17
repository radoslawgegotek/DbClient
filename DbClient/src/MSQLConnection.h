#pragma once

#include "ODBCConnection.h"

namespace rgmc
{
	class MSQLConnection : public ODBCConnection
	{
	public:
		MSQLConnection(const wchar_t* server, const wchar_t* database);

	private:
		const wchar_t* m_server;
		const wchar_t* m_database;
	};
}

