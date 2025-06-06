#include <iostream>
#include <sqlite3.h>
#include <string>

// 回调函数，用于处理查询结果
static int callback(void* data, int argc, char** argv, char** colName) {
    for(int i = 0; i < argc; i++) {
        std::cout << colName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << "-----------------\n";
    return 0;
}

int main() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;
    
    // 1. 打开/创建数据库
    rc = sqlite3_open("example.db", &db);
    if(rc) {
        std::cerr << "无法打开数据库: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    } else {
        std::cout << "成功打开数据库\n";
    }

    // 2. 创建表
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS EMPLOYEE("
        "ID INT PRIMARY KEY NOT NULL,"
        "NAME TEXT NOT NULL,"
        "AGE INT NOT NULL,"
        "SALARY REAL);";
    
    rc = sqlite3_exec(db, createTableSQL, callback, 0, &errMsg);
    if(rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "表创建成功\n";
    }

    // 3. 插入数据
    const char* insertSQL = 
        "INSERT INTO EMPLOYEE (ID, NAME, AGE, SALARY) "
        "VALUES (1, 'John', 28, 50000.00);"
        "INSERT INTO EMPLOYEE (ID, NAME, AGE, SALARY) "
        "VALUES (2, 'Alice', 32, 65000.00);";
    
    rc = sqlite3_exec(db, insertSQL, callback, 0, &errMsg);
    if(rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "记录插入成功\n";
    }

    // 4. 查询数据
    const char* selectSQL = "SELECT * from EMPLOYEE";
    std::cout << "查询结果:\n";
    rc = sqlite3_exec(db, selectSQL, callback, 0, &errMsg);
    if(rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // 5. 更新数据
    const char* updateSQL = "UPDATE EMPLOYEE set SALARY = 55000.00 where ID=1";
    rc = sqlite3_exec(db, updateSQL, callback, 0, &errMsg);
    if(rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "记录更新成功\n";
    }

    // 再次查询以显示更新后的结果
    std::cout << "更新后的查询结果:\n";
    rc = sqlite3_exec(db, selectSQL, callback, 0, &errMsg);

    // 6. 关闭数据库
    sqlite3_close(db);
    return 0;
}