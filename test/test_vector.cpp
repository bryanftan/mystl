#include "container/vector.h"
#include <gtest/gtest.h>

TEST(VectorTests, PushBack) {
  mystl::vector<int> v{};
  for (int i{0}; i < 10; i++) {
    v.push_back(i);
  }
  EXPECT_EQ(v.size(), 10);
  EXPECT_EQ(v[9], 9);
}

TEST(VectorTests, PushBackMove) {
  mystl::vector<std::string> v{};
  for (int i{0}; i < 10; i++) {
    std::string s = "123";
    v.push_back(std::move(s));
  }
  EXPECT_EQ(v.size(), 10);
  EXPECT_EQ(v[0], "123");
}

TEST(VectorTests, PushBackPopBack) {
  mystl::vector<int> v{};
  v.push_back(1);
  v.pop_back();
  EXPECT_TRUE(v.empty());
}

TEST(VectorTests, EmplaceBack) {
  mystl::vector<std::pair<int, int>> v{};
  v.emplace_back(1, 2);
  EXPECT_EQ(v[0].first, 1);
  EXPECT_EQ(v[0].second, 2);
}
