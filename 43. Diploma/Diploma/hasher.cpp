#include "hasher.h"
#include <QFile>
#include <QCryptographicHash>

QString sha256File(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return {};
    QCryptographicHash h(QCryptographicHash::Sha256);
    const qint64 chunk = 1 << 16;
    while (!f.atEnd()) h.addData(f.read(chunk));
    return QString::fromLatin1(h.result().toHex());
}
