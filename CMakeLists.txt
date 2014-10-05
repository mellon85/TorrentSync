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
        message(FATAL_ERROR "gcc must be version >= 4.7 but found ${GCC_VERSION}")
    endif()


    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

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
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++11")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    torrentsync/dht/Callback.cpp
    torrentsync/dht/Node.cpp 
    torrentsync/dht/NodeData.cpp
    torrentsync/dht/NodeTree.cpp
    torrentsync/dht/RoutingTable.cpp
    torrentsync/dht/RoutingTable_InitializeTable.cpp
    torrentsync/dht/RoutingTable_MessageHandlers.cpp
    torrentsync/dht/RoutingTable_RecvMessage.cpp
    torrentsync/dht/message/BEncodeDecoder.cpp
    torrentsync/dht/message/BEncodeEncoder.cpp
    torrentsync/dht/message/Constants.cpp
    torrentsync/dht/message/Message.cpp
    torrentsync/dht/message/Query.cpp
    torrentsync/dht/message/Reply.cpp
    torrentsync/dht/message/query/FindNode.cpp
    torrentsync/dht/message/query/GetPeers.cpp
    torrentsync/dht/message/query/Ping.cpp
    torrentsync/dht/message/reply/Error.cpp
    torrentsync/dht/message/reply/FindNode.cpp
    torrentsync/dht/message/reply/GetPeers.cpp
    torrentsync/dht/message/reply/Ping.cpp
    torrentsync/dht/TokenManager.cpp
    torrentsync/utils/Buffer.cpp
    torrentsync/utils/RandomGenerator.cpp
    torrentsync/utils/Spinlock.cpp
    torrentsync/utils/log/Log.cpp
    torrentsync/utils/log/LogStream.cpp
    torrentsync/utils/log/Logger.cpp
)
set(SOURCES_UT
    test/torrentsync/dht/Callback.cpp
    test/torrentsync/dht/Node.cpp
    test/torrentsync/dht/NodeBucket.cpp
    test/torrentsync/dht/NodeData.cpp
    test/torrentsync/dht/NodeTree.cpp
    test/torrentsync/dht/RoutingTable.cpp
    test/torrentsync/dht/message/BEncodeDecoder.cpp
    test/torrentsync/dht/message/BEncodeEncoder.cpp
    test/torrentsync/dht/message/query/FindNode.cpp
    test/torrentsync/dht/message/query/Ping.cpp
    test/torrentsync/dht/message/reply/Error.cpp
    test/torrentsync/dht/message/reply/FindNode.cpp
    test/torrentsync/dht/message/reply/GetPeers.cpp
    test/torrentsync/dht/message/reply/Ping.cpp
    test/torrentsync/utils/Buffer.cpp
    test/torrentsync/utils/log/Log.cpp
)

# threads
find_package(Threads)
set(COMMON_LIBS ${CMAKE_THREAD_LIBS_INIT})

# common source code lib
add_library(TorrentSync STATIC ${SOURCES})

# boost
find_package(Boost
    COMPONENTS
    serialization
    system
    date_time
    regex
    unit_test_framework)

if (NOT ${Boost_FOUND})
    message(FATAL_ERROR "Boost not found")
endif()

set(TEST_BOOST_LIBS
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES}
    ${Boost_REGEX_LIBRARIES})
set(COMMON_BOOST_LIBS
    ${Boost_SERIALIZATION_LIBRARIES}
    ${Boost_THREAD_LIBRARIES}
    ${Boost_SYSTEM_LIBRARIES}
    ${Boost_DATE_TIME_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

# main executable
add_executable(TorrentSyncExecutable
    torrentsync/App.cpp
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
    ${TEST_BOOST_LIBS}
    ${COMMON_LIBS}
    )

add_custom_target(doxygen doxygen doxygen.config)
