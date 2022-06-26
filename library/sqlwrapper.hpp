#ifndef SQLWRAPPER_HPP
#define SQLWRAPPER_HPP

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <exception>
#include <mysql/mysql.h>

// variables and functions that user should not have access to
namespace
{
    MYSQL mysql;
    MYSQL_RES *res = nullptr;
    MYSQL_ROW row;
    // MYSQL_FIELD *field = nullptr;

    void inputTypes(std::stringstream &stream, const char *repl1, const char *repl2) {}

    template <class firstArg, class... Args>
    void inputTypes(std::stringstream &stream, const char *repl1, const char *repl2, firstArg fArg, Args... args)
    {
        stream << repl1 << fArg << repl1 << repl2;

        inputTypes(stream, repl1, repl2, args...);
    }

    // #ifdef CHECK_TAMPLATE

    bool checkTemplate()
    {
        return false;
    }

    template <class... Args>
    bool checkTemplate(Args... args)
    {
        return true;
    }

    // #endif
};

namespace SQLWrap
{
    // namespace that consists of attributes 
    namespace attrs
    {
        const char *primaryKey = "PRIMARY KEY";
        const char *unique = "UNIQUE";
        const char *autoIncrement = "AUTO_INCREMENT";
        const char *nullValue = "NULL";
        const char *notNullValue = "NOT NULL";
        const char *setNullValue = "SET NULL";
        const char *setDefault = "SET DEFAULT";
        const char *And = "AND";
        const char *Or = "OR";
        const char *onDelete = "ON DELETE";
        const char *onUpdate = "ON UPDATE";
        const char *Cascade = "CASCADE";
        const char *Restrict = "RESTRICT";
        const char *noAction = "NO ACTION";

        // generate a request which which forms CHECK()
        template <class checkType, class... checkList>
        std::string Check(checkType checkValue, checkList... args)
        {
            std::stringstream stream;

            stream << "CHECK (" << checkValue << ' ';

            inputTypes(stream, "", " ", args...);

            std::string result = stream.str();

            result[result.size() - 1] = ')';

            return result;
        }

        // generate a request which froms DEFAULT
        template <class valueType>
        std::string Default(valueType value)
        {
            std::stringstream stream;

            stream << "SET DEFAULT \"" << value << "\"";

            return stream.str(); 
        }
    };

    // namespace that consists of types
    namespace types
    {
        const char *boolean = "BOOL";
        const char *integer = "INT";
        const char *bigInt = "BIGINT";
        const char *smallInt = "SMALLINT";
        const char *tinyInt = "TINYINT";

        std::string floatType(size_t countIntegers, size_t precision)
        {
            return "FLOAT(" + std::to_string(countIntegers) + ',' + std::to_string(precision) + ')';
        }

        std::string doubleType(size_t countIntegers, size_t precision)
        {
            return "DOUBLE(" + std::to_string(countIntegers) + ',' + std::to_string(precision) + ')';
        }

        std::string decimal(size_t countIntegers, size_t precision)
        {
            return "DECIMAL(" + std::to_string(countIntegers) + ',' + std::to_string(precision) + ')';
        }

        std::string numeric(size_t countIntegers, size_t precision)
        {
            return "NUMERIC(" + std::to_string(countIntegers) + ',' + std::to_string(precision) + ')';
        }

        std::string charType(size_t size)
        {
            return "CHAR(" + std::to_string(size) + ')';
        }

        std::string varchar(size_t size)
        {
            return "VARCHAR(" + std::to_string(size) + ')';
        }

        const char *text = "TEXT";
        const char *mediumText = "MEDIUMTEXT";
        const char *longText = "LONGTEXT";
        const char *blob = "BLOB";
        const char *mediumBlob = "MEDIUMBLOB";
        const char *longBlob = "LONGBLOB";

        const char *date = "DATE";
        const char *time = "TIME";
        const char *datatime = "DATATIME";
        const char *timestamp = "TIMESTAMP";

        std::string year(size_t form)
        {
            return "YEAR(" + std::to_string(form) + ")";
        }
    };

    // create a pair that consists of field, value and comparison/assignment operator
    template <class fieldName, class valueType>
    std::string Pair(fieldName field, valueType value, std::string specSign = "=")
    {
        std::stringstream stream;

        stream << field << specSign << '\"' << value << '\"';

        return stream.str();
    }

    // forms SET
    template <class pairValue, class... pairList>
    std::string Set(pairValue value, pairList... args)
    {
        std::stringstream stream;

        stream << value << ", ";

        inputTypes(stream, "", ", ", args...);

        std::string result = stream.str();

        result[result.size() - 2] = ' ';

        return result;
    }

    // struct that forms and save values to insert them into table
    template <class fieldType, class... Args>
    struct Value
    {
        std::string result;

        Value(fieldType value, Args... args)
        {
            std::stringstream stream;

            stream << "(\"" << value << "\", ";

            inputTypes(stream, "\"", ", ", args...);

            result = stream.str();

            result[result.size() - 2] = ')';
        }
    };

    // struct that forms field for table
    template <class fieldName, class fieldType, class... Args>
    struct Field
    {
        std::string result;
        fieldName name;
        fieldType type;

        Field(fieldName name, fieldType type, Args... args)
        {
            std::stringstream stream;

            stream << name << ' ' << type << ' ';

            inputTypes(stream, "", " ", args...);

            result = stream.str();

            result[result.size() - 1] = ',';
            this->name = name;
            this->type = type;
        }
    };

    // struct that forms fields for values which will be inserted into table
    template <class fieldType, class... Args>
    struct FieldInput
    {
        std::string result;

        FieldInput(fieldType value, Args... args)
        {
            std::stringstream stream;

            stream << '(' << value << ", ";

            inputTypes(stream, "", ", ", args...);

            result = stream.str();

            result[result.size() - 2] = ')';
        }
    };

    // struct that forms fields for getting values from table
    template <class fieldName, class... Args>
    struct FieldOutput
    {
        std::string result;

        FieldOutput(fieldName value, Args... args)
        {
            std::stringstream stream;

            stream << "" << value << ", ";

            inputTypes(stream, "", ", ", args...);

            result = stream.str();

            result[result.size() - 2] = ' ';
        }
    };

    // class that save getted values from table
    class Result
    {
        int row = -1;
        std::vector<std::vector<std::string>> data;

    public:
        // get value by index
        std::string operator()(int i, int j)
        {
            return data.at(i).at(j);
        }

        // insert values into class Result
        friend void setResult(Result *result, std::string data, bool next);

        // output all values
        friend std::ostream &operator << (std::ostream &stream, Result &result);
    };

    // insert values into class Result
    // this friend function will be changed to anonymous
    void setResult(Result *result, std::string data, bool next = false)
    {
        if (next)
        {
            result->data.push_back(std::vector<std::string>{data});

            result->row++;

            return;
        }

        result->data[result->row].push_back(data);
    }

    // output all values
    std::ostream & operator<<(std::ostream &stream, Result &result)
    {
        for (auto it : result.data)
        {
            for (auto itt : it)
                stream << itt << '\t';

            stream << '\n';
        }

        return stream;
    }

    // class that is a table
    class Table
    {
        std::string tableName;
        std::map<std::string, std::string> fieldsInTable;
        std::vector<std::string> values;
        std::vector<std::string> constraints;

        // forms tables from fields
        void createTable(std::stringstream &stream) {}

        template <class fieldName, class fieldType, class... Args, class... Fields>
        void createTable
        (
            std::stringstream &stream,
            Field<fieldName, fieldType, Args...> f,
            Fields... fields
        );

        // insert tables into table
        void insertValues(std::stringstream &stream) {}

        template <class fieldType, class... Args, class... ValueList>
        void insertValues
        (
            std::stringstream &stream,
            Value<fieldType, Args...> v,
            ValueList... args
        );

        // delete valuess from tables
        void deleteValues(std::stringstream &stream) {}

        template <class String, class... Args>
        void deleteValues
        (
            std::stringstream &stream,
            String condition,
            Args... args
        );

    public:
        // create table
        template <class fieldName, class fieldType, class... Args, class... Fields>
        Table
        (
            std::string tableName,
            Field<fieldName, fieldType, Args...> f,
            Fields... fields
        );

        // get values from table
        template <class fieldName, class... Args, class... conditionList>
        Result selectValues
        (
            FieldOutput<fieldName, Args...> f,
            conditionList... args
        );

        // insert values into table
        template <class fieldType, class... Args, class... ValueList>
        void insertValues
        (
            FieldInput<fieldType, Args...> f,
            ValueList... valueList
        );

        // delete values
        template <class String, class... Args>
        void deleteValues
        (
            String condition,
            Args... args
        );

        // update values
        template <class forUpdate, class... conditionList>
        void updateValues
        (
            forUpdate values, 
            conditionList... args
        );

        // insert constraint into table
        void addConstraint(std::string nameConstraint, std::string arg);

        // drop table
        void dropTable();

        // reference 2 tables
        template<class... Attrs>
        friend void referenceTables(Table &mainTable, std::string mainField, Table &refTable, std::string refField, std::string nameConstr, Attrs... attrs);
    };

    // reference 2 tables
    template <class... Attrs>
    void referenceTables(Table &mainTable, std::string mainField, Table &refTable, std::string refField, std::string nameConstr, Attrs... attrs)
    {
        auto f1 = mainTable.fieldsInTable.find(mainField);
        auto f2 = refTable.fieldsInTable.find(refField);

        if (f1 ==  mainTable.fieldsInTable.end())
            throw std::invalid_argument("\n" + mainTable.tableName + " hasn`t " + mainField);

        if (f2 == refTable.fieldsInTable.end())
            throw std::invalid_argument("\n" + refTable.tableName + " hasn`t " + refField);

        std::stringstream stream;

        stream << "ALTER TABLE " << refTable.tableName << " ADD" 
               << " CONSTRAINT " << nameConstr
               << " FOREIGN KEY (" << refField << ") REFERENCES " 
               << mainTable.tableName << "(" << mainField << ") ";

        inputTypes(stream, "", " ", attrs...);

        if (mysql_query(&mysql, stream.str().c_str()))
            throw std::invalid_argument(mysql_error(&mysql));

        refTable.constraints.push_back(nameConstr);
    }

    // connect to database
    void connectToDatabase
    (
        const char *host,
        const char *username,
        const char *password,
        const char *dataname,
        int port = 8000,
        const char *usocks = NULL,
        unsigned long int flags = 0
    )
    {
        if (!mysql_init(&mysql))
            throw std::logic_error(mysql_error(&mysql));

        if (!mysql_real_connect(&mysql, host, username, password, dataname, port, usocks, flags))
            throw std::invalid_argument(mysql_error(&mysql));
    }

    // close connection
    void shutdownConnection()
    {
        mysql_close(&mysql);
    }
};

// ----------------
// methods of class Table

template <class fieldName, class fieldType, class... Args, class... Fields>
void SQLWrap::Table::createTable(
    std::stringstream &stream,
    Field<fieldName, fieldType, Args...> f,
    Fields... fields)
{
    stream << f.result << ' ';
    fieldsInTable.emplace(f.name, f.result);

    createTable(stream, fields...);
}

template <class fieldType, class... Args, class... ValueList>
void SQLWrap::Table::insertValues(
    std::stringstream &stream,
    Value<fieldType, Args...> v,
    ValueList... args)
{
    values.push_back(v.result);

    insertValues(stream, args...);
}

template <class String, class... Args>
void SQLWrap::Table::deleteValues(
    std::stringstream &stream,
    String condition,
    Args... args)
{
    stream << condition << ' ';

    deleteValues(stream, args...);
}

template <class forUpdate, class... conditionList>
void SQLWrap::Table::updateValues(
    forUpdate values,
    conditionList... args)
{
    std::stringstream stream;

    stream << "UPDATE " << tableName << " SET " << values << " WHERE ";

    inputTypes(stream, "", ", ", args...);

    std::string command = stream.str();

    command[command.size() - 2] = ' ';

    if (mysql_query(&mysql, command.c_str()))
        throw std::invalid_argument(mysql_error(&mysql));
}

template <class fieldName, class fieldType, class... Args, class... Fields>
SQLWrap::Table::Table(
    std::string tableName,
    Field<fieldName, fieldType, Args...> f,
    Fields... fields)
{
    std::stringstream stream;

    this->tableName = tableName;

    stream << "CREATE TABLE IF NOT EXISTS " << tableName << '(' << f.result << ' ';
    fieldsInTable.emplace(f.name, f.result);

    createTable(stream, fields...);

    std::string command = stream.str();

    command.resize(command.size() - 1);
    command[command.size() - 1] = ')';

    if (mysql_query(&mysql, command.c_str()))
        throw std::invalid_argument(mysql_error(&mysql));
}

template <class fieldName, class... Args, class... conditionList>
SQLWrap::Result SQLWrap::Table::selectValues(
    FieldOutput<fieldName, Args...> f,
    conditionList... args)
{
    std::stringstream stream;

    stream << "SELECT " << f.result << "FROM " << tableName;

    if (checkTemplate(args...))
    {
        stream << " WHERE ";

        inputTypes(stream, "", " ", args...);
    }

    if (mysql_query(&mysql, stream.str().c_str()))
        throw std::invalid_argument(mysql_error(&mysql));

    if (!(res = mysql_store_result(&mysql)))
        throw std::invalid_argument(mysql_error(&mysql));

    unsigned int i = 0;
    Result *result = new Result;

    while ((row = mysql_fetch_row(res)))
    {
        setResult(result, row[i++], true);

        for (; i < mysql_num_fields(res); i++)
            setResult(result, row[i]);

        i = 0;
    }

    mysql_free_result(res);

    res = nullptr;

    return *result;
}

template <class fieldType, class... Args, class... ValueList>
void SQLWrap::Table::insertValues(
    FieldInput<fieldType, Args...> f,
    ValueList... valueList)
{
    std::stringstream stream;

    stream << f.result;

    insertValues(stream, valueList...);

    for (auto it : values)
    {
        std::string command = "INSERT INTO " + tableName + stream.str() + " VALUES " + it;

        if (mysql_query(&mysql, command.c_str()))
            throw std::invalid_argument(mysql_error(&mysql));
    }

    values.clear();
}

template <class String, class... Args>
void SQLWrap::Table::deleteValues(
    String condition,
    Args... args)
{
    std::stringstream stream;

    stream << "DELETE FROM " << tableName << " WHERE " << condition << ' ';

    deleteValues(stream, args...);

    if (mysql_query(&mysql, stream.str().c_str()))
        throw std::invalid_argument(mysql_error(&mysql));
}

void SQLWrap::Table::addConstraint(std::string nameConstraint, std::string arg)
{
    std::stringstream stream;

    stream << "ALTER TABLE " << tableName << " ADD CONSTRAINT "
           << nameConstraint << ' ' << arg;

    if (mysql_query(&mysql, stream.str().c_str()))
        throw std::invalid_argument(mysql_error(&mysql));

    constraints.push_back(nameConstraint);
}

void SQLWrap::Table::dropTable()
{
    std::string command = "DROP TABLE " + tableName;

    if (mysql_query(&mysql, command.c_str()))
        throw std::invalid_argument(mysql_error(&mysql));
}

#endif // SQLWRAPPER_HPP
