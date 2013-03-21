if(NOT QT5_PREFIX_PATH)
    find_path(
        QT5_PREFIX_PATH
        NAMES
        bin/qmake
        bin/qmake.exe
        PATHS
        /usr/local/Qt-5.0.0/
        /usr/local/Qt-5.0.1/
        /usr/local/Qt-5.0.2/
        /Qt/Qt5.0.0/5.0.0/msvc2010
        /Qt/Qt5.0.1/5.0.1/msvc2010
        /Qt/Qt5.0.2/5.0.2/msvc2010
        DOC "Path where Qt5 base is located."
        NO_DEFAULT_PATH
    )
    #message(STATUS "Found Qt5: ${QT5_PREFIX_PATH}")
    if(QT5_PREFIX_PATH)
        set(QT5_FOUND "YES")
        set(CMAKE_PREFIX_PATH ${QT5_PREFIX_PATH}/lib/cmake)
    else()
        set(QT5_FOUND "NO")
    endif()
else()
    set(CMAKE_PREFIX_PATH ${QT5_PREFIX_PATH}/lib/cmake)
endif()
find_package(Qt5Core REQUIRED)

#include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED
# if all listed variables are TRUE
#find_package_handle_standard_args()
mark_as_advanced(QT5_PREFIX_PATH)
