set(SCRAPP_TEST_SOURCES
        test.cpp request_tests.cpp)


# CHECK Catch downloaded
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/catch2/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/trompeloeil/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif ()

add_executable(scrapp_tests ${SCRAPP_TEST_SOURCES})

add_subdirectory(external/catch2)
add_subdirectory(external/trompeloeil)
target_include_directories(scrapp_tests
        PRIVATE ${CMAKE_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/catch2
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/trompeloeil/include)

target_link_libraries(scrapp_tests PRIVATE scrapp
        PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(scrapp_tests)