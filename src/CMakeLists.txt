set(TARGET_NAME lambda)
set(OUTPUT_NAME lambda_amxx)



set(LIBRARY_BUILD_TYPE SHARED)
set(TARGET_LIBRARIES_SCOPE PRIVATE)

add_library(${TARGET_NAME} ${LIBRARY_BUILD_TYPE}
        sdk/amxxmodule.cpp
        sdk/amxxmodule.h
        sdk/moduleconfig.h
        lambda.h
        lambda.cpp
        native.cpp
)

target_include_directories(${TARGET_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${TARGET_NAME} PRIVATE
        NOMINMAX
        HAVE_STDINT_H
        NO_MSVC8_AUTO_COMPAT
)

if (UNIX)
    target_compile_definitions(${TARGET_NAME} PRIVATE
            LINUX
    )
else ()
    target_compile_definitions(${TARGET_NAME} PRIVATE
            _WIN32
            _WIN32_WINNT=0x0600
            WIN32
            WINVER=0x0600
    )
endif ()

if (GCC)
    target_compile_options(${TARGET_NAME} PRIVATE
            -fno-rtti  # disable rtti to reduce the library size
            -Werror    # treat all warnings as error
    )
elseif (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE
            /GR-       # disable rtti to reduce the library size
            /WX        # treat all warnings as error
            /EHsc      # NEVER catch SEH exceptions by catch(...); c functions cannot throw exceptions
    )

    if (CMAKE_BUILD_TYPE MATCHES Release)
        target_compile_options(${TARGET_NAME} PRIVATE
                /Zi              # produce a PDB file
        )

        target_link_options(${TARGET_NAME} PRIVATE
                /LTCG            # enables whole program optimization on link-time
                /INCREMENTAL:NO  # /INCREMENTAL is not compatible with /LTCG
                /DEBUG           # enables PDB generation
                /OPT:REF         # eliminates functions and data that are never referenced
                /OPT:ICF         # enables COMDAT folding
        )
    endif ()
endif ()

set_target_properties(${TARGET_NAME} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "${OUTPUT_NAME}$<$<BOOL:${UNIX}>:_i386>"
        PDB_NAME ${OUTPUT_NAME}
)