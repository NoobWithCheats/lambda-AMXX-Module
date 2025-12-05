cmake_minimum_required(VERSION 3.15)
project(Lambda
        VERSION        0.7.0
        DESCRIPTION    "ACL system AMXX plugins"
        HOMEPAGE_URL   "https://github.com/NoobWithCheats/lambda-AMXX-Module"
        LANGUAGES      CXX
)

###
### Cmake setup
###

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) # Prevent option() from removing normal variables with the same name
set(CMAKE_POLICY_DEFAULT_CMP0117 NEW) # Do not add /GR to standard compilation flags (prevents D9025 warning)

set(AMXX_LAMBDA_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH
        ${AMXX_LAMBDA_ROOT}/cmake
        ${CMAKE_MODULE_PATH}
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out/pdb/${CMAKE_BUILD_TYPE})


set(BUILD_SHARED_LIBS OFF) # BUILD_SHARED_LIBS only affect external dependencies

###
### Compilers setup
###

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(FATAL_ERROR "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}. Only GCC and MSVC are supported")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(GCC ON)
endif ()

set(CMAKE_CXX_STANDARD 17)

if (GCC)
    add_compile_options(
            -m32
    )

    add_link_options(
            -m32
            -static-libstdc++    # Static linking of the GCC C++ runtime
            -static-libgcc       # Static linking of the GCC library
    )
elseif (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Static linking of the MSVC's C++ runtime
endif ()

###
### Add dependencies and subprojects
###

add_subdirectory(src)

###
### Add custom build targets
###

set(MAIN_TARGET_NAME lambda)

add_custom_target(BUILD_ALL ALL)
add_dependencies(BUILD_ALL ${MAIN_TARGET_NAME})

add_custom_target(INSTALL_ALL
        COMMENT "Copying '${MAIN_TARGET_NAME}' library to '$ENV{${MAIN_TARGET_NAME}_COPY_TO_PATH}'"
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${MAIN_TARGET_NAME}> $ENV{${MAIN_TARGET_NAME}_COPY_TO_PATH})

add_custom_target(BUILD_AND_INSTALL_ALL ALL)
add_dependencies(BUILD_AND_INSTALL_ALL BUILD_ALL)
add_dependencies(BUILD_AND_INSTALL_ALL INSTALL_ALL)