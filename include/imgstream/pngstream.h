#ifndef IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_
#define IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_ 1

#include "imgstream/imgstream.h"

#include <string>
#include <png.h>

using namespace std;
namespace imgstream {
template<size_t row_size = 4096>
class pngostream;

template<size_t row_size>
class pngostream : public imgostream<pngostream<row_size>>{
  const size_t width;
  
  FILE *fp             = NULL;
  png_structp png_ptr  = NULL;
  png_infop   info_ptr = NULL;

  png_byte row[row_size];
  size_t   row_pos = 0;

public:

  pngostream(string file_name, size_t width, size_t height) : width(width) {
    if (row_size < this->width * 3) throw string("row size too low");

    if ((fp = fopen(file_name.c_str(), "wb")) == NULL) {
      throw string("failed to open file ") + file_name;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
      throw string("failed to create png structure");
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == NULL) {
      throw string("could not allocate info struct");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
      throw string("error during png creation");
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
  }

  pngostream& operator<<(uint8_t byte) {
    row[row_pos++] = byte;
    tryFlush();
    return *this;
  }

  pngostream& operator<<(const rgb& color) {
    row[row_pos++] = color.r;
    row[row_pos++] = color.g;
    row[row_pos++] = color.b;
    tryFlush();
    return *this;
  }

  void tryFlush() {
    if (row_pos == row_size) {
      row_pos = 0;
      png_write_row(png_ptr, row);
    }
  }

  void close() {
    tryFlush();
    png_write_end(png_ptr, NULL);
  }

  uint8_t inspect(size_t pos) {
    return row[pos];
  }

  ~pngostream() {
    if (fp != NULL) fclose(fp);

    if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);

    if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  }
};
}
#endif // IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_
