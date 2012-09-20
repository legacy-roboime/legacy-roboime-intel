
if(NOT GLUT_FOUND)
    find_package(PhysX)
    if(PHYSX_FOUND)
        find_path(GLUT_INCLUDE_DIR
            NAMES GL/glut.h
            PATH_SUFFIXES Graphics/include/win32
            PATHS ${PHYSX_DIR}
            DOC "Should contain the folder GL not the file glut.h directly."
        )
        if(GLUT_INCLUDE_DIR)
            set(GLUT_FOUND YES)
        endif()
        if(CMAKE_CL_64)
            set(GLUT_PATH_SUFFIX Graphics/lib/win32/glut/x64)
        else()
            set(GLUT_PATH_SUFFIX Graphics/lib/win32/glut)
        endif()
        find_library(GLUT_glut_LIBRARY
            NAMES glut32.lib
            PATH_SUFFIXES ${GLUT_PATH_SUFFIX}
            PATHS ${PHYSX_DIR}
        )
    endif()
endif()

if(GLUT_FOUND)
    set(GLUT_LIBRARIES ${GLUT_glut_LIBRARY})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLUT DEFAULT_MSG GLUT_INCLUDE_DIR)

mark_as_advanced(
    GLUT_INCLUDE_DIR
    GLUT_glut_LIBRARY
)

