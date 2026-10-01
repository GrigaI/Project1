cmake_minimum_required(VERSION 3.20)
project(TodoApp CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Библиотека с логикой
add_library(todo_lib
    Task.cpp
    TaskManager.cpp
    Command.cpp
)

target_include_directories(todo_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Приложение
add_executable(todo_app main.cpp)
target_link_libraries(todo_app PRIVATE todo_lib)

# GoogleTest через FetchContent
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(todo_tests
    ../TodoTests/test_tasks.cpp
    ../TodoTests/test_taskmanager.cpp
    ../TodoTests/test_undo.cpp
    ../TodoTests/test_files.cpp
)

target_include_directories(todo_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(todo_tests PRIVATE todo_lib gtest_main)

include(GoogleTest)

