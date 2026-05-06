#include <QCoreApplication>
#include <QTimer>
#include <QFile>
#include "logger/AppLogger.h"
#include "config/AppConfig.h"
#include "checker/ExistenceWatcher.h"
#include "notifier/ConsoleNotifier.h"

// накикунл вариант, чтобы проверить (можно не смотреть)
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    auto& logger = AppLogger::instance();
    logger.setLevel(LogLevel::DEBUG_LVL);
    logger.info("Программа запущена");

    auto& config = AppConfig::instance();
    config.load("config.ini");
    logger.debug("Интервал опроса: " + QString::number(config.pollIntervalMs()));

    ExistenceWatcher watcher;
    ConsoleNotifier notifier;

    const QStringList watchedFiles = {"test.txt", "test2.txt"};

    for (const QString& path : watchedFiles) {
        QFile file(path);
        if (!file.exists()) {
            logger.warning("Файл " + path + " не существует, создаём пустой");
            file.open(QIODevice::WriteOnly);
            file.close();
        }
    }

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        for (const QString& path : watchedFiles) {
            CheckResult result = watcher.check(path);
            if (result.event != CheckEvent::NONE) {
                notifier.notify(result);
            }
        }
    });
    timer.start(config.pollIntervalMs());

    QTimer::singleShot(20000, &app, &QCoreApplication::quit);
    logger.info("Наблюдение за test.txt и test2.txt начато.");
    return app.exec();
}