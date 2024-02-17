#pragma once

#include <string>
#include <vector>
#include <map>

namespace rgmc
{
	using record_list = std::vector<std::vector<std::pair<std::wstring, std::wstring>>>;

	class QueryResult
	{
	public:
		QueryResult();

		record_list& getData();
		std::vector<std::wstring>& getColumnNames();

		friend class ODBCConnection;
		friend std::wostream& operator<<(std::wostream& ostream, const QueryResult& qr);

	private:
		record_list m_data;
		std::vector<std::wstring> m_columnNames;
		short m_columns;
	};
	
}