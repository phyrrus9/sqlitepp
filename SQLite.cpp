#include "SQLite.h"
#include <stdarg.h>

SQLite::SQLite()
{
    this->result = NULL;
    this->db = NULL;
    this->dbPath = NULL;
    this->dbOpened = false;
}

SQLite::~SQLite()
{
    if (this->dbOpened)
        this->close();
    this->db = NULL;
    this->dbOpened = false;
    delete this->result;
}

bool SQLite::open(std::string i_dbPath, int ip_flags)
{
    bool ret = false;
    if (dbOpened)
        return false;
    dbPath = strdup((char *)i_dbPath.c_str());
    ret = (sqlite3_open_v2(this->dbPath, &this->db, ip_flags, NULL)) == SQLITE_OK ? true : false;
    if (!ret)
        sqlite3_close(this->db);
    else
        this->dbOpened = true;
    return ret;
}

bool SQLite::open_create(std::string i_dbPath, int ip_flags)
{
    return this->open(i_dbPath, ip_flags | SQLITE_OPEN_CREATE);
}

void SQLite::close()
{
    if (!this->dbOpened)
        return;
    sqlite3_close(this->db);
    this->dbOpened = false;
    if (dbPath != NULL)
        free(dbPath);
    dbPath = NULL;
}

bool SQLite::query(std::string i_query)
{
    char * sql;
    sqlite3_stmt * stmt;
    SQLiteResult *ret = new SQLiteResult();
    if (this->result != NULL)
        delete this->result;
    this->result = ret;
    if (!this->dbOpened)
        return false;
    sql = (char *)i_query.c_str();
    sqlite3_prepare_v2(this->db, (const char *)sql, strlen(sql), &stmt, 0);
    while (1)
    {
        int s = sqlite3_step(stmt);
        if (s == SQLITE_ROW)
        {
            /*****get row info*****/
            if (ret->nCols == 0)
            {
                ret->nCols = sqlite3_column_count(stmt);
                int tmp_ncols = sqlite3_column_count(stmt);
                for (int tmp_i = 0; tmp_i < tmp_ncols; tmp_i++)
                {
                    std::string tmp_name = sqlite3_column_name(stmt, tmp_i);
                    ret->colNames->push_back(tmp_name);
                }
            }
            /*****get data*****/
            SQLiteRow *tmp_row = new SQLiteRow();
            tmp_row->colNames = ret->colNames; //pointer to parent array
            for (int tmp_i = 0; tmp_i < ret->nCols; tmp_i++)
            {
                SQLiteColumn *tmp_col = new SQLiteColumn;
                tmp_col->length = sqlite3_column_bytes(stmt, tmp_i);
                tmp_col->data = (unsigned char *)strdup((char *)sqlite3_column_text(stmt, tmp_i));
                tmp_row->cols->push_back(tmp_col);
            }
            ret->rows->push_back(tmp_row);
            ++ret->nRows;
        }
        else if (s == SQLITE_DONE)
        {
            ret->error = false;
            return !ret->error;
        }
        else return ret->error;
    }
}

bool SQLite::query(const char *i_fmt, ...)
{
    char *str = new char[2048];
    bool ret;
    va_list args;
    va_start(args, i_fmt);
    vsprintf(str, i_fmt, args);
    va_end(args);
    ret = SQLite::query(std::string(str));
    delete str;
    return ret;
}

bool SQLite::statement(std::string i_query)
{
    this->query(i_query);
    return !(this->result->error);
}

bool SQLite::statement(const char *i_fmt, ...)
{
    char *str = new char[2048];
    bool ret;
    va_list args;
    va_start(args, i_fmt);
    vsprintf(str, i_fmt, args);
    va_end(args);
    ret = SQLite::statement(std::string(str));
    delete str;
    return ret;
}

SQLiteRow SQLite::operator[](int i_index)
{
    return *this->result->rows->at(i_index);
}

SQLiteResult::SQLiteResult()
{
    this->colNames = new std::vector<std::string>;
    this->rows = new std::vector<SQLiteRow*>;
    this->error = true;
    this->nCols = 0;
    this->nRows = 0;
}

SQLiteResult::~SQLiteResult()
{
    this->colNames->clear();
    this->rows->clear();
    this->colNames = NULL;
    this->rows = NULL;
    this->error = true;
    this->nCols = 0;
    this->nRows = 0;
}
