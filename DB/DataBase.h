#pragma once

#include "../header.h"

class DataBase {

public:
    DataBase(const std::string &path) {
        if (sqlite3_open(path.c_str(), &db)) {
            throw std::runtime_error("[DB]: Error while opening db file!");
        }
    }

    ~DataBase() {
        sqlite3_close(db);
    }

    void createTable(const std::string &name) {
        char* messaggeError;
        sqlite3_exec(db, ("CREATE TABLE IF NOT EXISTS " + name + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, CODE TEXT UNIQUE, VAL TEXT UNIQUE);").c_str(), NULL, 0, &messaggeError);
    }

    void insertElement(const std::string &tableName, const std::string &code, const std::string &value) {
        std::string insertQuery = "INSERT INTO " + tableName + " (CODE, VAL) VALUES ('" + code + "', '" + value + "');";
        sqlite3_stmt *insertStmt;
        sqlite3_prepare(db, insertQuery.c_str(), insertQuery.size(), &insertStmt, NULL);
        if (sqlite3_step(insertStmt) != SQLITE_DONE)
            throw std::runtime_error("[DB]: Error while inserting!");
        sqlite3_finalize(insertStmt);
    }

    bool checkValueExists(const std::string &tableName, const std::string &value) {
        std::string selectQuery = "select * from " + tableName + " where VAL='" + value + "';";
        sqlite3_stmt *selectStmt;
        sqlite3_prepare(db, selectQuery.c_str(), selectQuery.size(), &selectStmt, NULL);
        int result = sqlite3_step(selectStmt);
        sqlite3_finalize(selectStmt);
        return result == SQLITE_ROW;
    }

    bool checkCodeExists(const std::string &tableName, const std::string &code) {
        std::string selectQuery = "select * from " + tableName + " where CODE='" + code + "';";
        sqlite3_stmt *selectStmt;
        sqlite3_prepare(db, selectQuery.c_str(), selectQuery.size(), &selectStmt, NULL);
        int result = sqlite3_step(selectStmt);
        sqlite3_finalize(selectStmt);
        return result == SQLITE_ROW;
    }

    int getNextId(const std::string &tableName) {
        std::string selectQuery = "SELECT seq FROM SQLITE_SEQUENCE WHERE name = '" + tableName + "' LIMIT 1;";
        sqlite3_stmt *selectStmt;
        sqlite3_prepare(db, selectQuery.c_str(), selectQuery.size(), &selectStmt, NULL);
        int result = sqlite3_step(selectStmt);
        sqlite3_finalize(selectStmt);
        return sqlite3_column_int(selectStmt, 0) + 1;
    }

    std::string getCode(const std::string &tableName, const std::string  &value) {
        std::string selectQuery = "select * from " + tableName + " where VAL='" + value + "';";
        sqlite3_stmt *selectStmt;
        sqlite3_prepare(db, selectQuery.c_str(), selectQuery.size(), &selectStmt, NULL);
        int result = sqlite3_step(selectStmt);
        if (result != SQLITE_ROW)
            return "";
        std::string code = (const char*)sqlite3_column_text(selectStmt, 1);
        sqlite3_finalize(selectStmt);
        return code;
    }



private:
    sqlite3 *db;
};
