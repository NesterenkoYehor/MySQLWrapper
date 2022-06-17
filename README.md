# MySQLWrapper
It`s a library which make it possible not to use SQL requests.
Insted this library gives functions, structures, classes and methods which can partially replace SQL

# How it works
This library dosen`t have strong typing and user can enter values which have different types.
Then it transforms to SQL requests and send it to MySQL. If it have some mistakes, program will send exception.



Library have namespaces `SQLWrap`, `attrs` and `types`. 



`attrs` have attributes and elemnets which use in SQL. `types` have types and `SQLWrap` consists of these namspaces, functions, methods, tructures and classes.



Structures `Field`, `FieldInput` and `FieldOutput` are fields of table.
Use `Field` to make fields in table. Use `FieldInput` to insert values in table and `FieldOutput` to output.



Use function `Pair` to specify field and value (also can add comparison operator).
Use function `Set` to update values.


We must initialize class `Table` to create tavle and insert name of table and fields.



To insert values we have to call method insertValue and enter structure `FieldInput` and structures `Values` which consits of values in a certain oreder (under rach field).




