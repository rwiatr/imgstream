if (__TEST__LIB)
else(__TEST__LIB)

include(ExternalProject)

### GoogleTest
ExternalProject_Add(
    gtest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    PREFIX ${LIBRARIES}/gtest
    # Disable install step
    INSTALL_COMMAND ""
    DOWNLOAD_DIR "${LIBRARIES}/download/gtest"
)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

ExternalProject_Get_Property(gtest source_dir binary_dir)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    #"INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)
include_directories("${source_dir}/include")

### GoogleMock
ExternalProject_Add(
    gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    PREFIX ${LIBRARIES}/gmock
    # Disable install step
    INSTALL_COMMAND ""
    DOWNLOAD_DIR "${LIBRARIES}/download/gmock"
)
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)

ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    #"INTERFACE_INCLUDE_DIRECTORIES" "${source_dir}/include"
)

include_directories("${source_dir}/include")

set(__TEST__LIB true)
endif(__TEST__LIB)
