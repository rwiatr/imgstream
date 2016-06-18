#ifndef IMGSTREAM_INCLUDE_IMGSTEAM_MONOSTREAM_H_
#define IMGSTREAM_INCLUDE_IMGSTEAM_MONOSTREAM_H_ 1

namespace imgstream {
template<typename ostream_t>
class monoostream {
  ostream_t &ostr;

public:

  monoostream(ostream_t &ostr) : ostr(ostr) {}

  monoostream& operator<<(uint8_t byte) {
  	rgb rgb_ = {byte, byte, byte};

    ostr << rgb_;

    return *this;
  }

  monoostream& operator<<(double val) {
    uint8_t byte;

    if (val < 0) byte = 0;
    else if (val >= 1) byte = 255;
    else byte = 256 * val;
    rgb rgb_ = {byte, byte, byte};
    
    ostr << rgb_;

    return *this;
  }
};
}

#endif // IMGSTREAM_INCLUDE_IMGSTEAM_MONOSTREAM_H_