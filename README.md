# DbClient

Simple C++ library for querying the MS SQL Server and receiving the results.

- CREATE
- UPDATE
- SELECT
- REMOVE
- ALTER TABLE

## Usage/Examples
```cpp
#include "QueryBuilder.h"
#include "MSQLConnection.h"

#define CREATE

int main()
{
	rgmc::SqlQueryBuilder builder;
	rgmc::MSQLConnection sqlConnection(L"localhost\\SQLEXPRESS", L"Test");

	sqlConnection.open();

#ifdef CREATE

	std::wstring sqlCreateGroups = builder
		.createTableArg({
			{"group_id", "INT IDENTITY(1,1) PRIMARY KEY"},
			{"group_name", "VARCHAR(255)"}
		})
		.createTable("Groups")
		.get_query();
	sqlConnection.executeQuery(sqlCreateGroups);

	std::wstring sqlCreateStudents = builder
		.createTableArg({
			{"student_id", "INT IDENTITY(1,1) PRIMARY KEY"},
			{"student_name", "VARCHAR(255)"},
			{"group_id", "INT"}
		})
		.addForeignKey("group_id", "Groups", "group_id")
		.createTable("Students")
		.get_query();
	sqlConnection.executeQuery(sqlCreateStudents);

#endif // CREATE

#ifdef INSERT_INTO
	builder.reset();
	std::wstring sql1 = builder
		.insert_into("Groups", { "group_name" }, { "'GROUP A'" })
		.get_query();
	sqlConnection.executeQuery(sql1);

	builder.reset();
	std::wstring sql2 = builder
		.insert_into("Groups", { "group_name" }, { "'GROUP B'" })
		.get_query();
	sqlConnection.executeQuery(sql2);

	builder.reset();
	std::wstring sql3 = builder
		.insert_into("Students", { "student_name", "group_id" }, { "'Kowalski'", "2" })
		.get_query();
	sqlConnection.executeQuery(sql3);

#endif // INSERT_INTO

#ifdef UPDATE
	builder.reset();
	std::wstring sqlUpdate = builder
		.update("Groups")
		.set("group_name", "GROUP C")
		.where("group_id", 1)
		.get_update_query();
	sqlConnection.executeQuery(sqlUpdate);
#endif // UPDATE

#ifdef SELECT1
	builder.reset();
	std::wstring sqlSelect = builder
		.select({ "Students.student_id", "Students.student_name", "Groups.group_name" })
		.from("Students")
		.join("Groups", "Students.group_id = Groups.group_id")
		.get_select_query();

	rgmc::QueryResult sqlResult = sqlConnection.executeQuery(sqlSelect);
	std::wcout << sqlResult;
#endif // SELECT1

#ifdef SELECT2
	builder.reset();
	std::wstring sqlSelect2 = builder
		.select({ "*" })
		.from("tasks")
		.where("id", 6, rgmc::SqlComparisonOperator::GreaterOrEqual)
		.orderBy("nazwa", true)
		.get_select_query();

	rgmc::QueryResult sqlResult = sqlConnection.executeQuery(sqlSelect2);
	std::wcout << sqlResult;
#endif // SELECT2

#ifdef ADD_COLUMN
	builder.reset();
	std::wstring sql = builder
		.alterTable("tasks")
		.addColumn("czas_wykonania", "DATETIME")
		.get_query();
	sqlConnection.executeQuery(sql);

#endif // ADD_COLUMN


#ifdef DROP_COLUMN
	builder.reset();
	std::wstring sqlAlterTable = builder
		.alterTable("tasks")
		.dropColumn("czas_wykonania")
		.get_query();
	sqlConnection.executeQuery(sqlAlterTable);

#endif // DROP_COLUMN

sqlConnection.close();
```
