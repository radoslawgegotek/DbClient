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
        void add_filter(
            const std::string& column,
            const std::string& valueToFilter,
            const SqlComparisonOperator filterOperator);
        void reset();
        std::string join(const std::vector<std::string>& elements, const std::string& delimiter) const;

    public:
        SqlQueryBuilder()
            : m_topCount(-1), m_isDistinct(false) 
        {
            reset();
        }

        SqlQueryBuilder& select(const std::vector<std::string>& columns);
        SqlQueryBuilder& select_distinct(const std::vector<std::string>& columns);
        SqlQueryBuilder& from(const std::string& dataSource);
        SqlQueryBuilder& where(
            const std::string& column,
            const std::string& value,
            SqlComparisonOperator filterOperator = SqlComparisonOperator::Equals);
        SqlQueryBuilder& where(
            const std::string& column,
            int value,
            SqlComparisonOperator filterOperator = SqlComparisonOperator::Equals);
        SqlQueryBuilder& orderBy(const std::string& columnName, bool ascending);
        std::string get_select_query();
        const std::vector<std::string>& GetParameters() const;
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
    };
}

