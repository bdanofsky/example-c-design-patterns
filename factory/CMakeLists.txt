cmake_minimum_required(VERSION 3.4)
project(c++-utilities
	DESCRIPTION "various c++17 utilities to speed design -> code"
	LANGUAGES CXX
	)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	option(PACKAGE_TESTS "Simple tests for various c++ utilities" ON)
endif()

if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
