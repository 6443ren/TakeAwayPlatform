#pragma once
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <mutex>
#include <map>

class DBConn {
public:
    static DBConn& getInstance();
    sql::Connection* getConn();
    void close();
    void begin();
    void commit();
    void rollback();

private:
    DBConn();
    ~DBConn();
    DBConn(const DBConn&) = delete;
    DBConn& operator=(const DBConn&) = delete;

    sql::Driver* driver;
    std::map<std::thread::id, sql::Connection*> connMap;
    std::mutex mutex;

    sql::Connection* createConn();
};

int executeUpdate(const std::string& sql);
std::vector<std::vector<std::string>> query(const std::string& sql);