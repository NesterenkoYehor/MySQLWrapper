#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <exception>
#include <mysql/mysql.h>
#include "../library/sqlwrapper.hpp"

using namespace SQLWrap;

int main()
{
    try
    {
        std::map<int, std::vector<std::string>> data;

        connectToDatabase("localhost", "worker", "12Err##tur#-3425TTu", "test_data");

        Table table
        (
            "People",
            Field("Name", types::varchar(30), attrs::notNullValue, attrs::unique),
            Field("ID", types::integer, attrs::notNullValue, attrs::primaryKey),
            Field("Date", types::date, attrs::notNullValue)
        );

        table.insertValues
        (
            FieldInput("Name", "ID", "Date"),
                 Value("Peter", 1, "22.01.2001"),
                 Value("Igor", "2", "23.02.2021")
        );

        table.deleteValues(Pair("Name", "Peter"), attrs::And, Pair("ID", 2));

        table.addConstraint
        (
            "CHK_ID",
            attrs::Check
            (
                Pair("ID", 10, "<=")
            )
        );

        data = table.selectValues(FieldOutput("*"));

        for (auto it_map : data)
        {
            for (auto it_vect : it_map.second)
                std::cout << it_vect << '\t';

            std::cout << '\n';
        }

        data = table.selectValues
        (
            FieldOutput("Name", "ID"),
            Pair("ID", 2, "<")
        );

        for (auto it_map : data)
        {
            for (auto it_vect : it_map.second)
                std::cout << it_vect << '\t';

            std::cout << '\n';
        }

        table.updateValues
        (
            Set
            (
                Pair("Name", "Andrey")
            ),
            Pair("ID", 2)
        );

        data = table.selectValues(FieldOutput("*"));

        for (auto it_map : data)
        {
            for (auto it_vect : it_map.second)
                std::cout << it_vect << '\t';

            std::cout << '\n';
        }

        table.dropTable();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
};
