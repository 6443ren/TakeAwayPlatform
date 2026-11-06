cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(TakeAwayPlatform)

include_directories(./)
include_directories(/usr/include/mysql)
include_directories(/usr/include/mysqlcppconn)

file(GLOB COMMON_SRC
    db.cpp
    util.cpp
    json_util.cpp
    service/user_service.cpp
    service/dish_service.cpp
    service/order_service.cpp
    service/merchant_service.cpp
)

add_executable(server
    main.cpp
    ${COMMON_SRC}
)

add_executable(http_server
    http_server.cpp
    http_handler.cpp
    ${COMMON_SRC}
)

target_link_libraries(server
    mysqlcppconn
    ssl
    crypto
    z
    uuid
    pthread
)

target_link_libraries(http_server
    mysqlcppconn
    ssl
    crypto
    z
    uuid
    pthread
)