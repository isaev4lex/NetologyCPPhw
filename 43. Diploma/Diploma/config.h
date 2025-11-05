#pragma once
#include <QString>
#include <QStringList>

struct DbConfig {
    QString host = "127.0.0.1";
    int     port = 5432;
    QString name = "searchdb";
    QString user = "search_user";
    QString password = "secret";
};

struct IndexerConfig {
    QStringList roots;
    QStringList extensions;  // "txt","md",...
    int minTokenLen = 3;
    int maxTokenLen = 32;
    int maxFileMB   = 5;
};

struct SearchConfig {
    int limit = 20;
};

struct Config {
    DbConfig db;
    IndexerConfig indexer;
    SearchConfig search;

    static Config load(const QString& iniPath);
};
