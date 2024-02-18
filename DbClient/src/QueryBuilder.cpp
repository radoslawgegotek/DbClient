#include "QueryBuilder.h"

namespace rgmc
{

    std::wstring SqlQueryBuilder::get_query() {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(m_query.str());
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



    std::wstring SqlQueryBuilder::get_update_query()
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
        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(m_query.str());
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


    SqlQueryBuilder& SqlQueryBuilder::select(const std::vector<std::string>& columns)
    {
        m_columnsToSelect.insert(m_columnsToSelect.end(), columns.begin(), columns.end());
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::select_distinct(const std::vector<std::string>& columns)
    {
        m_isDistinct = true;
        return select(columns);
    }

    SqlQueryBuilder& SqlQueryBuilder::from(const std::string& dataSource)
    {
        this->m_dataSource = dataSource;
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::where(
        const std::string& column,
        const std::string& value,
        SqlComparisonOperator filterOperator)
    {
        add_filter(column, value, filterOperator);
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::where(
        const std::string& column,
        int value,
        SqlComparisonOperator filterOperator)
    {
        add_filter(column, std::to_string(value), filterOperator);
        return *this;
    }

    SqlQueryBuilder& SqlQueryBuilder::orderBy(const std::string& columnName, bool ascending)
    {
        m_sortingBy = { columnName, ascending };
        return *this;
    }

    std::wstring SqlQueryBuilder::get_select_query()
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

        return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(m_query.str());
    }

    const std::vector<std::string>& SqlQueryBuilder::GetParameters() const
    {
        return m_parameters;
    }

    void SqlQueryBuilder::add_filter(
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
    void SqlQueryBuilder::reset()
    {
        m_columnsToSelect.clear();
        m_dataSource.clear();
        m_whereConditions.clear();
        m_parameters.clear();
        m_isDistinct = false;
        m_query.clear();
    }

    std::string SqlQueryBuilder::join(const std::vector<std::string>& elements, const std::string& delimiter) const
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
}