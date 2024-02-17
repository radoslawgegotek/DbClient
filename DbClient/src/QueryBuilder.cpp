#include "QueryBuilder.h"

namespace rgmc
{

    void SqlQueryBuilder::getQuery() {
        std::cout << "Wygenerowany sql: " << m_query.str() << std::endl;
    }

    SqlQueryBuilder& SqlQueryBuilder::createTableArg(const std::vector<std::pair<std::string, std::string>>& columns)
    {
        m_tableColumns = columns;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::createTable(const std::string& tableName)
    {
        m_query.str("");

        m_query << "CREATE TABLE " << tableName << " (";

        for (size_t i = 0; i < m_tableColumns.size(); ++i) {
            m_query << m_tableColumns[i].first << " " << m_tableColumns[i].second;
            if (i != m_tableColumns.size() - 1) {
                m_query << ", ";
            }
        }

        m_query << ");";

        std::cout << "Wygenerowany SQL: " << m_query.str() << std::endl;


        m_tableColumns.clear();

        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::insert_into(const std::string& tableName, const std::vector<std::string>& columns, const std::vector<std::string>& values)
    {
        m_query << "INSERT INTO " << tableName << " (";
        m_query << join(columns, ", ");
        m_query << ") VALUES (";
        m_query << join(values, ", ");
        m_query << ");";

        std::cout << "Wygenerowany SQL: " << m_query.str() << std::endl;

        reset();

        return *this;
    }
    
    SqlQueryBuilder& SqlQueryBuilder::update(const std::string& tableName)
    {
        m_query.str("");
        m_query << "UPDATE " << tableName;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::set(const std::string& column, const std::string& value)
    {
        m_setConditionsUpdate.push_back(column + " = '" + value + "'");
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::join(const std::string& tableName, const std::string& condition)
    {
        m_joinConditions.push_back(tableName + " ON " + condition);
        return *this;
    }



    std::string SqlQueryBuilder::get_update_query()
    {
        if (!m_setConditionsUpdate.empty())
        {
            m_query << " SET " << join(m_setConditionsUpdate, ", ");
        }

        if (!m_whereConditions.empty())
        {
            m_query << " WHERE " << join(m_whereConditions, " AND ");
        }

        if (!m_joinConditions.empty())
        {
            m_query << " JOIN " << join(m_joinConditions, " ");;
        }
        return m_query.str();
    }

    SqlQueryBuilder& SqlQueryBuilder::alterTable(const std::string& tableName)
    {
        m_query.str("");
        m_query << "ALTER TABLE " << tableName;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::addColumn(const std::string& columnName, const std::string& columnType)
    {
        m_query << " ADD " << columnName << " " << columnType;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::dropColumn(const std::string& columnName)
    {
        m_query << " DROP COLUMN " << columnName;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::alterColumn(const std::string& columnName, const std::string& newColumnType)
    {
        m_query << " ALTER COLUMN " << columnName << " " << newColumnType;
        return *this;
    }

}