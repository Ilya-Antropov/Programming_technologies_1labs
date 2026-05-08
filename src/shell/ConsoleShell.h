#pragma once

#include <QObject>
#include <memory>
#include "../monitor/FileMonitor.h"
#include "../monitor/PollWorker.h"
#include "../notifier/IntNotifier.h"

class ConsoleShell : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(ConsoleShell);

public:
    explicit ConsoleShell(FileMonitor* monitor,
                          IntNotifier* notifier,
                          int pollIntervalMs,
                          QObject* parent = nullptr);

    ~ConsoleShell() override = default;

    void run();

private:
    void handleAdd(const QString& path);

    void handleRemove(const QString& path);

    void handleList();

    void handleStart();

    void handleStop();

    void printHelp();

    FileMonitor* m_monitor;
    IntNotifier* m_notifier;
    std::unique_ptr<PollWorker> m_pollWorker;
    int m_pollIntervalMs;
};