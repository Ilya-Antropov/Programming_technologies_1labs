#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include "logger/AppLogger.h"
#include "config/AppConfig.h"
#include "checker/ExistenceWatcher.h"
#include "checker/SizeWatcher.h"
#include "checker/RestorationWatcher.h"
#include "notifier/ConsoleNotifier.h"
#include "monitor/FileMonitor.h"
#include "monitor/PollWorker.h"

// реализация для проверки чисто (можно не смотреть)
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    auto& logger = AppLogger::instance();
    logger.setLevel(LogLevel::DEBUG_LVL);
    logger.info("Программа запущена");

    auto& config = AppConfig::instance();
    config.load("config.ini");
    int pollInterval = config.pollIntervalMs();
    logger.debug("Интервал опроса: " + QString::number(config.pollIntervalMs()));

    FileMonitor monitor;

    monitor.addChecker(std::make_unique<ExistenceWatcher>());
    monitor.addChecker(std::make_unique<SizeWatcher>());
    monitor.addChecker(std::make_unique<RestorationWatcher>());

    ConsoleNotifier notifier;
    monitor.setNotifier(&notifier);

    const QStringList watchedFiles = {"test.txt", "test2.txt"};
    for (const QString& path : watchedFiles) {
        QFile file(path);
        if (!file.exists()) {
            logger.warning("Файл " + path + " не существует, создаём пустой");
            if (!file.open(QIODevice::WriteOnly)) {
                logger.error("Не удалось создать файл " + path);
                return 1;
            }
            file.close();
        }
        if (!monitor.addFile(path)) {
            logger.error("Не удалось добавить файл в мониторинг: " + path);
        }
    }

    QObject::connect(&monitor, &FileMonitor::watchListBecameEmpty,
                     [&]() {
                         logger.info("Список файлов пуст, завершаем работу");
                         QCoreApplication::quit();
                     });

    PollWorker worker([&]() { monitor.pollOnce(); }, pollInterval);
    worker.start();
    logger.info("Мониторинг запущен через FileMonitor и PollWorker");

    QTimer::singleShot(60000, &app, &QCoreApplication::quit);
    int ret = app.exec();
    worker.stop();

    return ret;
}