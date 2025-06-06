#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Callback function for query results
int rscallback(void *p, int argc, char **argv, char **argvv)
{
    int i;
    // Mark the table as non - empty
    *(int *)p = 0;
    for (i = 0; i < argc; i++) {
        printf("%s=%s ", argvv[i], argv[i]? argv[i] : "NULL");
    }
    // Bug fix: Use standard single - quote
    putchar('\n');
    return 0;
}

int main(void)
{
    sqlite3 *db;
    char *err = 0;
    int ret = 0;
    int empty = 1;

    // Open the database
    ret = sqlite3_open("./test.db", &db);
    if (ret != SQLITE_OK) {
        fputs(sqlite3_errmsg(db), stderr);
        fputs("\n", stderr);
        exit(1);
    }

    // Execute the query
    ret = sqlite3_exec(db, "select * from employee;", rscallback, &empty, &err);
    if (ret != SQLITE_OK) {
        // Bug fix: Print error message and free error string
        fputs(err, stderr);
        fputs("\n", stderr);
        sqlite3_free(err);
        sqlite3_close(db);
        exit(1);
    }

    // Check if the table is empty
    if (empty) {
        fputs("table employee is empty\n", stderr);
        sqlite3_close(db);
        exit(1);
    }

    // Close the database
    sqlite3_close(db);
    return 0;
}