cmake_minimum_required(VERSION 2.8)
project (TorrentSync)

include(CheckCXXCompilerFlag)
SET(TURTLE_PATH "${PROJECT_SOURCE_DIR}/turtle/")
SET(CMAKE_BUILD_TYPE Debug)

enable_testing()

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )

    
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                            OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_LESS 4.7 )
        message(FATAL_ERROR "gcc must be version >= 4.7")
    endif()


    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=gnu++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

    # check for code coverage unit test
    if (COVERAGE)
        message("-- Coverage is ON")
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS_DEBUG} -fno-inline-small-functions -fdce -fno-default-inline -fno-elide-constructors -fno-inline -fprofile-arcs -ftest-coverage ")
        set(CMAKE_EXE_LINKER_FLAGS
            "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
        set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    torrentsync/utils/log/Log.cpp
    torrentsync/utils/log/Logger.cpp
    torrentsync/utils/log/LogStream.cpp
    torrentsync/dht/AddressData.cpp
    torrentsync/dht/Address.cpp 
    torrentsync/dht/RoutingTable.cpp
    torrentsync/dht/Node.cpp
    torrentsync/dht/message/Message.cpp
    torrentsync/dht/message/Ping.cpp
    torrentsync/dht/AddressTree.cpp
    torrentsync/dht/message/BEncodeDecoder.cpp
    torrentsync/utils/RandomGenerator.cpp)
set(SOURCES_UT
    test/torrentsync/utils/log/Log.cpp
    test/torrentsync/utils/Buffer.cpp
    test/torrentsync/dht/AddressData.cpp
    test/torrentsync/dht/Address.cpp
    test/torrentsync/dht/Node.cpp
    test/torrentsync/dht/RoutingTable.cpp
    test/torrentsync/dht/AddressBucket.cpp
    test/torrentsync/dht/AddressTree.cpp
    test/torrentsync/dht/message/Ping.cpp
    test/torrentsync/dht/message/BEncodeEncoder.cpp
    test/torrentsync/dht/message/BEncodeDecoder.cpp)

# threads
find_package(Threads)
set(COMMON_LIBS ${CMAKE_THREAD_LIBS_INIT})

# common source code lib
add_library(TorrentSync SHARED ${SOURCES})

# boost
find_package(Boost
    COMPONENTS
    serialization
    thread
    system
    date_time
    regex
    unit_test_framework)

if (NOT ${Boost_FOUND})
    message(FATAL_ERROR "Boost not found")
endif()

set(COMMON_BOOST_LIBS
    ${Boost_SERIALIZATION_LIBRARIES}
    ${Boost_THREAD_LIBRARIES}
    ${Boost_SYSTEM_LIBRARIES}
    ${Boost_DATE_TIME_LIBRARIES}
    ${Boost_REGEX_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

# main executable
add_executable(TorrentSyncExecutable
    torrentsync/main.cpp)

set_target_properties(
    TorrentSyncExecutable
    PROPERTIES
    OUTPUT_NAME "TorrentSync")

# Turtle configuration
include_directories("${TURTLE_PATH}/include")

add_executable(unittest
    test/main.cpp ${SOURCES_UT})

add_test(NAME unit_test
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
         COMMAND unittest)

# add linking
target_link_libraries(TorrentSyncExecutable
    TorrentSync
    ${COMMON_BOOST_LIBS}
    ${COMMON_LIBS}
    )
target_link_libraries(unittest
    TorrentSync
    ${COMMON_BOOST_LIBS}
    ${COMMON_LIBS}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES}
    )

