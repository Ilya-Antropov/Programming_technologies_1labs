set(SOURCES
        main.cpp
        config/AppConfig.cpp
        logger/AppLogger.cpp
        notifier/ConsoleNotifier.cpp
        checker/ExistenceWatcher.cpp
        checker/SizeWatcher.cpp
)

set(HEADERS
        config/AppConfig.h
        logger/AppLogger.h
        checker/IntFileChecker.h
        notifier/IntNotifier.h
        notifier/ConsoleNotifier.h
        checker/ExistenceWatcher.h
        checker/SizeWatcher.h
        checker/RestorationWatcher.h
)

add_executable(programing_1labs ${SOURCES} ${HEADERS})

target_include_directories(programing_1labs PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(programing_1labs PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
)

find_package(Threads REQUIRED)
target_link_libraries(programing_1labs PRIVATE Threads::Threads)
