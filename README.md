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


To start work we have to connect to database, that`s why we have to call function `connectToDatabase`. We must enter server, username, password and name of database.


We must initialize class `Table` to create tavle and insert name of table and fields.



To insert values we have to call method `insertValue` and enter structure `FieldInput` and structures `Values` which consits of values in a certain oreder (under rach field).


To delete values we have to call method `deleteValues` and enter `Pair` (if you want to unite it you can enter attributes `And`, `Or`).


To update values we have to call method `updateValues` and enter `Set` to contribute new values and condition where we want to update.


To get values we have to call `selectValues` and enter `FieldOutput` (to get all - `FieldOutput("*")) and to specify fields which we want ot get. Also we can enter condition.


Also we can to specify constraints and we must call `addConstraint` and to enter name of it and attribute. For example, to enter check we have to enter `Check(Pair(types))`.


Call `dropTable` to drop/delete table.


The all methods and function is are recursive and templated.

