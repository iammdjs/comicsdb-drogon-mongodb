cmake_minimum_required(VERSION 3.5)
project(comicsdb-drogon-mongodb CXX)

include(CheckIncludeFileCXX)

check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} main.cc)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# add_subdirectory(drogon) 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
find_package(Drogon CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

# ##############################################################################

if (CMAKE_CXX_STANDARD LESS 17)
    message(FATAL_ERROR "c++17 or higher is required")
elseif (CMAKE_CXX_STANDARD LESS 20)
    message(STATUS "use c++17")
else ()
    message(STATUS "use c++20")
endif ()

aux_source_directory(comicsdb COMICS_DB_SRC)
aux_source_directory(controllers CTL_SRC)
aux_source_directory(filters FILTER_SRC)
aux_source_directory(plugins PLUGIN_SRC)
aux_source_directory(models MODEL_SRC)

drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
                    ${CMAKE_CURRENT_BINARY_DIR})
# use the following line to create views with namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE)
# use the following line to create views with namespace CHANGE_ME prefixed
# and path namespaces.
# drogon_create_views(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/views
#                     ${CMAKE_CURRENT_BINARY_DIR} TRUE CHANGE_ME)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR}/models)
target_sources(${PROJECT_NAME}
               PRIVATE
               ${COMICS_DB_SRC}
               ${SRC_DIR}
               ${CTL_SRC}
               ${FILTER_SRC}
               ${PLUGIN_SRC}
               ${MODEL_SRC})
# ##############################################################################
# uncomment the following line for dynamically loading views 
# set_property(TARGET ${PROJECT_NAME} PROPERTY ENABLE_EXPORTS ON)

# ##############################################################################

include(FetchContent)

# Fetch and build the MongoDB C driver
FetchContent_Declare(
    libmongoc
    URL https://github.com/mongodb/mongo-c-driver/releases/download/1.27.4/mongo-c-driver-1.27.4.tar.gz
)

# Get properties of the dependency
FetchContent_GetProperties(libmongoc)

# Check if the dependency has been populated (downloaded and extracted)
if(NOT libmongoc_POPULATED)
  # If not, make it available (this will fetch and build the content)
  FetchContent_MakeAvailable(libmongoc)
endif()

# Fetch and build the MongoDB C++ driver
FetchContent_Declare(
    libmongocxx
    URL https://github.com/mongodb/mongo-cxx-driver/releases/download/r3.10.2/mongo-cxx-driver-r3.10.2.tar.gz
)

# Get properties of the dependency
FetchContent_GetProperties(libmongocxx)

# Check if the dependency has been populated (downloaded and extracted)
if(NOT libmongocxx_POPULATED)
  # If not, make it available (this will fetch and build the content)
  FetchContent_MakeAvailable(libmongocxx)
endif()

# Include the paths for the MongoDB C++ driver (mongocxx)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${libmongocxx_SOURCE_DIR}/src/mongocxx/include
    ${libmongocxx_SOURCE_DIR}/src/bsoncxx/include
    ${libmongocxx_BINARY_DIR}/src/mongocxx/include
    ${libmongocxx_BINARY_DIR}/src/bsoncxx/include
)

# Set library directories for mongocxx and bsoncxx
target_link_directories(${PROJECT_NAME}
    PRIVATE
    ${libmongocxx_SOURCE_DIR}/src/mongocxx/include
    ${libmongocxx_SOURCE_DIR}/src/bsoncxx/include
    ${libmongocxx_BINARY_DIR}/src/mongocxx/include
    ${libmongocxx_BINARY_DIR}/src/bsoncxx/include
)

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    mongocxx_shared
    bsoncxx_shared
)

add_subdirectory(test)
