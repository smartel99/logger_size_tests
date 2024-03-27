set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

# must ALWAYS be after static_library option
project(size_tests C CXX ASM)

option(USE_CVAR_LOGGER BOOL OFF)
option(FACTORIAL_CALLS BOOL OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR})

# project settings
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)

add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors -Wno-unused-function -Wno-unused-parameter -Wno-unknown-pragmas")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

# To reduce binary file output size.
add_compile_options(-fdata-sections)
add_compile_options(-ffunction-sections)
add_compile_options(-fmessage-length=0)
add_compile_options(-fno-common)
add_compile_options(-fno-exceptions)
add_compile_options(-Wno-psabi)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-enforce-eh-specs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-use-cxa-atexit")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fnothrow-opt")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-enums")

add_compile_options(-fdiagnostics-color=always)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Os -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g)
endif ()

include(FetchContent)

set(BOOST_INCLUDE_LIBRARIES preprocessor)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
        boost_preprocessor
        GIT_REPOSITORY https://github.com/boostorg/preprocessor.git
        GIT_TAG boost-1.84.0
)
FetchContent_MakeAvailable(boost_preprocessor)

include_directories(
        ${SRC_DIR}
        ${SRC_DIR}/cep
)

if (USE_CVAR_LOGGER)
    set(BATCH_TYPE cvar)
    add_definitions(-DUSE_CVAR)
else ()
    set(BATCH_TYPE var_template)
endif ()


MESSAGE(STATUS "Factorial calls: ${FACTORIAL_CALLS}")

if (FACTORIAL_CALLS)
    set(BATCH_TYPE ${BATCH_TYPE}_Factorial)
    add_definitions(-DFACTORIAL_CALLS)
else ()
endif ()

add_definitions(-DDEBUG)

file(GLOB_RECURSE SOURCES
        "${SRC_DIR}/cep/logger_test.cpp"
)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-u _printf_float)
add_link_options(-Wl,--no-warn-rwx-segments) # Disable GCC 12.2 new warning for RWX segments

set(test_output_dir ${PROJECT_BINARY_DIR}/test)
file(MAKE_DIRECTORY ${test_output_dir})

set(test_list)
message(STATUS ${CMAKE_HOST_SYSTEM_NAME})
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    message(STATUS "building for windows")
    set(BIN_FILE_EXTENSION .exe)
endif ()

macro(make_test dir_name iter flag value)
    file(MAKE_DIRECTORY ${test_output_dir}/${name})
    set(test_name ${dir_name}-${iter})
    message(VERBOSE "Making ${test_name}...")
    add_executable(${test_name} ${SOURCES} ${LINKER_SCRIPT})
    set_target_properties(${test_name}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${test_output_dir}/${dir_name}/elf"
            LIBRARY_OUTPUT_DIRECTORY "${test_output_dir}/${dir_name}/elf"
            RUNTIME_OUTPUT_DIRECTORY "${test_output_dir}/${dir_name}/elf")
    target_compile_definitions(${test_name} PRIVATE -DITER_COUNT=${iter} -DFLAG=%${flag} -DVALUE=${value})
    target_link_libraries(${test_name} PRIVATE Boost::preprocessor)
    set(EXE_FILE "${test_output_dir}/${dir_name}/elf/${test_name}${BIN_FILE_EXTENSION}")
    set(BIN_FILE ${test_output_dir}/${dir_name}/${test_name}.bin)
    message(STATUS "exe: ${EXE_FILE}, bin: ${BIN_FILE}")
    list(APPEND test_list ${BIN_FILE})
    add_custom_command(TARGET ${test_name} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Obinary ${EXE_FILE} ${BIN_FILE}
            COMMENT "Building ${BIN_FILE}")
endmacro()

macro(make_my_tests name flag value)
    file(MAKE_DIRECTORY ${test_output_dir}/${name})
    foreach (it RANGE 1 256 20)
        make_test(${name} ${it} ${flag} ${value})
    endforeach ()
endmacro()

make_my_tests(char c 'a')
make_my_tests(string s "\"a\"")
make_my_tests(integer i 1)
make_my_tests(float f 1.123f)
make_my_tests(pointer p nullptr)

set(OUT_RES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/result/${BATCH_TYPE}_${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/result)
file(REMOVE ${OUT_RES_PATH}.csv)
file(REMOVE ${OUT_RES_PATH}.png)
add_custom_target(out.csv
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/utils/make_csv.py ${test_output_dir} ${OUT_RES_PATH}.csv
        DEPENDS ${test_list}
        COMMENT "Building CSV")

add_custom_target(graph
        COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/utils/graph_from_csv.py ${OUT_RES_PATH}.csv ${OUT_RES_PATH}.png
        DEPENDS out.csv
        COMMENT "Making Graph")

message(STATUS "Output graph: ${OUT_RES_PATH}.png")
