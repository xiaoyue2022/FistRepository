#include <iostream>
#include <sqlite3.h>
#include <vector>

int main() {
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    // 打开数据库
    if(sqlite3_open("example.db", &db) != SQLITE_OK) {
        std::cerr << "无法打开数据库\n";
        return 1;
    }
    
    // 准备参数化插入语句
    const char* sql = "INSERT INTO EMPLOYEE (ID, NAME, AGE, SALARY) VALUES (?, ?, ?, ?)";
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "准备语句失败\n";
        return 1;
    }
    
    // 绑定参数
    std::vector<std::vector<std::string>> employees = {
        {"3", "Bob", "35", "45000"},
        {"4", "Eve", "28", "52000"}
    };
    
    for(auto& emp : employees) {
        sqlite3_bind_int(stmt, 1, std::stoi(emp[0]));       // ID
        sqlite3_bind_text(stmt, 2, emp[1].c_str(), -1, 0);  // NAME
        sqlite3_bind_int(stmt, 3, std::stoi(emp[2]));       // AGE
        sqlite3_bind_double(stmt, 4, std::stod(emp[3]));    // SALARY
        
        if(sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "执行失败\n";
        }
        
        sqlite3_reset(stmt);  // 重置语句以用于下一次执行
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}