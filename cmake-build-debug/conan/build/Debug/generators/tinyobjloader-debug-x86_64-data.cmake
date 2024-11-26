########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(tinyobjloader_COMPONENT_NAMES "")
if(DEFINED tinyobjloader_FIND_DEPENDENCY_NAMES)
  list(APPEND tinyobjloader_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES tinyobjloader_FIND_DEPENDENCY_NAMES)
else()
  set(tinyobjloader_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(tinyobjloader_PACKAGE_FOLDER_DEBUG "C:/Users/34660/.conan2/p/b/tinyoaf0b5210e088f/p")
set(tinyobjloader_BUILD_MODULES_PATHS_DEBUG )


set(tinyobjloader_INCLUDE_DIRS_DEBUG "${tinyobjloader_PACKAGE_FOLDER_DEBUG}/include")
set(tinyobjloader_RES_DIRS_DEBUG )
set(tinyobjloader_DEFINITIONS_DEBUG )
set(tinyobjloader_SHARED_LINK_FLAGS_DEBUG )
set(tinyobjloader_EXE_LINK_FLAGS_DEBUG )
set(tinyobjloader_OBJECTS_DEBUG )
set(tinyobjloader_COMPILE_DEFINITIONS_DEBUG )
set(tinyobjloader_COMPILE_OPTIONS_C_DEBUG )
set(tinyobjloader_COMPILE_OPTIONS_CXX_DEBUG )
set(tinyobjloader_LIB_DIRS_DEBUG "${tinyobjloader_PACKAGE_FOLDER_DEBUG}/lib")
set(tinyobjloader_BIN_DIRS_DEBUG )
set(tinyobjloader_LIBRARY_TYPE_DEBUG STATIC)
set(tinyobjloader_IS_HOST_WINDOWS_DEBUG 1)
set(tinyobjloader_LIBS_DEBUG tinyobjloader)
set(tinyobjloader_SYSTEM_LIBS_DEBUG )
set(tinyobjloader_FRAMEWORK_DIRS_DEBUG )
set(tinyobjloader_FRAMEWORKS_DEBUG )
set(tinyobjloader_BUILD_DIRS_DEBUG )
set(tinyobjloader_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(tinyobjloader_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${tinyobjloader_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${tinyobjloader_COMPILE_OPTIONS_C_DEBUG}>")
set(tinyobjloader_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${tinyobjloader_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${tinyobjloader_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${tinyobjloader_EXE_LINK_FLAGS_DEBUG}>")


set(tinyobjloader_COMPONENTS_DEBUG )