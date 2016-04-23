sqlitepp.a: SQLite.o SQLiteColumn.o SQLiteRow.o MySQL.o sqlite3.o
	ar rcs sqlitepp.a SQLite.o SQLiteColumn.o SQLiteRow.o MySQL.o sqlite3.o
SQLite.o: SQLite.cpp
	g++ -c SQLite.cpp
SQLiteColumn.o: SQLiteColumn.cpp
	g++ -c SQLiteColumn.cpp
SQLiteRow.o: SQLiteRow.cpp
	g++ -c SQLiteRow.cpp
MySQL.o: MySQL.cpp
	g++ -c MySQL.cpp
sqlite3.o:
	gcc -c sqlite3/sqlite3.c
clean:
	rm -f *.o
	rm -f sqlitepp.a
