#include "ConsoleNotifier.h"
#include <QTextStream>

void ConsoleNotifier::notify(const CheckResult& result) {
    const QString msg = formatMessage(result);
    if (msg.isEmpty()) { return; }

    QTextStream out(stdout);
    out << msg << '\n';
    out.flush();
}

QString ConsoleNotifier::formatMessage(const CheckResult& result) {
    switch (result.event) {
        case CheckEvent::NONE:
            return {};

        case CheckEvent::FILE_EXISTS:
            return QStringLiteral("  [+] Файл существует: '%1'  (%2 байт)")
                .arg(result.path).arg(result.size);

        case CheckEvent::FILE_CHANGED:
            return QStringLiteral("  [~] Файл изменён: '%1'  %2 -> %3 байт")
                .arg(result.path).arg(result.oldSize).arg(result.size);

        case CheckEvent::FILE_DELETED:
            return QStringLiteral("  [-] Файл удалён: '%1'")
                .arg(result.path);

        case CheckEvent::FILE_RESTORED:
            return QStringLiteral("  [*] Файл восстановлен: '%1'  (%2 байт)")
                .arg(result.path).arg(result.size);
    }
    return {};
}