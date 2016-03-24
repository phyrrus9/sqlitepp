sqlitepp.a: SQLite.o SQLiteColumn.o SQLiteRow.o sqlite3.o
	ar rcs sqlitepp.a SQLite.o SQLiteColumn.o SQLiteRow.o sqlite3.o
SQLite.o:
	g++ -c SQLite.cpp
SQLiteColumn.o:
	g++ -c SQLiteColumn.cpp
SQLiteRow.o:
	g++ -c SQLiteRow.cpp
sqlite3.o:
	gcc -c sqlite3/sqlite3.c
clean:
	rm -f *.o
	rm -f sqlitepp.a
