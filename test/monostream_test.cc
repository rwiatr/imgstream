#include "imgstream/pngstream.h"
#include "imgstream/monoostream.h"
#include "gtest/gtest.h"

namespace imgstream {
class monoTest : public ::testing::Test {};

TEST_F(monoTest, write_to_buffer) {
  pngostream<> postr("write_to_buffer.png", 400, 400);
  monoostream<pngostream<>> mono(postr);

  mono << uint8_t(1) << uint8_t(2) << uint8_t(3) << uint8_t(5) << uint8_t(6);
}
}