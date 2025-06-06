//SQLiteC++封装库
#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

int main() {
    try {
        // 打开数据库(如果不存在则创建)
        SQLite::Database db("example.db", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);
        
        // 创建表
        db.exec("CREATE TABLE IF NOT EXISTS CUSTOMERS ("
               "ID INTEGER PRIMARY KEY,"
               "NAME TEXT NOT NULL,"
               "EMAIL TEXT UNIQUE)");
        
        // 插入数据
        SQLite::Statement insert(db, "INSERT INTO CUSTOMERS VALUES (?, ?, ?)");
        insert.bind(1, 101);
        insert.bind(2, "John Smith");
        insert.bind(3, "john@example.com");
        insert.exec();
        
        // 查询数据
        SQLite::Statement query(db, "SELECT * FROM CUSTOMERS");
        while(query.executeStep()) {
            std::cout << "ID: " << query.getColumn(0) 
                      << ", Name: " << query.getColumn(1) 
                      << ", Email: " << query.getColumn(2) << "\n";
        }
        
    } catch(const std::exception& e) {
        std::cerr << "SQLite异常: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}