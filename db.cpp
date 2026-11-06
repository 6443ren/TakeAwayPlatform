#include "db.hpp"
#include <stdexcept>
#include <iostream>

const char* DB_HOST = "127.0.0.1";
const char* DB_USER = "root";
const char* DB_PWD = "123456";
const char* DB_NAME = "TakeAwayDatabase";

DBConn& DBConn::getInstance() {
    static DBConn instance;
    return instance;
}

DBConn::DBConn() {
    driver = get_driver_instance();
}

DBConn::~DBConn() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& pair : connMap) {
        if (pair.second) {
            pair.second->close();
            delete pair.second;
        }
    }
}

sql::Connection* DBConn::createConn() {
    sql::Connection* c = driver->connect(DB_HOST, DB_USER, DB_PWD);
    c->setSchema(DB_NAME);
    c->setAutoCommit(true);
    sql::Statement* stmt = c->createStatement();
    stmt->execute("SET NAMES utf8mb4");
    stmt->execute("SET CHARACTER SET utf8mb4");
    delete stmt;
    return c;
}

sql::Connection* DBConn::getConn() {
    std::thread::id tid = std::this_thread::get_id();
    std::lock_guard<std::mutex> lock(mutex);
    
    auto it = connMap.find(tid);
    if (it != connMap.end()) {
        if (it->second && !it->second->isClosed()) {
            return it->second;
        }
        if (it->second) {
            delete it->second;
        }
    }
    
    sql::Connection* c = createConn();
    connMap[tid] = c;
    return c;
}

void DBConn::close() {
    std::thread::id tid = std::this_thread::get_id();
    std::lock_guard<std::mutex> lock(mutex);
    
    auto it = connMap.find(tid);
    if (it != connMap.end() && it->second) {
        it->second->close();
        delete it->second;
        connMap.erase(it);
    }
}

void DBConn::begin() {
    getConn()->setAutoCommit(false);
}

void DBConn::commit() {
    getConn()->commit();
    getConn()->setAutoCommit(true);
}

void DBConn::rollback() {
    getConn()->rollback();
    getConn()->setAutoCommit(true);
}

int executeUpdate(const std::string& sql) {
    auto conn = DBConn::getInstance().getConn();
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    int affected = stmt->executeUpdate(sql);
    return affected;
}

std::vector<std::vector<std::string>> query(const std::string& sql) {
    std::vector<std::vector<std::string>> result;
    auto conn = DBConn::getInstance().getConn();
    std::unique_ptr<sql::Statement> stmt(conn->createStatement());
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery(sql));

    sql::ResultSetMetaData* meta = rs->getMetaData();
    int colCnt = meta->getColumnCount();

    while (rs->next()) {
        std::vector<std::string> row;
        for (int i = 1; i <= colCnt; ++i) {
            if (rs->isNull(i)) {
                row.push_back("");
            } else {
                row.push_back(rs->getString(i));
            }
        }
        result.push_back(row);
    }
    return result;
}