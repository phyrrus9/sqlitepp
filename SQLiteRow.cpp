#include "SQLite.h"

SQLiteRow::SQLiteRow()
{
    this->cols = new std::vector<SQLiteColumn*>;
}

SQLiteRow::~SQLiteRow()
{
    this->cols->clear();
    this->cols = NULL;
}

SQLiteColumn SQLiteRow::operator[](int i_index)
{
    return *this->cols->at(i_index);
}
SQLiteColumn SQLiteRow::operator[](std::string i_name)
{
    return(this->operator[](this->columnIndexByName(i_name)));
}
int SQLiteRow::columnIndexByName(std::string i_name)
{
    for (int i = 0; i < this->cols->size(); i++)
    {
        if (!i_name.compare(this->colNames->at(i)))
            return i;
    }
    return -1;
}

