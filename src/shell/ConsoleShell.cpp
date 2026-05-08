#include <QString>
#include <QTextStream>
#include "ConsoleShell.h"

ConsoleShell::ConsoleShell(FileMonitor* monitor,
                           IntNotifier* notifier,
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

void ConsoleShell::run() {
    printHelp();

    QTextStream in(stdin);
    QTextStream out(stdout);

    while (true) {
        out << "\n> ";
        out.flush();

        const QString line = in.readLine().trimmed();
        if (line.isNull()) { break; }
        if (line.isEmpty()) { continue; }

        const int spaceIdx = line.indexOf(' ');
        const QString cmd = (spaceIdx >= 0)
                                ? line.left(spaceIdx).toLower()
                                : line.toLower();
        const QString arg = (spaceIdx >= 0)
                                ? line.mid(spaceIdx + 1).trimmed()
                                : QString{};

        // команды реализую чуть позже
        if (cmd == QStringLiteral("add")) { handleAdd(arg); } else if (
            cmd == QStringLiteral("remove")) { handleRemove(arg); } else if (
            cmd == QStringLiteral("list")) { handleList(); } else if (
            cmd == QStringLiteral("start")) { handleStart(); } else if (cmd == QStringLiteral("stop")) {
            handleStop();
        } else if (cmd == QStringLiteral("quit")
                   || cmd == QStringLiteral("exit")) {
            handleStop();
            out << "  Мы закончили\n";
            out.flush();
            break;
        } else {
            out << "  Неизвестная команда -> Введите 'help'\n";
            out.flush();
        }
    }
}

void ConsoleShell::handleAdd(const QString& path) {
    QTextStream out(stdout);

    if (path.isEmpty()) {
        out << "  Ошибка: укажите путь\n";
        out.flush();
        return;
    }

    QFileInfo info(path);
    if (info.exists() && !info.isFile()) {
        out << QStringLiteral(
            "  Ошибка: '%1' является директорией\n").arg(path);
        out.flush();
        return;
    }

    if (m_monitor->addFile(path)) {
        out << QStringLiteral("  Добавлен: '%1'\n").arg(path);
    } else {
        out << QStringLiteral("  Не удалось добавить: '%1'\n").arg(path);
    }
    out.flush();
}

void ConsoleShell::handleRemove(const QString& path) {
    QTextStream out(stdout);
    if (path.isEmpty()) {
        out << "  Ошибка: укажите путь\n";
        out.flush();
        return;
    }
    if (m_monitor->removeFile(path)) {
        out << QStringLiteral("  Удалён: '%1'\n").arg(path);
    } else {
        out << QStringLiteral("  Не найден: '%1'\n").arg(path);
    }
    out.flush();
}

void ConsoleShell::handleList() {
    QTextStream out(stdout);
    const QVector<QString> files = m_monitor->watchedFiles();

    if (files.isEmpty()) {
        out << "  Список мониторинга пуст\n";
    } else {
        out << QStringLiteral("  Файлов: %1\n").arg(files.size());
        for (const QString& f : files) {
            out << QStringLiteral("    - %1\n").arg(f);
        }
    }
    out.flush();
}

void ConsoleShell::handleStart(){
    QTextStream out(stdout);

    if (m_monitor->isEmpty()) {
        out << "  Нет файлов. Добавьте командой 'add'\n";
        out.flush();
        return;
    }

    if (m_pollWorker && m_pollWorker->isRunning()) {
        out << "  Мониторинг уже запущен\n";
        out.flush();
        return;
    }

    m_pollWorker = std::make_unique<PollWorker>(
        [this]() { m_monitor->pollOnce(); },
        m_pollIntervalMs);
    m_pollWorker->start();

    out << "  Мониторинг запущен\n"; out.flush();
}