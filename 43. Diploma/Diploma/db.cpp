#include "db.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

static QString e2s(const QSqlError& e) { return e.text().isEmpty() ? "Unknown SQL error" : e.text(); }

bool Db::open(const DbConfig& cfg, QString* err) {
    if (QSqlDatabase::contains("pg")) {
        auto dbOld = QSqlDatabase::database("pg");
        if (dbOld.isOpen()) return true;
        dbOld.close();
        QSqlDatabase::removeDatabase("pg");
    }
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "pg");
    db.setHostName(cfg.host);
    db.setPort(cfg.port);
    db.setDatabaseName(cfg.name);
    db.setUserName(cfg.user);
    db.setPassword(cfg.password);

    if (!db.open()) { if (err) *err = "DB open failed: " + e2s(db.lastError()); return false; }
    return true;
}

bool Db::ensureSchema(QString* err) {
    QSqlDatabase db = QSqlDatabase::database("pg");
    if (!db.isOpen()) { if (err) *err = "DB is not open"; return false; }

    const char* stmts[] = {
        "CREATE TABLE IF NOT EXISTS documents ("
        "  id SERIAL PRIMARY KEY,"
        "  path TEXT UNIQUE NOT NULL,"
        "  title TEXT,"
        "  size_bytes BIGINT,"
        "  mtime TIMESTAMPTZ,"
        "  sha256 CHAR(64),"
        "  indexed_at TIMESTAMPTZ DEFAULT NOW());",

        "CREATE TABLE IF NOT EXISTS words ("
        "  id SERIAL PRIMARY KEY,"
        "  token TEXT UNIQUE NOT NULL);",

        "CREATE TABLE IF NOT EXISTS doc_words ("
        "  doc_id INT REFERENCES documents(id) ON DELETE CASCADE,"
        "  word_id INT REFERENCES words(id) ON DELETE CASCADE,"
        "  freq INT NOT NULL,"
        "  PRIMARY KEY (doc_id, word_id));",

        "CREATE INDEX IF NOT EXISTS idx_words_token ON words(token);",
        "CREATE INDEX IF NOT EXISTS idx_doc_words_wid ON doc_words(word_id);",
        "CREATE INDEX IF NOT EXISTS idx_doc_words_did ON doc_words(doc_id);",
        nullptr
    };

    for (int i=0; stmts[i]; ++i) {
        QSqlQuery q(db);
        if (!q.exec(stmts[i])) {
            if (err) *err = QString("Schema step failed: %1\n%2").arg(i).arg(e2s(q.lastError()));
            return false;
        }
    }
    return true;
}
