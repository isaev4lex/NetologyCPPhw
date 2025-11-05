#include "config.h"
#include <QSettings>
#include <QFileInfo>
#include <stdexcept>

static QStringList splitCsv(const QString& s) {
    auto parts = s.split(',', Qt::SkipEmptyParts);
    for (auto& p : parts) p = p.trimmed();
    return parts;
}
static QStringList splitRoots(const QString& s) {
    auto parts = s.split(';', Qt::SkipEmptyParts);
    for (auto& p : parts) p = p.trimmed();
    return parts;
}

Config Config::load(const QString& iniPath) {
    if (!QFileInfo::exists(iniPath))
        throw std::runtime_error(("config.ini not found: " + iniPath).toStdString());

    QSettings st(iniPath, QSettings::IniFormat);
    Config c;

    c.db.host = st.value("db/host", c.db.host).toString();
    c.db.port = st.value("db/port", c.db.port).toInt();
    c.db.name = st.value("db/name", c.db.name).toString();
    c.db.user = st.value("db/user", c.db.user).toString();
    c.db.password = st.value("db/password", c.db.password).toString();

    c.indexer.roots       = splitRoots(st.value("indexer/roots").toString());
    c.indexer.extensions  = splitCsv(st.value("indexer/extensions", "txt,md").toString());
    c.indexer.minTokenLen = st.value("indexer/min_token_len", c.indexer.minTokenLen).toInt();
    c.indexer.maxTokenLen = st.value("indexer/max_token_len", c.indexer.maxTokenLen).toInt();
    c.indexer.maxFileMB   = st.value("indexer/max_file_mb",   c.indexer.maxFileMB).toInt();

    c.search.limit = st.value("search/limit", c.search.limit).toInt();
    return c;
}
