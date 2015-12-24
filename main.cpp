#include "SQLite.h"
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <sstream>
#include <string.h>

using std::cout;
using std::endl;

int main()
{
    srand(time(NULL));
    SQLite q;
    if (q.open("test.db3")) //if database exists, open it
    {
        if (q.query("SELECT COUNT(*) FROM tmp;")) //check row count
            if (q[0][0].integer() > 10)
                q.statement("DELETE FROM tmp where 1=1;"); //truncate if more than 10 rows
        q.statement("INSERT INTO tmp (a, b) VALUES(%d,%d);",
                    rand() % 128 + 1,
                    rand() % 128 + 1);
        if (q.query("SELECT a, b FROM tmp ORDER BY a")) //select all rows
        {
            cout << "a\tb" << endl;
            for (int i = 0; i < q.count(); i++)
                cout << q[i]["a"].integer() << "\t" << q[i]["b"].integer() << endl; //print each row
        }
    }
    else //if it does not exist, create it
    {
        q.open_create("test.db3");
        cout << q.statement("CREATE TABLE tmp(a int, b int);"); //create our table
    }
    q.close();
}
