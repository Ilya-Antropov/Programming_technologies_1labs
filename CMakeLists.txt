cmake_minimum_required(VERSION 3.16)
project(programing_1labs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(src/logger)

SET(SOURCES
        main.cpp
        src/logger/AppLogger.cpp
)

find_package(Qt5 COMPONENTS
        Core
        REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
        Core
        REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
)

