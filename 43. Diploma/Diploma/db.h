#pragma once
#include "config.h"
#include <QString>

namespace Db {
bool open(const DbConfig& cfg, QString* err = nullptr);
bool ensureSchema(QString* err = nullptr);
}
