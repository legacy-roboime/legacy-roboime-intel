if(WIN32)
    find_path(GLUT_INCLUDE_DIR
        NAMES GL/glut.h 
        PATHS  ${GLUT_ROOT_PATH}/include
    )
    find_library(GLUT_glut_LIBRARY
        NAMES glut glut32 freeglut
        PATHS
        ${OPENGL_LIBRARY_DIR}
        ${GLUT_ROOT_PATH}/Release
    )
elseif(APPLE)
    # These values for Apple could probably do with improvement.
    find_path(GLUT_INCLUDE_DIR
        glut.h
        /System/Library/Frameworks/GLUT.framework/Versions/A/Headers
        ${OPENGL_LIBRARY_DIR}
    )
    set(GLUT_glut_LIBRARY "-framework GLUT" CACHE STRING "GLUT library for OSX") 
    set(GLUT_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
else()
    find_path(GLUT_INCLUDE_DIR
        GL/glut.h
        /usr/include/GL
        /usr/openwin/share/include
        /usr/openwin/include
        /opt/graphics/OpenGL/include
        /opt/graphics/OpenGL/contrib/libglut
    )
  
    find_library(GLUT_glut_LIBRARY
        glut
        /usr/openwin/lib
    )
    
    find_library(GLUT_Xi_LIBRARY
        Xi
        /usr/openwin/lib
    )
    
    find_library(GLUT_Xmu_LIBRARY
        Xmu
        /usr/openwin/lib
    )
endif()

set(GLUT_FOUND NO)
if(GLUT_INCLUDE_DIR)
    if(GLUT_glut_LIBRARY)
        # Is -lXi and -lXmu required on all platforms that have it?
        # If not, we need some way to figure out what platform we are on.
        set(GLUT_LIBRARIES
            ${GLUT_glut_LIBRARY}
            ${GLUT_Xmu_LIBRARY}
            ${GLUT_Xi_LIBRARY} 
            ${GLUT_cocoa_LIBRARY}
        )
        set(GLUT_FOUND YES)
    
        #The following deprecated settings are for backwards compatibility with CMake1.4
        set(GLUT_LIBRARY ${GLUT_LIBRARIES})
        set(GLUT_INCLUDE_PATH ${GLUT_INCLUDE_DIR})
    endif()
else()
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
    elseif()
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
    GLUT_Xmu_LIBRARY
    GLUT_Xi_LIBRARY
)

