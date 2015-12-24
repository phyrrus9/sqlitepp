#include "SQLite.h"

SQLiteColumn::SQLiteColumn()
{
    this->length = 0;
    this->data = NULL;
}

SQLiteColumn::~SQLiteColumn()
{
    free(this->data);
}

bool SQLiteColumn::isNull()
{
    return length == 0 ? true : false;
}

std::string SQLiteColumn::text()
{
    return std::string((char *)data);
}

int64_t SQLiteColumn::integer()
{
    return atoi((const char *)data);
}

double SQLiteColumn::real()
{
    double r;
    sscanf((const char *)this->data, "%lf", &r);
    return r;
}

void *SQLiteColumn::blob()
{
    return (void *)data;
}
