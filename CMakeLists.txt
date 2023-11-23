cmake_minimum_required(VERSION 3.24)
project(metaverse-tools-editor)

# Add include directories
include_directories("C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Tools\\MSVC\\14.37.32822\\include")

# ----------
#  clap.dll
# ----------
# Add the source file
set(SOURCES_CLAP ${CMAKE_SOURCE_DIR}/lib/rust/C/clap.c)
# Set the target to build a shared library
add_library(clap SHARED ${SOURCES_CLAP})
add_dependencies(clap start)
target_link_libraries(clap "${CMAKE_SOURCE_DIR}\\Debug\\start.lib")

# ----------
#  start.dll
# ----------
# Add the source file
set(SOURCES_START ${CMAKE_SOURCE_DIR}/lib/rust/C/start.cpp)
# Set the target to build a shared library
add_library(start SHARED ${SOURCES_START})
target_compile_definitions(start PRIVATE GRAMMAR_END_OF_TOKEN_MARKER=" ")
target_compile_definitions(start PRIVATE GRAMMAR_END_OF_TOKEN_MARKER_SIZE=1)

# https://stackoverflow.com/questions/34799916/copy-file-from-source-directory-to-binary-directory-using-cmake
# Add a custom command to copy clap.dll to the desired folder
add_custom_command(TARGET clap POST_BUILD    
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:clap> C:/metaverse-tools/editor/$<TARGET_FILE_NAME:clap>
    DEPENDS clap
)



