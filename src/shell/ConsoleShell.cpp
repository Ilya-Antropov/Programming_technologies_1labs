#include <QString>
#include <QFileInfo>
#include <QTextStream>
#include <cstdio>
#include "ConsoleShell.h"

static void printUtf8(const QString& s) {
    const QByteArray bytes = s.toUtf8();
    fwrite(bytes.constData(), 1, bytes.size(), stdout);
    fflush(stdout);
}

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
                             printUtf8(QStringLiteral(
                                 "\n  [!] Список пуст - мониторинг остановлен.\n"));
                         }
                     });
}

void ConsoleShell::run() {
    printHelp();

    QTextStream in(stdin);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    in.setCodec("UTF-8");
#else
    in.setEncoding(QStringConverter::Utf8);
#endif

    while (true) {
        printf("\n> ");
        fflush(stdout);

        QString line = in.readLine();
        if (line.isNull()) { break; }

        line = line.trimmed().remove(QChar('\r')).remove(QChar('\0'));
        if (line.isEmpty()) { continue; }

        const int spaceIdx = line.indexOf(' ');
        const QString cmd = (spaceIdx >= 0)
                                ? line.left(spaceIdx).toLower()
                                : line.toLower();
        const QString arg = (spaceIdx >= 0)
                                ? line.mid(spaceIdx + 1).trimmed()
                                : QString{};

        if (cmd == QStringLiteral("add")) {
            handleAdd(arg);
        } else if (cmd == QStringLiteral("remove")) {
            handleRemove(arg);
        } else if (cmd == QStringLiteral("list")) {
            handleList();
        } else if (cmd == QStringLiteral("start")) {
            handleStart();
        } else if (cmd == QStringLiteral("stop")) {
            handleStop();
        } else if (cmd == QStringLiteral("help")
                   || cmd == QStringLiteral("?")) {
            printHelp();
        } else if (cmd == QStringLiteral("quit")
                   || cmd == QStringLiteral("exit")) {
            handleStop();
            printUtf8(QStringLiteral("  Мы закончили\n"));
            break;
        } else {
            printUtf8(QStringLiteral(
                "  Неизвестная команда -> Введите 'help'\n"));
        }
    }
}

void ConsoleShell::handleAdd(const QString& path) {
    if (path.isEmpty()) {
        printUtf8(QStringLiteral("  Ошибка: укажите путь\n"));
        return;
    }

    QFileInfo info(path);

    if (info.exists() && !info.isFile()) {
        printUtf8(QStringLiteral(
            "  Ошибка: '%1' является директорией\n").arg(path));
        return;
    }

    if (!info.exists()) {
        printUtf8(QStringLiteral(
            "  Ошибка: файл '%1' не существует\n").arg(path));
        return;
    }

    if (m_monitor->addFile(path)) {
        printUtf8(QStringLiteral("  Добавлен: '%1'\n").arg(path));
    } else {
        printUtf8(QStringLiteral("  Не удалось добавить: '%1'\n").arg(path));
    }
}

void ConsoleShell::handleRemove(const QString& path) {
    if (path.isEmpty()) {
        printUtf8(QStringLiteral("  Ошибка: укажите путь\n"));
        return;
    }
    if (m_monitor->removeFile(path)) {
        printUtf8(QStringLiteral("  Удалён: '%1'\n").arg(path));
    } else {
        printUtf8(QStringLiteral("  Не найден: '%1'\n").arg(path));
    }
}

void ConsoleShell::handleList() {
    const QVector<QString> files = m_monitor->watchedFiles();

    if (files.isEmpty()) {
        printUtf8(QStringLiteral("  Список мониторинга пуст\n"));
    } else {
        printUtf8(QStringLiteral("  Файлов: %1\n").arg(files.size()));
        for (const QString& f : files) {
            printUtf8(QStringLiteral("    - %1\n").arg(f));
        }
    }
}

void ConsoleShell::handleStart() {
    if (m_monitor->isEmpty()) {
        printUtf8(QStringLiteral(
            "  Нет файлов. Добавьте командой 'add'\n"));
        return;
    }

    if (m_pollWorker && m_pollWorker->isRunning()) {
        printUtf8(QStringLiteral("  Мониторинг уже запущен\n"));
        return;
    }

    m_pollWorker = std::make_unique<PollWorker>(
        [this]() { m_monitor->pollOnce(); },
        m_pollIntervalMs);
    m_pollWorker->start();

    printUtf8(QStringLiteral("  Мониторинг запущен\n"));
}

void ConsoleShell::handleStop() {
    if (!m_pollWorker || !m_pollWorker->isRunning()) {
        printUtf8(QStringLiteral("  Мониторинг не запущен\n"));
        return;
    }

    m_pollWorker->stop();
    printUtf8(QStringLiteral("  Мониторинг остановлен\n"));
}

void ConsoleShell::printHelp() {
    printf("\n"
        "  ╔═══════════════════════════════════════╗\n"
        "  ║      FileMonitor — Команды            ║\n"
        "  ╠═══════════════════════════════════════╣\n"
        "  ║  help ?        — эта справка          ║\n"
        "  ║  add <путь>    — добавить файл        ║\n"
        "  ║  remove <путь> — удалить файл         ║\n"
        "  ║  list          — список файлов        ║\n"
        "  ║  start         — запустить опрос      ║\n"
        "  ║  stop          — остановить опрос     ║\n"
        "  ║  quit          — выход                ║\n"
        "  ╚═══════════════════════════════════════╝\n\n");
    fflush(stdout);
}