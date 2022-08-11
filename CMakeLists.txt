cmake_minimum_required(VERSION 3.22)
project(scrapp)

set(CMAKE_CXX_STANDARD 17)

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
execute_process(COMMAND ./install-boost.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(Boost_DIR external/boost/lib/cmake/Boost-1.80.0)
find_package(Boost 1.80.0 COMPONENTS json REQUIRED NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)

set(SCRAPP_SOURCES
        spider.cpp request.h
        request.h utils.h)

add_library(${PROJECT_NAME} ${SCRAPP_SOURCES})
#target_include_directories(${PROJECT_NAME}
#        PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
        PUBLIC cpr::cpr
        PUBLIC ${Boost_LIBRARIES})
enable_testing()
add_subdirectory(tests)