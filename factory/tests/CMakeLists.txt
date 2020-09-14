cmake_minimum_required(VERSION 3.10)

#include(GoogleTest)
find_package(GTest REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.73.0 REQUIRED COMPONENTS program_options)

set(SOURCE runTests.cc)
add_executable(runTests ${SOURCE})
target_include_directories( runTests PUBLIC .. )
target_include_directories( runTests PUBLIC ${GTEST_INCLUDE_DIRS})
if(Boost_FOUND)
	target_include_directories( runTests PUBLIC ${Boost_INCLUDE_DIRS})
	target_link_libraries(runTests Boost::program_options)
else()
	message(FATAL_ERROR "cannot find boost library")
endif()
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)
set_target_properties(runTests PROPERTIES CXX_STANDARD 17)

