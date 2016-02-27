cmake_minimum_required(VERSION 3.1)

project (HomeDisc)

include (Local.cmake)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
set (INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin CACHE STRING "Instalation path")
set (COMMON_HEADERS ${CMAKE_SOURCE_DIR}/src )

SET ( dep_DIR ${CMAKE_SOURCE_DIR} )
FIND_PACKAGE ( dep )

IF ( DEP_FOUND )

    add_definitions(-std=c++1y -ggdb -D_GLIBCXX_DEBUG)

    INCLUDE_DIRECTORIES ( ${ZEROMQ_INCLUDE_DIR} )
    LINK_DIRECTORIES ( ${ZEROMQ_LIBRARY} )

    INCLUDE_DIRECTORIES ( ${EASYLOGGING_INCLUDE_DIR} )
    INCLUDE_DIRECTORIES ( ${MD5_INCLUDE_DIR} )
    INCLUDE_DIRECTORIES ( ${B64_INCLUDE_DIR} )
    INCLUDE_DIRECTORIES ( ${BOOST_INCLUDE_DIR} )
	
	add_subdirectory(clients)
   
    install( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/res/conf" 
                       "${CMAKE_CURRENT_SOURCE_DIR}/res/logs"
             DESTINATION "${INSTALL_DIR}" )
ENDIF ( DEP_FOUND )


