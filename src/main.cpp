#include "AppLogger.h"
#include <QCoreApplication>
#include <thread>
#include <vector>

// сделал чисто для проверки(можно не смотреть)
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    AppLogger& logger = AppLogger::instance();

    logger.setLevel(LogLevel::DEBUG_LVL);
    logger.debug("Отладка: инициализация");
    logger.info("Информация: старт");
    logger.warning("Предупреждение: что-то не так");
    logger.error("Ошибка: сбой");

    logger.setLevel(LogLevel::INFO);
    logger.debug("Эта строка не появится (уровень ниже INFO)");
    logger.info("Эта строка появится (INFO)");
    logger.warning("Предупреждение выводится всегда, если >= INFO");
    logger.error("Ошибка выводится");

    return 0;
}