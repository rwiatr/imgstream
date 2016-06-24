#include "imgstream/pngstream.h"
#include "imgstream/monoostream.h"
#include "gtest/gtest.h"

namespace imgstream {
class monoTest : public ::testing::Test {};

TEST_F(monoTest, mono_writes_istream_reads) {
  pngostream<> postr("mono_writes_istream_reads.png", 8, 4);
  monoostream<pngostream<>> mono(postr);

  for (size_t i = 0; i < 4; i++) {
    mono << uint8_t(0) << uint8_t(31) << uint8_t(63) << uint8_t(127) 
         << uint8_t(255) << uint8_t(1) << uint8_t(32) << uint8_t(64);
  }

  postr.close();

  pngistream<> pistr("mono_writes_istream_reads.png");

  ASSERT_EQ(size_t(8), pistr.get_width());
  ASSERT_EQ(size_t(4), pistr.get_height());

  rgb val;

  for (size_t i = 0; i < 4; i++) {
    pistr >> val;
    EXPECT_EQ(val, rgb(0, 0, 0)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(31, 31, 31)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(63, 63, 63)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(127, 127, 127)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(255, 255, 255)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(1, 1, 1)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(32, 32, 32)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(64, 64, 64)) << "failed for iteration " << i;
  }
}
}