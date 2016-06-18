#ifndef IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_
#define IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_ 1

#include <stdint.h>

using namespace std;
namespace imgstream {
struct rgb { uint8_t r, g, b; };

template<typename ostream_t>
class imgostream {
public:
  virtual ostream_t& operator<<(uint8_t byte) = 0;
  virtual ostream_t& operator<<(const rgb& color) = 0;
};
}

#endif // IMGSTREAM_INCLUDE_IMGSTEAM_IMGOSTREAM_H_
