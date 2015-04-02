cmake_minimum_required(VERSION 2.8.4)
project(rococo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_check_modules(APR REQUIRED apr-1)

IF(APPLE)
ELSE()
pkg_check_modules(PYTHON2 REQUIRED python2)
ENDIF()

subdirs(rrr)

link_directories(
    ${PYTHON2_LIBRARY_DIRS}
    ${APR_LIBRARY_DIRS}
    )

include_directories(
    ${PYTHON2_INCLUDE_DIRS}
    ${APR_INCLUDE_DIRS}
    .
    rrr
    memdb
    deptran
    bench)

add_library(ROCOCO
    memdb/blob.h
    memdb/locking.h
    memdb/MurmurHash3.cc
    memdb/MurmurHash3.h
    memdb/row.cc
    memdb/row.h
    memdb/schema.cc
    memdb/schema.h
    memdb/snapshot.h
    memdb/table.cc
    memdb/table.h
    memdb/txn.cc
    memdb/txn.h
    memdb/utils.cc
    memdb/utils.h
    memdb/value.cc
    memdb/value.h
    memdb/xxhash.cc
    memdb/xxhash.h
    bench/micro/chopper.cc
    bench/micro/chopper.h
    bench/micro/piece.cc
    bench/micro/piece.h
    bench/rw_benchmark/chopper.cc
    bench/rw_benchmark/chopper.h
    bench/rw_benchmark/piece.cc
    bench/rw_benchmark/piece.h
    bench/tpca/chopper.h
    bench/tpca/piece.cc
    bench/tpca/piece.h
    bench/tpcc/chopper.cc
    bench/tpcc/chopper.h
    bench/tpcc/piece.cc
    bench/tpcc/piece.h
    bench/tpcc/stock_level.cpp
    bench/tpcc/order_status.cpp
    bench/tpcc/new_order.cpp
    bench/tpcc/delivery.cpp
    bench/tpcc/payment.cpp
    bench/tpcc_dist/chopper.cc
    bench/tpcc_dist/chopper.h
    bench/tpcc_dist/piece.h
    bench/tpcc_real_dist/chopper.cc
    bench/tpcc_real_dist/chopper.h
    bench/tpcc_real_dist/piece.h
    deptran/__init__.py
    deptran/all.h
    deptran/batch_start_args_helper.cc
    deptran/batch_start_args_helper.h
    deptran/benchmark_control_rpc.cc
    deptran/benchmark_control_rpc.h
    deptran/c_main.cc
    deptran/config.cpp
    deptran/config.hpp
    deptran/constants.hpp
    deptran/coordinator.cc
    deptran/coordinator.h
    deptran/dep_graph.cc
    deptran/dep_graph.hpp
    deptran/rcc_row.cpp
    deptran/rcc_row.hpp
    deptran/ro6_row.hpp
    deptran/ro6_row.cpp
    deptran/dtxn.cpp
    deptran/dtxn.hpp
    deptran/tpl.hpp
    deptran/tpl.cpp
    deptran/rcc.cpp
    deptran/rcc.hpp
    deptran/ro6.hpp
    deptran/ro6.cpp
    deptran/graph.cc
    deptran/graph.hpp
    deptran/inactive_client.h
    deptran/inactive_input.h
    deptran/inactive_piecegraph.h
    deptran/marshal-value.cc
    deptran/marshal-value.h
    deptran/pie_info.h
    deptran/piece.cpp
    deptran/piece.hpp
    deptran/rcc_rpc.h
    deptran/rcc_rpc.py
    deptran/rcc_rpc.rpc
    deptran/rcc_service.cc
    deptran/rcc_service.h
    deptran/s_main.cc
    deptran/sharding.cpp
    deptran/sharding.h
    deptran/timer.cc
    deptran/timer.h
    deptran/txn-chopper-factory.cc
    deptran/txn-chopper-factory.h
    deptran/txn_chopper.cc
    deptran/txn_chopper.h
    deptran/txn_info.h
    deptran/txn_reg.hpp
    deptran/txn_req_factory.cc
    deptran/txn_req_factory.h deptran/compress.hpp)

add_executable(deptran_server deptran/s_main.cc)
add_executable(deptran_client deptran/c_main.cc)

target_link_libraries(
    deptran_server
    ROCOCO
    rrr
    ${CMAKE_THREAD_LIBS_INIT}
    "boost_coroutine"
    "boost_context"
    )

target_link_libraries(
    deptran_client
    ROCOCO
    rrr
    ${CMAKE_THREAD_LIBS_INIT}
    "boost_coroutine"
    "boost_context"
    )

