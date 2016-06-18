#include "imgstream/pngstream.h"
#include "gtest/gtest.h"

namespace imgstream {
class pngTest : public ::testing::Test {};

TEST_F(pngTest, write_to_buffer) {
  pngostream<> postr("write_to_buffer.png", 400, 400);
  postr << 1 << 2 << 3 << 5 << 6;
  EXPECT_EQ(1, postr.inspect(0));
  EXPECT_EQ(2, postr.inspect(1));
  EXPECT_EQ(3, postr.inspect(2));
  EXPECT_EQ(5, postr.inspect(3));
  EXPECT_EQ(6, postr.inspect(4));
}

TEST_F(pngTest, close_stream) {
  pngostream<> postr("close_stream.png", 2, 2);
  postr << 255 << 255 << 0 << 0;
  postr.close();
}
}
