cmake_minimum_required(VERSION 3.13)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-std=c++14 -Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(
        include
)

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp src/factory.cpp src/reports.cpp src/simulation.cpp)

add_executable(NetSim main.cpp include/package.hpp include/types.hpp include/storage_types.hpp include/helpers.hpp include/nodes.hpp include/config.hpp include/global_functions_mock.hpp include/nodes_mocks.hpp
        src/package.cpp src/storage_types.cpp src/nodes.cpp src/helpers.cpp include/factory.hpp src/factory.cpp include/reports.hpp src/reports.cpp include/simulation.hpp src/simulation.cpp)

target_compile_definitions(NetSim PUBLIC EXERCISE_ID=EXERCISE_ID_NODES)

# == Unit testing using Google Testing Framework ==

# Ustaw zmienną `SOURCES_FILES_TESTS`, która będzie przechowywać ścieżki do
# plików źródłowych z testami jednostkowymi.
set(SOURCES_FILES_TESTS
        test/test_package.cpp test/test_package_2.cpp test/test_storage_types.cpp test/test_Factory.cpp test/test_simulate.cpp)

# Dodaj konfigurację typu `Test`.
add_executable(NetSim_test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(NetSim_test PUBLIC
        googletest-master/googletest/include
        mocks
        )

# Dodaj podkatalog `googletest-master`.
add_subdirectory(googletest-master googletest-master)

# Podlinkuj bibliotekę o identyfikatorze `gmock` (w pliku CMake) wyłącznie do konkretnej
# konfiguracji (tu: `Test`).
target_link_libraries(NetSim_test gmock)