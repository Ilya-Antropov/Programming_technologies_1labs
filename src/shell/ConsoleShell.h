#pragma once

#include <QObject>
#include <memory>
#include "../monitor/FileMonitor.h"
#include "../monitor/PollWorker.h"
#include "../notifier/INotifier.h"

class ConsoleShell : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(ConsoleShell);

public:
    explicit ConsoleShell(FileMonitor* monitor,
                          INotifier* notifier,
                          int pollIntervalMs,
                          QObject* parent = nullptr);

    ~ConsoleShell() override = default;

private:
    FileMonitor* m_monitor;
    INotifier* m_notifier;
    std::unique_ptr<PollWorker> m_pollWorker;
    int m_pollIntervalMs;
};