message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(glfw3)
find_package(opengl_system)
find_package(tinyobjloader)

set(CONANDEPS_LEGACY  glfw  opengl::opengl  tinyobjloader::tinyobjloader )