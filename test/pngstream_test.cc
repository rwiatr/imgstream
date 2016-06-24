#include "imgstream/pngstream.h"
#include "gtest/gtest.h"

namespace imgstream {
class pngTest : public ::testing::Test {};

TEST_F(pngTest, ostream_writes_istream_reads) {
  pngostream<> postr("ostream_writes_istream_reads.png", 8, 4);
  for (size_t i = 0; i < 4; i++) {
    postr << 125 << 0 << 0;
    postr << 0 << 125 << 0;
    postr << 0 << 0 << 125;
    postr << 125 << 125 << 125;
    postr << 255 << 0 << 0;
    postr << 0 << 255 << 0;
    postr << 0 << 0 << 255;
    postr << 255 << 255 << 255;
  }
  postr.close();

  pngistream<> pistr("ostream_writes_istream_reads.png");

  ASSERT_EQ(size_t(8), pistr.get_width());
  ASSERT_EQ(size_t(4), pistr.get_height());

  rgb val;

  for (size_t i = 0; i < 4; i++) {
    pistr >> val;
    EXPECT_EQ(val, rgb(125, 0, 0)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(0, 125, 0)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(0, 0, 125)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(125, 125, 125)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(255, 0, 0)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(0, 255, 0)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(0, 0, 255)) << "failed for iteration " << i;
    pistr >> val;
    EXPECT_EQ(val, rgb(255, 255, 255)) << "failed for iteration " << i;
  }
}
}
