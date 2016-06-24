#ifndef IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_
#define IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_ 1

#include <stdint.h>

using namespace std;
namespace imgstream {
struct rgb { 
	uint8_t r, g, b; 
	rgb(){}
	rgb(uint8_t r, uint8_t g, uint8_t b): r(r), g(g), b(b) {}
};

inline bool operator==(const rgb& t, const rgb& o) {
	return t.r == o.r && t.g == o.g && t.b == o.b;
}

template<typename ostream_t>
class imgostream {
public:
  virtual ostream_t& operator<<(const uint8_t& byte) = 0;
  virtual ostream_t& operator<<(const rgb& color) = 0;
};

template<typename istream_t>
class imgistream {
public:
  virtual istream_t& operator>>(uint8_t& byte) = 0;
  virtual istream_t& operator>>(rgb& color) = 0;
  virtual size_t get_width() = 0;
  virtual size_t get_height() = 0;
};
}

#endif // IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_
