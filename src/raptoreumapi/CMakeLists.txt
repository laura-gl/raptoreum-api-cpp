FIND_PACKAGE(JSONCPP REQUIRED)
FIND_PACKAGE(JSONRPCCPP REQUIRED)
FIND_PACKAGE(CURL REQUIRED)

# Find header and source files
FILE(GLOB raptoreumapi_header ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB raptoreumapi_source ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Set target libraries
ADD_LIBRARY(raptoreumapi SHARED ${raptoreumapi_source})
ADD_LIBRARY(raptoreumapi_static STATIC ${raptoreumapi_source})

# Set output name of static library
SET_TARGET_PROPERTIES(raptoreumapi_static PROPERTIES OUTPUT_NAME raptoreumapi)

# Link the library with json-rpc-cpp libs
TARGET_LINK_LIBRARIES(raptoreumapi
                        ${CURL_LIBRARY}
                        ${JSONCPP_LIBRARY}
                        jsonrpccpp-common
                        jsonrpccpp-client)

TARGET_LINK_LIBRARIES(raptoreumapi_static
                        ${CURL_LIBRARY}
                        ${JSONCPP_LIBRARY}
                        jsonrpccpp-common
                        jsonrpccpp-client)

# Set version settings
SET(VERSION_STRING ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
SET_TARGET_PROPERTIES(raptoreumapi raptoreumapi_static PROPERTIES
    VERSION "${VERSION_STRING}"
    SOVERSION "${SO_VERSION}")

# Install to appropriate destinations
INSTALL(FILES ${raptoreumapi_header} DESTINATION include/raptoreumapi)

INSTALL(TARGETS raptoreumapi raptoreumapi_static
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
