########
# GLFW #
########

project(GLFW)

set(CMAKE_C_STANDARD 11)

aux_source_directory(glfw/src GLFW_FILES)

# Defining preprocessor macros and selecting files to be removed
if (WIN32 OR CYGWIN)
    set(
        GLFW_DEFINITIONS

        -D_GLFW_WIN32
    )

    file(
        GLOB
        GLFW_UNUSED_FILES

        glfw/src/cocoa*
        glfw/src/x11*
        glfw/src/wl*
        glfw/src/mir*
        glfw/src/glx*
        glfw/src/linux*
        glfw/src/posix*
        glfw/src/*.m
    )

    if (CYGWIN)
        # GLFW needs to be linked against the Windows Gdi32 library with Cygwin
        set(GLFW_LINKER_FLAGS Gdi32)
    endif ()
elseif (APPLE)
    set(
        GLFW_DEFINITIONS

        -D_GLFW_COCOA
        -D_GLFW_USE_CHDIR
        -D_GLFW_USE_MENUBAR
        -D_GLFW_USE_RETINA
    )

    file(
        GLOB
        GLFW_UNUSED_FILES

        glfw/src/win32*
        glfw/src/x11*
        glfw/src/wl*
        glfw/src/mir*
        glfw/src/glx*
        glfw/src/linux*
    )
elseif (UNIX)
    set(
        GLFW_DEFINITIONS

        -D_GLFW_X11
        -D_GLFW_HAS_XF86VM
    )

    file(
        GLOB
        GLFW_UNUSED_FILES

        glfw/src/cocoa*
        glfw/src/win32*
        glfw/src/wl*
        glfw/src/wgl*
        glfw/src/mir*
        glfw/src/*.m
    )

    set(
        GLFW_LINKER_FLAGS

        dl
        pthread
        GL
        X11
        Xrandr
        Xcursor
        Xinerama
        Xxf86vm
    )
endif ()

# Adding recursively every file we want to compile
file(
    GLOB
    GLFW_SRC

    ${GLFW_FILES}
)

list(
    REMOVE_ITEM
    GLFW_SRC

    ${GLFW_UNUSED_FILES}
)

# Building GLFW
add_library(GLFW OBJECT ${GLFW_SRC})

target_include_directories(
    GLFW

    SYSTEM
    PUBLIC

    glfw/include
    glfw/deps
)

target_compile_definitions(GLFW PUBLIC ${GLFW_DEFINITIONS})

# Disabling all compilers warnings
if (MSVC)
    target_compile_options(GLFW PRIVATE /w)
else ()
    target_compile_options(GLFW PRIVATE -w)
endif ()

target_link_libraries(GLFW PUBLIC ${GLFW_LINKER_FLAGS})
