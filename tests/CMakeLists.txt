set(SCRAPP_TEST_SOURCES
        test.cpp)


# CHECK Catch downloaded
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/catch2/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_executable(scrapp_tests ${SCRAPP_TEST_SOURCES})

add_subdirectory(external/catch2)
target_include_directories(scrapp_tests
        PRIVATE ${CMAKE_SOURCE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/catch2)

target_link_libraries(scrapp_tests PRIVATE scrapp
        PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(scrapp_tests)