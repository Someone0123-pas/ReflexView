#ifndef DEBUG_QT_H
#define DEBUG_QT_H

#include <QString>

namespace qtdebug {
void print_qrc_directories();
void check_qrc_file(const QString& filepath);
}  // namespace qtdebug

#endif  // DEBUG_QT_H
