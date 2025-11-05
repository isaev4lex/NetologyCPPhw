#include <QCoreApplication>
#include <QDirIterator>
#include <QFileInfo>
#include <QTextStream>
#include <QSet>
#include <QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "config.h"
#include "db.h"
#include "textnormalizer.h"
#include "hasher.h"

static bool beginTx() {
    auto db = QSqlDatabase::database("pg");
    if (!db.transaction()) { qCritical() << "BEGIN failed:" << db.lastError().text(); return false; }
    return true;
}
static bool commitTx() {
    auto db = QSqlDatabase::database("pg");
    if (!db.commit()) { qCritical() << "COMMIT failed:" << db.lastError().text(); return false; }
    return true;
}

static bool getOrInsertWordId(const QString& token, int& idOut) {
    auto db = QSqlDatabase::database("pg");
    {   QSqlQuery ins(db);
        ins.prepare("INSERT INTO words(token) VALUES ($1) ON CONFLICT (token) DO NOTHING RETURNING id;");
        ins.addBindValue(token);
        if (!ins.exec()) { qCritical() << "words insert:" << ins.lastError().text(); return false; }
        if (ins.next()) { idOut = ins.value(0).toInt(); return true; }
    }
    {   QSqlQuery sel(db);
        sel.prepare("SELECT id FROM words WHERE token=$1;");
        sel.addBindValue(token);
        if (!sel.exec()) { qCritical() << "words select:" << sel.lastError().text(); return false; }
        if (sel.next()) { idOut = sel.value(0).toInt(); return true; }
        qCritical() << "word id not found after insert/select race"; return false;
    }
}

static bool upsertDocument(const QFileInfo& fi, const QString& sha, int& docId) {
    auto db = QSqlDatabase::database("pg");
    QSqlQuery q(db);
    q.prepare(
        "INSERT INTO documents(path, title, size_bytes, mtime, sha256) "
        "VALUES ($1,$2,$3,$4,$5) "
        "ON CONFLICT (path) DO UPDATE SET "
        "  size_bytes=EXCLUDED.size_bytes, mtime=EXCLUDED.mtime, sha256=EXCLUDED.sha256, indexed_at=NOW() "
        "RETURNING id;"
        );
    q.addBindValue(fi.absoluteFilePath());
    q.addBindValue(fi.fileName());
    q.addBindValue(static_cast<qlonglong>(fi.size()));
    q.addBindValue(QDateTime::fromSecsSinceEpoch(fi.lastModified().toSecsSinceEpoch()));
    q.addBindValue(sha);

    if (!q.exec() || !q.next()) { qCritical() << "documents upsert:" << q.lastError().text(); return false; }
    docId = q.value(0).toInt();
    return true;
}

static bool upsertDocWord(int docId, int wordId, int freq) {
    auto db = QSqlDatabase::database("pg");
    QSqlQuery q(db);
    q.prepare("INSERT INTO doc_words(doc_id, word_id, freq) VALUES ($1,$2,$3) "
              "ON CONFLICT (doc_id, word_id) DO UPDATE SET freq=EXCLUDED.freq;");
    q.addBindValue(docId);
    q.addBindValue(wordId);
    q.addBindValue(freq);
    if (!q.exec()) { qCritical() << "doc_words upsert:" << q.lastError().text(); return false; }
    return true;
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    const QString ini = (argc > 1) ? QString::fromLocal8Bit(argv[1]) : QStringLiteral("config.ini");
    Config cfg;
    try {
        cfg = Config::load(ini);
        qInfo().noquote() << "ini used:" << QFileInfo(ini).absoluteFilePath();
        qInfo().noquote() << "roots:" << cfg.indexer.roots.join("; ");
        qInfo().noquote() << "exts:"  << cfg.indexer.extensions.join(", ");
    } catch (const std::exception& e) { qCritical().noquote() << "Config error:" << e.what(); return 1; }

    QString err;
    if (!Db::open(cfg.db, &err)) { qCritical().noquote() << err; return 2; }
    if (!Db::ensureSchema(&err)) { qCritical().noquote() << err; return 3; }

    const qint64 maxBytes = static_cast<qint64>(cfg.indexer.maxFileMB) * 1024 * 1024;

    QSet<QString> exts;
    for (const auto& e : cfg.indexer.extensions) exts.insert(e.trimmed().toLower());

    int indexed = 0;

    for (const auto& root : cfg.indexer.roots) {
        qInfo().noquote() << "scanning:" << root;
        if (root.isEmpty()) continue;
        QDirIterator it(root, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString path = it.next();
            qInfo().noquote() << "found file:" << path;
            QFileInfo fi(path);
            const QString suffix = fi.suffix().toLower();
            if (!exts.isEmpty() && !exts.contains(suffix)) {
                qInfo().noquote() << "skip by ext:" << suffix << "for" << path;
                continue;
            }
            if (fi.size() > maxBytes) continue;

            QFile f(path);
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

            QTextStream ts(&f);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
            ts.setCodec("UTF-8");
#endif
            const QString text = ts.readAll();

            const auto tokens = tokenize(text, cfg.indexer.minTokenLen, cfg.indexer.maxTokenLen);
            if (tokens.isEmpty()) continue;
            const auto freq = countFreq(tokens);
            const QString sha = sha256File(path);

            if (!beginTx()) return 4;

            int docId = -1;
            if (!upsertDocument(fi, sha, docId)) { QSqlDatabase::database("pg").rollback(); return 5; }

            bool ok = true;
            for (auto itf = freq.constBegin(); itf != freq.constEnd(); ++itf) {
                int wid = -1;
                if (!getOrInsertWordId(itf.key(), wid)) { ok = false; break; }
                if (!upsertDocWord(docId, wid, itf.value())) { ok = false; break; }
            }

            if (ok) { if (!commitTx()) return 6; if (++indexed % 20 == 0) qInfo() << "Indexed:" << indexed; }
            else {
                QSqlDatabase::database("pg").rollback();
                qCritical() << "Rolled back for file:" << path;
            }
        }
    }
    qInfo() << "Done. Indexed files:" << indexed;
    return 0;
}
