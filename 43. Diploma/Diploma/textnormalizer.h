#pragma once
#include <QString>
#include <QStringList>
#include <QHash>

QStringList tokenize(const QString& text, int minLen, int maxLen);
QHash<QString,int> countFreq(const QStringList& tokens);
