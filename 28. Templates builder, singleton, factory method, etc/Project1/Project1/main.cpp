#include "allinclude.h"
#include "sql_query_builder.h"

int main() {
    SqlSelectQueryBuilder query_builder;

    std::string query1 = query_builder
        .AddColumn("name")
        .AddColumn("phone")
        .AddFrom("students")
        .AddWhere("id", "21")
        .AddWhere("name", "Alex")
        .BuildQuery();

    std::cout << query1 << std::endl;

    SqlSelectQueryBuilder query_builder2;
    std::string query2 = query_builder2
        .AddColumns({ "name", "age", "email" })
        .AddFrom("users")
        .AddWhere({ {"country", "Israel"}, {"active", "1"} })
        .BuildQuery();

    std::cout << query2 << std::endl;

    return 0;
}
