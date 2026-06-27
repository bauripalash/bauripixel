#
# CMakeLists.txt - Base CMake Configuration
# Copyright (C) <year>  <name of author>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.16...3.30)

project(bauripixel C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE 
		"Debug" 
		)
endif()


set(GIT_COMMIT_HASH "_")
set(GIT_COMMIT_COUNT "_")

set(IS_OS_WIN FALSE)
set(IS_OS_LINUX FALSE)
set(IS_OS_MACOS FALSE)
set(IS_OS_WEB FALSE)

set(IS_GCC FALSE)
set(IS_CLANG FALSE)
set(IS_MSVC FALSE)
set(IS_EMCC FALSE)

set(IS_DEBUG_BUILD FALSE)
set(IS_RELEASE_BUILD FALSE)
set(IS_GFX_BUILD FALSE)

if(BUILD_GFX)
    set(IS_GFX_BUILD TRUE)
endif()


# ================== CROSS PLATFORM OUTPUT SETUP ==================

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${CONFIG} CONFIG_UPPER)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR})
	seT(CMAKE_PDB_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR})
endforeach()

# ================== END CROSS PLATFORM OUTPUT SETUP ==================



# ================== OS CHECK ==================

if (WIN32 OR CMAKE_SYSTEM_NAME STREQUAL "Windows" OR 
	CMAKE_SYSTEM_NAME STREQUAL "CYGWIN" OR 
	CMAKE_SYSTEM_NAME STREQUAL "MSYS") 

	set(IS_OS_WIN TRUE)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(IS_OS_LINUX TRUE)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin") 
	set(IS_OS_MACOS TRUE)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
	set(IS_OS_WEB TRUE)
	set(IS_EMCC TRUE)
endif()

# ================== END OS CHECK ==================


# ================== COMPILER CHECK ==================

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(IS_GCC TRUE)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang" OR 
        CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
	set(IS_CLANG TRUE)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
	set(IS_MSVC TRUE)
elseif(CMAKE_C_COMPILER_ID STREQUAL "TinyCC")
    set(IS_GFX_BUILD NO)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(IS_DEBUG_BUILD TRUE)
else()
	set(IS_RELEASE_BUILD TRUE)
endif()

# ================== END COMPILER CHECK ==================

set_property(GLOBAL PROPERTY C_STANDARD 11)
set_property(GLOBAL PROPERTY C_STANDARD_REQUIRED ON)

if (IS_GCC)
	set_property(GLOBAL PROPERTY C_EXTENSIONS ON)
endif()
#target_link_options(${PROJECT_NAME} PRIVATE "-static")

if (IS_OS_LINUX AND (IS_GCC OR IS_CLANG))
    find_program(MOLD_PROG mold)
    if(MOLD_PROG)
        set(CMAKE_LINKER_TYPE MOLD)
    endif()
endif()



# ================== COMPILER FLAGS ==================
set(BAURIPIXEL_DEBUG_CFLAGS
	"-g3" 
	"-pedantic"
	"-Wall" 

)

set(BAURIPIXEL_RELEASE_CFLAGS
	"-pedantic" 
	"-Wall"
	"-O2"
	"-flto" 
	"-funroll-loops" 
)



set (BAURIPIXEL_REL_WITH_DINFO_CFLAGS
	"-g3"
	${BAURIPIXEL_RELEASE_CFLAGS}
)

if(IS_OS_LINUX)
	list(APPEND BAURIPIXEL_DEBUG_CFLAGS 	
        "-fdiagnostics-color=always"
	)
    if (NOT NO_ASAN)
		list(APPEND BAURIPIXEL_DEBUG_CFLAGS 	
            "-fsanitize=address"
            "-fsanitize=undefined"
            "-fsanitize=float-cast-overflow"
            "-fdiagnostics-color=always"
        )
    endif()
endif()

if(IS_MSVC)
	# Put MSVC CFLAGS overrides
	set(BAURIPIXEL_DEBUG_CFLAGS
		"/Od"
		"/W2"
		"/wd4127"
		"/wd4100"
	)
set(BAURIPIXEL_RELEASE_CFLAGS
		"/GL"
		"/Ox"
		"/GF"
		"/Gy"
	)
set (BAURIPIXEL_REL_WITH_DINFO_CFLAGS
		"-g3"
		${BAURIPIXEL_RELEASE_CFLAGS}
	)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
endif()

# ================== END COMPILER FLAGS ==================

# ================== LINKER FLAGS ==================
set(BAURIPIXEL_DEBUG_LINKER_FLAGS
	"-flto"
)

set(BAURIPIXEL_RELEASE_LINKER_FLAGS
	"-flto"
	"-funroll-loops"
)

set(BAURIPIXEL_REL_WITH_DINFO_LINKER_FLAGS 
	${BAURIPIXEL_RELEASE_LINKER_FLAGS}
)
if(IS_OS_LINUX)
    if (NOT NO_ASAN)
		list(APPEND BAURIPIXEL_DEBUG_LINKER_FLAGS
            "-fsanitize=address"
            "-fsanitize=undefined"
            "-fsanitize=float-cast-overflow" 
        )
    endif()
endif()
if(IS_MSVC)
	# Put MSVC LDFLAGS overrides
	set(BAURIPIXEL_DEBUG_LINKER_FLAGS
	)
set(BAURIPIXEL_RELEASE_LINKER_FLAGS
		"/LTCG"
	)
set(BAURIPIXEL_REL_WITH_DINFO_LINKER_FLAGS 
	${BAURIPIXEL_RELEASE_LINKER_FLAGS}
	)
endif()
if(IS_OS_WIN AND NOT IS_MSVC)
	list(APPEND BAURIPIXEL_DEBUG_LINKER_FLAGS "-static")
	list(APPEND BAURIPIXEL_RELEASE_LINKER_FLAGS "-static")
	list(APPEND BAURIPIXEL_REL_WITH_DINFO_LINKER_FLAGS "-static")
endif()
# ================== END LINKER FLAGS ==================

add_executable(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
add_subdirectory(src)
add_compile_definitions(USING_CMAKE_BUILD=1)
add_subdirectory(src/external/raylib)

target_compile_options(${PROJECT_NAME} PRIVATE 
	$<$<CONFIG:Debug>:${BAURIPIXEL_DEBUG_CFLAGS}>
	$<$<CONFIG:Release>:${BAURIPIXEL_RELEASE_CFLAGS}>
	$<$<CONFIG:RelWithDebInfo>:${BAURIPIXEL_REL_WITH_DINFO_CFLAGS}>
	$<$<CONFIG:MinSizeRel>:${BAURIPIXEL_RELEASE_CFLAGS}>
)

target_link_options(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:${BAURIPIXEL_DEBUG_LINKER_FLAGS}>
	$<$<CONFIG:Release>:${BAURIPIXEL_RELEASE_LINKER_FLAGS}>
	$<$<CONFIG:RelWithDebInfo>:${BAURIPIXEL_REL_WITH_DINFO_LINKER_FLAGS}>
	$<$<CONFIG:MinSizeRel>:${BAURIPIXEL_RELEASE_LINKER_FLAGS}>
)


target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if (NOT IS_MSVC)
	target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

if(IS_DEBUG_BUILD)
	target_compile_definitions(${PROJECT_NAME} PRIVATE MI_TRACK_ASAN=1)
	add_compile_definitions(BAURIPIXEL_BUILD_DEBUG)
else()
	add_compile_definitions(BAURIPIXEL_BUILD_RELEASE)
endif()

if(USE_NAN_BOXING)
	add_compile_definitions(USE_NAN_BOXING=1)
endif()

if (IS_OS_WIN) 
	add_compile_definitions(BAURIPIXEL_OS_WIN)
elseif (IS_OS_LINUX)
	add_compile_definitions(BAURIPIXEL_OS_LINUX)
elseif (IS_OS_MACOS) 
	add_compile_definitions(BAURIPIXEL_OS_MAC)
elseif (IS_OS_WEB)
	add_compile_definitions(BAURIPIXEL_OS_WEB)
endif()

message(STATUS "")
message(STATUS "")
message(STATUS "======== BauriPixel Build Information ========")
message(STATUS "")
message(STATUS "Build Type : ${CMAKE_BUILD_TYPE}")
message(STATUS "OS :  ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler :  ${CMAKE_C_COMPILER_ID}")
message(STATUS "Linker : ${CMAKE_LINKER_TYPE}")
message(STATUS "")
message(STATUS "==========================================")
message(STATUS "")
message(STATUS "")
