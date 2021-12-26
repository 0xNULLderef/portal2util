cmake_minimum_required(VERSION 3.0.0)
project(portal2util VERSION 0.1.0)

set(TARGET_DIR "~/.local/share/Steam/steamapps/common/Portal 2/portal2")

FILE(GLOB cppSrc *.cpp)
add_library(portal2util SHARED ${cppSrc})
target_include_directories(portal2util PRIVATE .)
target_link_libraries(portal2util SDL2)
add_custom_command(TARGET portal2util POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:portal2util> ${TARGET_DIR}/p2u.so
	COMMENT "Created ${TARGET_DIR}/p2u.so"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -g -fPIC -Wall -Werror -O0")
SET(CMAKE_SHARED_LINKER_FLAGS "-m32")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
