cmake_minimum_required(VERSION 3.10)
project(Vector3DDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Создаем исполняемый файл
add_executable(Vector3DDemo main.cpp)

# Включаем поддержку C++17
target_compile_features(Vector3DDemo PRIVATE cxx_std_17)

# Для лучшей диагностики включаем предупреждения
if(MSVC)
    target_compile_options(Vector3DDemo PRIVATE /W4 /WX)
else()
    target_compile_options(Vector3DDemo PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()