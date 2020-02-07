cmake_minimum_required(VERSION 3.12)
project(3a_ecc_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(3a_ecc_cpp
        includes/ecc/ECCTypes.h
        includes/ecc/UnsignedBigInteger.h
        includes/ecc/SignedBigInteger.h
        includes/ecc/ModularBigInteger.h
        src/ecc/ModularBigInteger.cpp
        src/ecc/SignedBigInteger.cpp
        src/ecc/UnsignedBigInteger.cpp
        main.cpp src/ecc/Montgomery.cpp includes/ecc/Montgomery.h)

add_executable(3a_ecc_cpp_tests
        includes/ecc/ECCTypes.h
        includes/ecc/UnsignedBigInteger.h
        includes/ecc/SignedBigInteger.h
        includes/ecc/ModularBigInteger.h
        includes/ecc/Montgomery.h
        includes/ecc/Point.h
        includes/ecc/P256.h
        src/ecc/UnsignedBigInteger.cpp
        src/ecc/SignedBigInteger.cpp
        src/ecc/ModularBigInteger.cpp
        src/ecc/Montgomery.cpp
        src/ecc/Point.cpp
        src/ecc/P256.cpp
        tests/ecc/UnsignedBigIntegerTest.cpp
        tests/ecc/SignedBigIntegerTest.cpp
        tests/ecc/ModularBigIntegerTest.cpp
        tests/ecc/MontgomeryTest.cpp)
target_link_libraries(3a_ecc_cpp_tests gtest gtest_main pthread)
