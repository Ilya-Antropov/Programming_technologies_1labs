#include "ConsoleShell.h"
#include <QTextStream>

ConsoleShell::ConsoleShell(FileMonitor* monitor,
                           INotifier* notifier,
                           int pollMs,
                           QObject* parent)
    : QObject(parent)
      , m_monitor(monitor)
      , m_notifier(notifier)
      , m_pollIntervalMs(pollMs) {
    QObject::connect(m_monitor, &FileMonitor::watchListBecameEmpty,
                     this, [this]() {
                         if (m_pollWorker && m_pollWorker->isRunning()) {
                             m_pollWorker->stop();
                             QTextStream out(stdout);
                             out << "\n  [!] Список пуст - мониторинг остановлен.\n";
                             out.flush();
                         }
                     });
}