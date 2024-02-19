#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>
#include <algorithm>

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