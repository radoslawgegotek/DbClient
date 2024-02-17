#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>
#include <tchar.h>
#include "QueryBuilder.h"
#include "MSQLConnection.h"


int main()
{

    rgmc::SqlQueryBuilder builder;
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