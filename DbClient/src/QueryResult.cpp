#include "QueryResult.h"

#include <iostream>


rgmc::QueryResult::QueryResult()
{
}

rgmc::record_list& rgmc::QueryResult::getData()
{
	return m_data;
}

std::vector<std::wstring>& rgmc::QueryResult::getColumnNames()
{
	return m_columnNames;
}

std::wostream& rgmc::operator<<(std::wostream& wostream, const QueryResult& qr)
{
	for (const auto& record : qr.m_data) 
	{
		wostream << "{\n";
		for (const auto& pair : record) {
			wostream << "\t";
			wostream << "\"" << pair.first << "\": \"" << pair.second << "\"";
			wostream << ",\n";
		}
		wostream << "\n}, " << std::endl;
	}
	return wostream;
}
