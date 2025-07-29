cmake_minimum_required(VERSION 3.24...3.30)
project(bauripixel)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(RAYLIB_VERSION 5.0)
set(RAYLIB_COMMIT 138010c1ebabe184097f3945534dfed1ca19e929)

FetchContent_Declare(
	raylib
	DOWNLOAD_EXTRACT_TIMESTAMP OFF
	URL https://github.com/raysan5/raylib/archive/${RAYLIB_COMMIT}.zip
	FIND_PACKAGE_ARGS
)

FetchContent_MakeAvailable(raylib)

add_executable(${PROJECT_NAME})
add_subdirectory(src)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)


if (WIN32 OR MINGW OR MSYS OR CYGWIN) 
	add_compile_definitions(BPM_OS_WIN)
endif()

if (UNIX AND NOT APPLE AND NOT CYGWIN)
	add_compile_definitions(BPM_OS_LINUX)
endif()

if (APPLE) 
	add_compile_definitions(BPM_OS_MAC)
endif()

#add_compile_definitions(BALLOC_DEBUG)


# ====== Raylib & Raygui Stuff ======

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

if ("${PLATFORM}" STREQUAL "Web")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )
    #DEPENDS ${PROJECT_NAME}
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
    #DEPENDS ${PROJECT_NAME}
endif()

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)

# Web Configurations
if ("${PLATFORM}" STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC --preload-file resources)
	add_compile_definitions(BFM_OS_WEB)
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
