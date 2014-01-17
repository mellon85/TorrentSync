cmake_minimum_required(VERSION 2.8)
project (TorrentSync)

include(CheckCXXCompilerFlag)
SET(CMAKE_BUILD_TYPE Debug)

enable_testing()

# message(STATUS "${CMAKE_BUILD_TYPE}")
# message(STATUS "${CMAKE_CXX_COMPILER_ID}")

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

    check_cxx_compiler_flag(-std=gnu++11 HAS_Cxx11)
    if (HAS_Cxx11)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAS_Cxx11 -std=gnu++11")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} -DHAS_Cxx11 -std=gnu++11")
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR})


set(DHT
    torrentsync/dht/AddressData.cpp
    torrentsync/dht/Address.cpp 
    torrentsync/dht/RoutingTable.cpp
    torrentsync/dht/Node.cpp
    torrentsync/dht/message/Message.cpp
    torrentsync/dht/message/Ping.cpp
    torrentsync/dht/AddressTree.cpp )
set(DHT_UT ${DHT}
    test/torrentsync/dht/AddressData.cpp
    test/torrentsync/dht/Address.cpp
    test/torrentsync/dht/Node.cpp
    test/torrentsync/dht/RoutingTable.cpp
    test/torrentsync/dht/AddressBucket.cpp
    test/torrentsync/dht/AddressTree.cpp
    test/torrentsync/dht/message/BEncodeEncoder.cpp)

# threads
find_package(Threads)
set(COMMON_LIBS ${CMAKE_THREAD_LIBS_INIT})

# boost
find_package(Boost
    COMPONENTS
    serialization
    thread
    system
    unit_test_framework)

if (NOT ${Boost_FOUND})
    message(FATAL_ERROR "Boost not found")
endif()
set(COMMON_BOOST_LIBS ${Boost_SERIALIZATION_LIBRARIES} ${Boost_THREAD_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

# main executable
add_executable(TorrentSync
    torrentsync/main.cpp ${DHT})

# unit tests
add_executable(unittest
    test/main.cpp ${DHT_UT})

add_test(NAME unit_test
         WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/tests
         COMMAND unittest)

# add linking
target_link_libraries(TorrentSync ${COMMON_BOOST_LIBS} ${COMMON_LIBS})
target_link_libraries(unittest ${COMMON_BOOST_LIBS} ${COMMON_LIBS} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})

