cmake_minimum_required(VERSION 3.23)
project(scrapp)

set(CMAKE_CXX_STANDARD 17)
option(SCRAPP_BUILD_TESTS "Build with tests" OFF)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if (GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif ()
    endif ()
endif ()

add_subdirectory(external/cpr)
include(FetchContent)
FetchContent_Declare(
        lexbor
        GIT_REPOSITORY https://github.com/lexbor/lexbor
        GIT_TAG 24dfceab90b9491b8efd44a19bb60936e5ee1922
)
FetchContent_MakeAvailable(lexbor)
execute_process(COMMAND ./install-boost.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(Boost_DIR external/boost/lib/cmake/Boost-1.80.0)
find_package(Boost 1.80.0 COMPONENTS json REQUIRED NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)

set(SCRAPP_HEADERS
        spider.h request.h response.h exceptions.h utils.h html/types.h html/element.h html/html_exceptions.h html/document.h)
set(SCRAPP_SOURCES
        spider.cpp request.cpp response.cpp exceptions.cpp utils.cpp html/element.cpp html/html_exceptions.cpp html/document.cpp)

add_library(${PROJECT_NAME} STATIC)
target_sources(
        ${PROJECT_NAME}
        PRIVATE ${SCRAPP_SOURCES}
        PUBLIC FILE_SET HEADERS BASE_DIRS ${PROJECT_SOURCE_DIR} FILES ${SCRAPP_HEADERS})
target_include_directories(${PROJECT_NAME}
        PUBLIC ${Boost_INCLUDE_DIRS} ${LEXBOR_INCLUDES})
target_link_libraries(${PROJECT_NAME}
        PUBLIC cpr::cpr
        PUBLIC ${Boost_LIBRARIES}
        PUBLIC lexbor)

install(TARGETS ${PROJECT_NAME} FILE_SET HEADERS)
if (SCRAPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()