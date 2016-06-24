#ifndef IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_
#define IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_ 1

#include "imgstream/imgstream.h"

#include <stdexcept>
#include <string>
#include <iostream>
#include <errno.h>

#include <png.h>

using namespace std;
namespace imgstream {
template<size_t row_size = 4096>
class pngostream;

template<size_t row_size = 4096>
class pngistream;

template<size_t row_size>
class pngostream : public imgostream<pngostream<row_size>>{
  const size_t width;
  
  FILE *fp             = NULL;
  png_structp png_ptr  = NULL;
  png_infop   info_ptr = NULL;

  png_byte row[row_size];
  size_t   row_pos = 0;

  bool isFull() {return this->width * 3 == row_pos;}
  void tryFlush() {
    if (isFull()) {
      row_pos = 0;
      png_write_row(png_ptr, row);
    }
  }

public:

  pngostream(string file_name, size_t width, size_t height) : width(width) {
    if (row_size < this->width * 3) throw std::invalid_argument("row size too low");

    if ((fp = fopen(file_name.c_str(), "wb")) == NULL) {
      throw std::invalid_argument(string("failed to open file ") + file_name);
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr == NULL) {
      throw std::invalid_argument(string("failed to create png structure"));
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (info_ptr == NULL) {
      throw std::invalid_argument("could not allocate info struct");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
      throw std::invalid_argument("error during png creation");
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);
  }

  pngostream& operator<<(const uint8_t& byte) {
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

  void close() {
    tryFlush();
    png_write_end(png_ptr, NULL);

    if (fp) fclose(fp);
    if (info_ptr) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    fp = NULL;
    png_ptr = NULL;
    info_ptr = NULL;
  }

  ~pngostream() {
    close();
  }
};

template<size_t row_size>
class pngistream: public imgistream<pngistream<row_size>> {
  FILE *fp;
  png_infop info_ptr;
  png_structp png_ptr;

  png_byte row[row_size];

  size_t width, height;
  size_t row_pos, row_idx;

  png_byte color_type;
  png_byte bit_depth;
  size_t number_of_passes;

  bool tryLoad() {
    if (row_pos < width * 3) return true;

    if (row_idx < height) {
      loadRow();
      return true;
    }

    return false;
  }

  void loadRow() {
    png_read_row(png_ptr, row, NULL);
    row_pos = 0;
    row_idx++;
  }

public:
  pngistream(string file_name) {
    if (!(fp = fopen(file_name.c_str(), "rb")))
      throw std::invalid_argument(string("failed to openn file ") + file_name);

    uint8_t header[8];

    if (fread(&header, 1, sizeof(header), fp) != 8) 
      throw std::invalid_argument(string("failed to read header of ") + file_name);
    
    if (png_sig_cmp(header, 0, 8))
      throw std::invalid_argument(string("failed to recognize ") + file_name + string(" as PNG file"));

    if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
      throw std::invalid_argument(string("png_create_read_struct failed for ") + file_name);

    if (!(info_ptr = png_create_info_struct(png_ptr)))
      throw std::invalid_argument(string("png_create_info_struct failed for ") + file_name);

    if (setjmp(png_jmpbuf(png_ptr)))
      throw std::invalid_argument(string("error during init_io for ") + file_name);

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    number_of_passes = png_set_interlace_handling(png_ptr);

    if (row_size < width * 3) 
      throw std::invalid_argument(
        string("row size is ") + std::to_string(row_size) + 
        string(" and should be ") + std::to_string(width * 3));

    png_read_update_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr)))
      throw std::invalid_argument(string("error during read file") + file_name);

    row_pos = 0;
    row_idx = 0;

    png_read_row(png_ptr, row, NULL);
  }

  size_t get_width() {
    return width;
  }

  size_t get_height() {
    return height;
  }

  pngistream& operator>>(uint8_t& byte) {
    if (tryLoad()) byte = row[row_pos++];
    else throw std::invalid_argument("no more data");

    return *this;
  }

  pngistream& operator>>(rgb& rgb) {
    *this >> rgb.r >> rgb.g >> rgb.b; 
    return *this;
  }

  ~pngistream() {
    if (fp) fclose(fp);
    if (png_ptr && info_ptr) png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    fp = NULL;
    png_ptr = NULL;
    info_ptr = NULL;
  }
};
}
#endif // IMGSTREAM_INCLUDE_IMGSTEAM_PNGSTREAM_H_
