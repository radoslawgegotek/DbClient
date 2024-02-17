#pragma once

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <sstream>

namespace rgmc
{
    enum SqlComparisonOperator 
    {
        Equals,
        Differs,
        Like,
        LessOrEqual,
        GreaterOrEqual
    };

    struct SortColumn 
    {
        std::string m_column;
        bool m_ascending;

        SortColumn(const std::string& column, bool ascending)
            : m_column(column), m_ascending(ascending) {}
        SortColumn() {}
    };

    class SqlQueryBuilder 
    {
    private:
        std::vector<std::string>    m_columnsToSelect;
        std::string                 m_dataSource;
        std::vector<std::string>    m_whereConditions;
        std::vector<std::string>    m_parameters;
        std::stringstream           m_query;
        SortColumn                  m_sortingBy;
        int                         m_topCount;
        bool                        m_isDistinct;
        std::vector<std::string>    m_setConditionsUpdate;
        std::vector<std::string>    m_joinConditions;
        std::vector<std::pair<std::string, std::string>> m_tableColumns;

    public:
        SqlQueryBuilder()
            : m_topCount(-1), m_isDistinct(false) 
        {
            reset();
        }

        SqlQueryBuilder& select(const std::vector<std::string>& columns) 
        {
            m_columnsToSelect.insert(m_columnsToSelect.end(), columns.begin(), columns.end());
            return *this;
        }

        SqlQueryBuilder& select_distinct(const std::vector<std::string>& columns) 
        {
            m_isDistinct = true;
            return select(columns);
        }

        SqlQueryBuilder& from(const std::string& dataSource) 
        {
            this->m_dataSource = dataSource;
            return *this;
        }

        SqlQueryBuilder& where(
            const std::string& column, 
            const std::string& value, 
            SqlComparisonOperator filterOperator = SqlComparisonOperator::Equals)
        {
            add_filter(column, value, filterOperator);
            return *this;
        }

        SqlQueryBuilder& where(
            const std::string& column, 
            int value, 
            SqlComparisonOperator filterOperator = SqlComparisonOperator::Equals)
        {
            add_filter(column, std::to_string(value), filterOperator);
            return *this;
        }

        SqlQueryBuilder& orderBy(const std::string& columnName, bool ascending) 
        {
            m_sortingBy = { columnName, ascending };
            return *this;
        }

        std::string get_select_query()
        {
            m_query << "SELECT ";

            if (m_isDistinct) 
            {
                m_query << "DISTINCT ";
            }

            m_query << join(m_columnsToSelect, ", ") << " FROM " << m_dataSource;

            if (!m_whereConditions.empty()) 
            {
                m_query << " WHERE " << join(m_whereConditions, " AND ");
            }

            if (m_sortingBy.m_column != "") {
                m_query << " ORDER BY " << m_sortingBy.m_column << (m_sortingBy.m_ascending ? " ASC" : " DESC");
            }

            if (!m_joinConditions.empty())
            {
                m_query << " JOIN " << join(m_joinConditions, " ");;
            }

            return m_query.str();
        }

        const std::vector<std::string>& GetParameters() const 
        {
            return m_parameters;
        }

        void getQuery();
        SqlQueryBuilder& createTableArg(const std::vector<std::pair<std::string, std::string>>& columns);
        SqlQueryBuilder& createTable(const std::string& tableName);
        SqlQueryBuilder& insert_into(const std::string& tableName, const std::vector<std::string>& columns, const std::vector<std::string>& values);
        SqlQueryBuilder& update(const std::string& tableName);
        SqlQueryBuilder& set(const std::string& column, const std::string& value);
        std::string get_update_query();
        SqlQueryBuilder& join(const std::string& tableName, const std::string& condition);

        SqlQueryBuilder& alterTable(const std::string& tableName);
        SqlQueryBuilder& addColumn(const std::string& columnName, const std::string& columnType);
        SqlQueryBuilder& dropColumn(const std::string& columnName);
        SqlQueryBuilder& alterColumn(const std::string& columnName, const std::string& newColumnType);

    private:
        void add_filter(
            const std::string& column, 
            const std::string& valueToFilter, 
            const SqlComparisonOperator filterOperator)
        {
            switch (filterOperator) {
            case Equals:
                m_whereConditions.push_back(column + " = " + valueToFilter);
                break;
            case Differs:
                m_whereConditions.push_back(column + " != " + valueToFilter);
                break;
            case Like:
                m_whereConditions.push_back(column + " LIKE " + valueToFilter);
                break;
            case LessOrEqual:
                m_whereConditions.push_back(column + " <= " + valueToFilter);
                break;
            case GreaterOrEqual:
                m_whereConditions.push_back(column + " >= " + valueToFilter);
                break;
            }
        }

        void reset() 
        {
            m_columnsToSelect.clear();
            m_dataSource.clear();
            m_whereConditions.clear();
            m_parameters.clear();
            m_isDistinct = false;
            m_query.clear();
        }

        std::string join(const std::vector<std::string>& elements, const std::string& delimiter) const
        {
            std::string result;
            for (size_t i = 0; i < elements.size(); i++) 
            {
                if (i > 0) 
                {
                    result += delimiter;
                }
                result += elements[i];
            }
            return result;
        }


    };
}

