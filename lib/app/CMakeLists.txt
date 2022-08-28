set(Sources
    models/model.cpp
    models/register.cpp
    models/Book.cpp
    models/History.cpp
    models/Member.cpp
    controller/Library.cpp
    controller/Database.cpp
)

set(Headers
    models/model.h
    models/register.h
    models/Book.h
    models/History.h
    models/Member.h
    controller/Library.h
    controller/Database.h
    constants.h
)

add_library(
    AppLib STATIC
    ${Sources}
    ${Headers}
)

if (NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()

target_link_libraries(AppLib PRIVATE spdlog::spdlog)
target_link_libraries(AppLib PUBLIC sqlite)
target_include_directories(AppLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})