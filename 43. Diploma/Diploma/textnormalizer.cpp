#include "textnormalizer.h"
#include <QRegularExpression>

QStringList tokenize(const QString& textIn, int minLen, int maxLen) {
    QString t = textIn.toLower();
    static const QRegularExpression rx(QStringLiteral("[^\\p{L}\\p{N}]+"));
    t = t.replace(rx, " ");
    const auto parts = t.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    QStringList out; out.reserve(parts.size());
    for (const auto& p : parts) if (p.size() >= minLen && p.size() <= maxLen) out << p;
    return out;
}

QHash<QString,int> countFreq(const QStringList& tokens) {
    QHash<QString,int> f;
    for (const auto& tok : tokens) f[tok] += 1;
    return f;
}
