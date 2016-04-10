#include <gtest/gtest.h>
#include <iostream>

#include <torrentsync/utils/Buffer.h>



using namespace torrentsync::utils;

TEST(Buffer, initialize_and_destroy) {
  Buffer buff1;
  Buffer buff2 = makeBuffer("abcdef");
  Buffer buff4 = makeBuffer("abcdef");
  buff4.resize(2);

  ASSERT_TRUE(buff1.empty());
  EXPECT_EQ(buff1.size(), 0);
  ASSERT_TRUE(!buff2.empty());
  EXPECT_EQ(buff2.size(), 6);
  ASSERT_TRUE(!buff4.empty());
  EXPECT_EQ(buff4.size(), 2);
}

TEST(Buffer, no_diff_between_cstr_and_str) {
  std::string str("aa");
  Buffer buff = makeBuffer("aa");

  ASSERT_TRUE(buff == str);
}

TEST(Buffer, delete_with_resize) {
  Buffer buff = makeBuffer("abc");
  buff.resize(2);
  ASSERT_TRUE(buff == "ab");
}

TEST(Buffer, resize) {
  Buffer buff = makeBuffer("abcdefg");

  buff.resize(2);
  EXPECT_EQ(memcmp("ab", buff.data(), 2), 0);
  ASSERT_TRUE(!buff.empty());
  EXPECT_EQ(buff.size(), 2);

  buff.resize(3);
  EXPECT_EQ(memcmp("ab", buff.data(), 2), 0);
  ASSERT_TRUE(!buff.empty());
  EXPECT_EQ(buff.size(), 3);

  buff[2] = 'f';
  EXPECT_EQ(memcmp("abf", buff.data(), 3), 0);
}

TEST(Buffer, parseHex) {
  Buffer buff;

  buff = parseIDFromHex("0000000000000000000000000000000000000000");

  std::for_each(buff.begin(), buff.end(),
                [&](uint8_t t) { EXPECT_EQ(t, 0); });

  buff = parseIDFromHex("0000000000000000000000000000000000000001");

  std::for_each(buff.begin(), buff.begin() + 19,
                [&](uint8_t t) { EXPECT_EQ(t, 0); });
  EXPECT_EQ(buff[19], 1);

  buff = parseIDFromHex("0000000000000000000000000000000000000100");
  std::for_each(buff.begin(), buff.begin() + 18,
                [&](uint8_t t) { EXPECT_EQ(t, 0); });
  EXPECT_EQ(buff[18], 1);
  EXPECT_EQ(buff[19], 0);

  buff = parseIDFromHex("0000000000000000000000000000000100000000");
  EXPECT_EQ(buff[15], 1);

  buff = parseIDFromHex("1000000000000000000000000000001F00000000");
  EXPECT_EQ(buff[0], 0x10);
  EXPECT_EQ(buff[15], 0x1F);
}

TEST(Buffer, toString_m) {
  Buffer buff;

  buff = parseIDFromHex("4747474747474747474747474747474747474747");
  auto str = toString(buff);

  EXPECT_EQ(str, "GGGGGGGGGGGGGGGGGGGG");
}


