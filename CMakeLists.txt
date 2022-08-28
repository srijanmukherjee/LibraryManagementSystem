cmake_minimum_required(VERSION 3.10)

project(App VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (NOT TARGET spdlog)
    find_package(spdlog REQUIRED)
endif()

find_package(wxWidgets REQUIRED gl core base OPTIONAL_COMPONENTS net)
include(${wxWidgets_USE_FILE})

include_directories(lib/sqlite)
link_directories(lib/sqlite)
add_library(
    sqlite STATIC 
    lib/sqlite/sqlite3.c
    lib/sqlite/sqlite3.h)

configure_file(lib/AppConfig.h.in AppConfig.h)

add_subdirectory(lib/app)

set(Sources 
    src/main.cpp
    src/gui.cpp
    src/gui/mainwindow.cpp
    src/gui/SearchBookDialog.cpp
    src/gui/BookListCtrl.cpp
    src/gui/tabs/BookTabPage.cpp
    src/gui/tabs/LendTabPage.cpp
    src/gui/tabs/MemberTabPage.cpp
    src/gui/tabs/ReturnTabPage.cpp
    src/gui/components/dialog/AddBookDialog.cpp
    src/gui/components/dialog/SearchMemberDialog.cpp
    src/gui/components/dialog/AddMemberDialog.cpp
    src/gui/components/listctrl/MemberListCtrl.cpp
    src/gui/components/listctrl/ReturnTnxListCtrl.cpp)

add_executable(App ${Sources})

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC sqlite
    PRIVATE spdlog::spdlog
    PRIVATE ${wxWidgets_LIBRARIES})
    
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/lib/sqlite"
    "${PROJECT_SOURCE_DIR}/lib/app"
)

target_link_libraries(${PROJECT_NAME} LINK_PRIVATE AppLib)
    
###########################
#         Testing         #
###########################
enable_testing()
add_subdirectory(lib/googletest)
add_subdirectory(test)