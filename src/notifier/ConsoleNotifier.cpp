#include "ConsoleNotifier.h"
#include <QTextStream>

void ConsoleNotifier::notify(const CheckResult& result) {
    const QString msg = formatMessage(result);
    if (msg.isEmpty()) { return; }

    QTextStream out(stdout);
    out << msg << '\n';
    out.flush();
}

// тут будет formatMessage