cmake_minimum_required(VERSION 3.1)

project (HomeDisc)

include (Local.cmake)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set (INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin CACHE STRING "Instalation path")
set (COMMON_HEADERS ${CMAKE_SOURCE_DIR}/src )


add_subdirectory(clients)
