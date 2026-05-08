#include <QCoreApplication>
#include "config/AppConfig.h"
#include "logger/AppLogger.h"
#include "checker/ExistenceWatcher.h"
#include "checker/SizeWatcher.h"
#include "checker/RestorationWatcher.h"
#include "notifier/ConsoleNotifier.h"
#include "monitor/FileMonitor.h"
#include "shell/ConsoleShell.h"
#include <clocale>

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, "en_US.UTF-8");
    
    QCoreApplication app(argc, argv);

    AppConfig::instance().load(QStringLiteral("config.ini"));

    const QString lvl = AppConfig::instance().logLevel();
    if (lvl == "DEBUG") AppLogger::instance().setLevel(LogLevel::DEBUG_LVL);
    else if (lvl == "WARNING") AppLogger::instance().setLevel(LogLevel::WARNING);
    else AppLogger::instance().setLevel(LogLevel::INFO);

    AppLogger::instance().info(QStringLiteral("=== FileMonitor v1.0 ==="));

    ConsoleNotifier notifier;

    FileMonitor monitor;
    monitor.addChecker(std::make_unique<ExistenceWatcher>());
    monitor.addChecker(std::make_unique<SizeWatcher>());
    monitor.addChecker(std::make_unique<RestorationWatcher>());
    monitor.setNotifier(&notifier);

    const int interval = AppConfig::instance().pollIntervalMs();
    ConsoleShell shell(&monitor, &notifier, interval);
    shell.run();

    return 0;
}