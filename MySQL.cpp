#include "SQLite.h"
#include <stdarg.h>
#include <stdlib.h>

#define condfree(a) if (a != NULL) free(a);

MySQL::MySQL()
{
    this->db = (MYSQL *)NULL;
    this->host = NULL;
    this->user = NULL;
    this->pass = NULL;
    this->port = 3306;
    this->result = NULL;
}

MySQL::~MySQL()
{
    this->close();
    condfree(this->host);
    condfree(this->user);
    condfree(this->pass);
}

bool MySQL::open(char *i_host, char *i_user, char *i_pass, char *i_db, unsigned int i_port)
{
    if (this->db != NULL)
        return false; //no duplicate connections
    condfree(this->host);
    condfree(this->user);
    condfree(this->pass);
    this->host = (char *)malloc(strlen(i_host) + 1);
    this->user = (char *)malloc(strlen(i_user) + 1);
    this->pass = (char *)malloc(strlen(i_pass) + 1);
    this->port = i_port == 0 ? 3306 : i_port;
    strcpy(this->host, i_host);
    strcpy(this->user, i_user);
    strcpy(this->pass, i_pass);
    this->db = mysql_init(NULL);
    if (mysql_real_connect(this->db, this->host, this->user, this->pass, i_db, this->port, NULL, 0) == NULL)
    {
        mysql_close(this->db);
        this->db = NULL;
        return false;
    }
    return true;
}

void MySQL::close()
{
    if (this->db == NULL)
        return;
    mysql_close(this->db);
    this->db = NULL;
}

bool MySQL::query(const char *i_fmt, ...)
{
    char *str = new char[2048];
    bool ret;
    va_list args;
    va_start(args, i_fmt);
    vsprintf(str, i_fmt, args);
    va_end(args);
    ret = this->query(std::string(str));
    delete str;
    return ret;
}

bool MySQL::statement(const char *i_fmt, ...)
{
    char *str = new char[2048];
    bool ret;
    va_list args;
    va_start(args, i_fmt);
    vsprintf(str, i_fmt, args);
    va_end(args);
    ret = this->statement(std::string(str));
    delete str;
    return ret;
}

SQLiteRow MySQL::operator[](int i_index)
{
    return *this->result->rows->at(i_index);
}

bool MySQL::query(std::string i_query)
{
    const char *query = i_query.c_str();
    bool rval = false;
    MYSQL_RES *result;
    MYSQL_FIELD *fields;
    MYSQL_ROW row;
    SQLiteRow *s_row;
    SQLiteColumn *s_col;
    if (mysql_query(this->db, query) == 0)
    {
        result = mysql_store_result(this->db);
        if (result != (MYSQL_RES *)NULL)
        {
            //allocate the result
            if (this->result != (SQLiteResult *)NULL)
                delete this->result;
            this->result = new SQLiteResult();
            //parse column names
            fields = mysql_fetch_fields(result);
            this->result->nCols = mysql_field_count(this->db);
            for (unsigned int i = 0; i < this->result->nCols; i++)
                this->result->colNames->push_back(std::string(fields[i].name));
            //parse rows
            while ((row = mysql_fetch_row(result)) != (MYSQL_ROW)NULL)
            {
                this->result->nRows++;
                s_row = new SQLiteRow();
                s_row->colNames = this->result->colNames;
                for (unsigned int i = 0; i < this->result->nCols; i++)
                {
                    s_col = new SQLiteColumn;
                    s_col->data = (unsigned char *)malloc(strlen(row[i]) + 1);
                    strcpy((char *)s_col->data, row[i]);
                    s_row->cols->push_back(s_col);
                }
                this->result->rows->push_back(s_row);
            }
            rval = true;
        }
    }
    return rval;
}

bool MySQL::statement(std::string i_query)
{
    const char *query = i_query.c_str();
    mysql_query(this->db, query) == 0 ? true : false;
}

#undef condfree
