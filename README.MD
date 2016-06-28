
## Add to project
To start using imgstream in your project simply add this to CMakeLists.txt:

```
include(ExternalProject)

set(LIBRARIES_HOME "Libraries")

ExternalProject_Add(
    imgstream
    GIT_REPOSITORY https://github.com/rwiatr/imgstream.git
    GIT_TAG v0.1.0
    PREFIX ${LIBRARIES_HOME}/imgstream
    # Disable install step
    INSTALL_COMMAND ""
    DOWNLOAD_DIR "${LIBRARIES_HOME}/download/imgstream"
)

add_library(libimgstream IMPORTED STATIC GLOBAL)
add_dependencies(libimgstream imgstream)

ExternalProject_Get_Property(imgstream source_dir binary_dir)
set_target_properties(libimgstream PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libimgstream.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/include")
```

## Example usage
```
#include "imgstream/pngstream.h"

#include <string>

const size_t width = 9;
const size_t height = 8;
const string file_name = string("smile.png");

const imgstream::rgb B(0, 0, 0);
const imgstream::rgb Y(255, 255, 0);
	
int main() {
	imgstream::pngostream<width * 3> ostr(file_name, width, height);

	ostr << B << B << Y << Y << Y << Y << Y << B << B
		 << B << Y << Y << Y << Y << Y << Y << Y << B
	 	 << Y << Y << B << Y << Y << Y << B << Y << Y
	 	 << Y << Y << Y << Y << Y << Y << Y << Y << Y
	 	 << Y << Y << Y << Y << Y << Y << Y << Y << Y
	 	 << Y << B << Y << Y << Y << Y << Y << B << Y
	 	 << B << Y << B << B << B << B << B << Y << B
	 	 << B << B << Y << Y << Y << Y << Y << B << B;

	ostr.close();

	return 0;
}

```

## Tests
Use cmake `-Dimgstream_build_tests=ON` to build tests.

## Dependencies
libpng is required to use imgstream::pngstream. Be aware that libpng has its own license.