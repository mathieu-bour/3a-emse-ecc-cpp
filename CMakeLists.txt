cmake_minimum_required(VERSION 3.12)
project(3a_ecc_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(3a_ecc_cpp
        main.cpp
        src/ecc/UnsignedBigInteger.h
        src/ecc/ModularBigInteger.h
        src/ecc/BigInteger.h
        src/ecc/ECCTypes.h
        )

add_executable(3a_ecc_cpp_tests
        tests/ecc/BigIntegerTest.cpp
        tests/ecc/UnsignedBigIntegerTest.cpp
        tests/ecc/ModularBigInteger.cpp
        )
target_link_libraries(3a_ecc_cpp_tests gtest gtest_main pthread)
