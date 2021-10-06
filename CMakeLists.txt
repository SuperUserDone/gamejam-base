cmake_minimum_required(VERSION 3.11)
project(base)

set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

set(DEP_INSTALL_DIR
        "${CMAKE_BINARY_DIR}/extern")

ExternalProject_Add(
        raylib_build
        GIT_REPOSITORY "https://github.com/raysan5/raylib"
        GIT_TAG "3.7.0"
        GIT_SHALLOW 1
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_INSTALL_DIR} -DUSE_EXTERNAL_GLFW=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_EXAMPLES=OFF -DBUILD_GAMES=OFF
        TEST_COMMAND ""
)

include_directories(${DEP_INSTALL_DIR}/include)

add_executable(base main.cpp)
add_dependencies(base raylib_build)

if(WIN32)
set(LIBS ${DEP_INSTALL_DIR}/lib/libraylib.lib)
else()
set(LIBS ${DEP_INSTALL_DIR}/lib/libraylib.a dl pthread)
endif()

target_link_libraries(base ${LIBS})

install(TARGETS base
        CONFIGURATIONS Release
        RUNTIME DESTINATION bin/)

set(CPACK_PACKAGE_GENERATOR "ZIP")
set(CPACK_STRIP_FILES ON)
set(CPACK_SOURCE_STRIP_FILES "")
set(CPACK_PACKAGE_EXECUTABLES "base" "Base")

include(CPack)


