cmake_minimum_required(VERSION 3.14)
project(market-data-dashboard)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(RDKAFKA REQUIRED IMPORTED_TARGET rdkafka++)
pkg_check_modules(PostgreSQL REQUIRED libpq)

find_package(nlohmann_json REQUIRED)


include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PostgreSQL_INCLUDE_DIRS}
)

set(SOURCES
    src/main.cpp
    src/KafkaConsumer.cpp
    src/DatabaseWriter.cpp
    src/DashboardWindow.cpp
)

set(HEADERS
    include/KafkaConsumer.h
    include/DatabaseWriter.h
    include/DashboardWindow.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    PkgConfig::RDKAFKA
    ${PostgreSQL_LIBRARIES}
    nlohmann_json::nlohmann_json
)

set(CMAKE_AUTOMOC ON)
