cmake_minimum_required(VERSION 3.10)

project(AutoPatcher)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -s")

if (MINGW32)
	set(CMAKE_SYSTEM_NAME Windows)
	SET(CMAKE_C_COMPILER i686-w64-mingw32-gcc)
	SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
	SET(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
	set(CMAKE_RANLIB i686-w64-mingw32-ranlib)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
	set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -static")

	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/xdelta3.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

include_directories(${PROJECT_SOURCE_DIR})
add_executable(AutoPatcher
	main.cpp
	registry.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
