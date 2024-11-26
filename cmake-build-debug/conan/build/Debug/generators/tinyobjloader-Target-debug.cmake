# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(tinyobjloader_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(tinyobjloader_FRAMEWORKS_FOUND_DEBUG "${tinyobjloader_FRAMEWORKS_DEBUG}" "${tinyobjloader_FRAMEWORK_DIRS_DEBUG}")

set(tinyobjloader_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET tinyobjloader_DEPS_TARGET)
    add_library(tinyobjloader_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET tinyobjloader_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${tinyobjloader_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${tinyobjloader_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### tinyobjloader_DEPS_TARGET to all of them
conan_package_library_targets("${tinyobjloader_LIBS_DEBUG}"    # libraries
                              "${tinyobjloader_LIB_DIRS_DEBUG}" # package_libdir
                              "${tinyobjloader_BIN_DIRS_DEBUG}" # package_bindir
                              "${tinyobjloader_LIBRARY_TYPE_DEBUG}"
                              "${tinyobjloader_IS_HOST_WINDOWS_DEBUG}"
                              tinyobjloader_DEPS_TARGET
                              tinyobjloader_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "tinyobjloader"    # package_name
                              "${tinyobjloader_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${tinyobjloader_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## GLOBAL TARGET PROPERTIES Debug ########################################
    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                 $<$<CONFIG:Debug>:${tinyobjloader_OBJECTS_DEBUG}>
                 $<$<CONFIG:Debug>:${tinyobjloader_LIBRARIES_TARGETS}>
                 )

    if("${tinyobjloader_LIBS_DEBUG}" STREQUAL "")
        # If the package is not declaring any "cpp_info.libs" the package deps, system libs,
        # frameworks etc are not linked to the imported targets and we need to do it to the
        # global target
        set_property(TARGET tinyobjloader::tinyobjloader
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     tinyobjloader_DEPS_TARGET)
    endif()

    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                 $<$<CONFIG:Debug>:${tinyobjloader_LINKER_FLAGS_DEBUG}>)
    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<$<CONFIG:Debug>:${tinyobjloader_INCLUDE_DIRS_DEBUG}>)
    # Necessary to find LINK shared libraries in Linux
    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                 $<$<CONFIG:Debug>:${tinyobjloader_LIB_DIRS_DEBUG}>)
    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                 $<$<CONFIG:Debug>:${tinyobjloader_COMPILE_DEFINITIONS_DEBUG}>)
    set_property(TARGET tinyobjloader::tinyobjloader
                 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                 $<$<CONFIG:Debug>:${tinyobjloader_COMPILE_OPTIONS_DEBUG}>)

########## For the modules (FindXXX)
set(tinyobjloader_LIBRARIES_DEBUG tinyobjloader::tinyobjloader)
