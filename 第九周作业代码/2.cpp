#include <iostream>
#include <sqlite3.h>
#include <string>

class SQLiteDB {
private:
    sqlite3* db;
    
public:
    SQLiteDB(const std::string& dbName) {
        if(sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("无法打开数据库: " + std::string(sqlite3_errmsg(db)));
        }
    }
    
    ~SQLiteDB() {
        sqlite3_close(db);
    }
    
    void execute(const std::string& sql) {
        char* errMsg = 0;
        if(sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
            std::string error = errMsg;
            sqlite3_free(errMsg);
            throw std::runtime_error("SQL错误: " + error);
        }
    }
    
    void query(const std::string& sql, int (*callback)(void*,int,char**,char**), void* data = 0) {
        char* errMsg = 0;
        if(sqlite3_exec(db, sql.c_str(), callback, data, &errMsg) != SQLITE_OK) {
            std::string error = errMsg;
            sqlite3_free(errMsg);
            throw std::runtime_error("SQL错误: " + error);
        }
    }
};

// 使用示例
int main() {
    try {
        SQLiteDB db("example.db");
        
        // 创建表
        db.execute("CREATE TABLE IF NOT EXISTS PRODUCTS("
                  "ID INT PRIMARY KEY NOT NULL,"
                  "NAME TEXT NOT NULL,"
                  "PRICE REAL);");
        
        // 插入数据
        db.execute("INSERT INTO PRODUCTS VALUES(1, 'Laptop', 999.99);");
        db.execute("INSERT INTO PRODUCTS VALUES(2, 'Phone', 699.99);");
        
        // 查询数据
        std::cout << "产品列表:\n";
        db.query("SELECT * FROM PRODUCTS", [](void*, int argc, char** argv, char** colName) {
            for(int i = 0; i < argc; i++) {
                std::cout << colName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
            }
            std::cout << "--------\n";
            return 0;
        });
        
    } catch(const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}