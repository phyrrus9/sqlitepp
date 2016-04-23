#include <vector>
#include <string>
#include <stdint.h>
#include <stdio.h>
#include "sqlite3/sqlite3.h"
#include <mysql/mysql.h>
#include <stdlib.h>
#include <string.h>
#ifndef SQLITE_H_INCLUDED
#define SQLITE_H_INCLUDED

class SQLiteColumn
{
public:
    SQLiteColumn();
    ~SQLiteColumn();
    uint32_t length;
    unsigned char *data; //NULL when type is NULL, else pointer to values. TEXT and BLOB in uint8_t* format
    bool isNull();
    std::string text();
    int64_t integer();
    double real();
    void *blob();
};


class SQLiteRow
{
public:
    SQLiteRow();
    ~SQLiteRow();
    std::vector<SQLiteColumn*> *cols;
    std::vector<std::string> *colNames;
    SQLiteColumn operator[](int i_index);
    SQLiteColumn operator[](std::string i_name);
    int columnIndexByName(std::string i_name);
};

class SQLiteResult
{
public:
    SQLiteResult();
    ~SQLiteResult();
    std::vector<std::string> *colNames;
    std::vector<SQLiteRow*> *rows;
    uint32_t nCols;
    uint64_t nRows;
    bool error;
};

class SQLite
{
    sqlite3 * db;
    char * dbPath;
    bool dbOpened;
    SQLiteResult *result;
public:
    SQLite();
    ~SQLite();
    bool query(std::string i_query);
    bool query(const char *i_fmt, ...);
    bool statement(std::string i_query);
    bool statement(const char *i_fmt, ...);
    bool open(std::string i_dbPath, int ip_flags = SQLITE_OPEN_READWRITE);
    bool open_create(std::string i_dbPath, int ip_flags = SQLITE_OPEN_READWRITE);
    void close();
    bool error();
    SQLiteRow operator[](int i_index);
    int count() { return result->nRows; }
    SQLiteResult *getResult() { return result; }
};

class MySQL
{
    MYSQL * db;
    char * host;
    char * user;
    char * pass;
    unsigned short port;
    SQLiteResult *result;
public:
    MySQL();
    ~MySQL();
    bool query(std::string i_query);
    bool query(const char *i_fmt, ...);
    bool statement(std::string i_query);
    bool statement(const char *i_fmt, ...);
    bool open(char *i_host, char *i_user, char *i_pass, char *i_db = NULL, unsigned int i_port = 0);
    void close();
    SQLiteRow operator[](int i_index);
    int count() { return result->nRows; }
    SQLiteResult *getResult() { return result; }
};

#endif // SQLITE_H_INCLUDED
