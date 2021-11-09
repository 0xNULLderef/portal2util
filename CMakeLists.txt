cmake_minimum_required(VERSION 3.0.0)
project(portal2util VERSION 0.1.0)

set(TARGET_DIR "~/.local/share/Steam/steamapps/common/Portal 2/portal2")

FILE(GLOB cppSrc *.cpp)
add_library(portal2util SHARED ${cppSrc})
target_include_directories(portal2util PRIVATE .)
add_custom_command(TARGET portal2util POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:portal2util> ${TARGET_DIR}/portal2util.so
	COMMENT "Created ${TARGET_DIR}/portal2util.so"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -g -fPIC -Wall -Wno-reorder -Wno-init-self -O0")
SET(CMAKE_SHARED_LINKER_FLAGS "-m32")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
