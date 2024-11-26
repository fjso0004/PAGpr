########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(tinyobjloader_FIND_QUIETLY)
    set(tinyobjloader_MESSAGE_MODE VERBOSE)
else()
    set(tinyobjloader_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tinyobjloaderTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${tinyobjloader_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(tinyobjloader_VERSION_STRING "2.0.0-rc10")
set(tinyobjloader_INCLUDE_DIRS ${tinyobjloader_INCLUDE_DIRS_DEBUG} )
set(tinyobjloader_INCLUDE_DIR ${tinyobjloader_INCLUDE_DIRS_DEBUG} )
set(tinyobjloader_LIBRARIES ${tinyobjloader_LIBRARIES_DEBUG} )
set(tinyobjloader_DEFINITIONS ${tinyobjloader_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${tinyobjloader_BUILD_MODULES_PATHS_DEBUG} )
    message(${tinyobjloader_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()

