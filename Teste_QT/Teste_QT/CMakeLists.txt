cmake_minimum_required(VERSION 3.5)

project(Teste_UI_API LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES ../../UI_API.cpp ../../UI_API.h)

add_library(mylib ../../UI_API.cpp ../../UI_API.h)
target_include_directories(mylib PRIVATE ${YOUR_DIRECTORY})

add_executable(Teste_UI_API
               tst_teste_ui_api.cpp
               ../../UI_API.cpp
           )
add_test(NAME Teste_UI_API COMMAND Teste_UI_API)

target_link_libraries(Teste_UI_API PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Test)

