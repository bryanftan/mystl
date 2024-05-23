#include <gtest/gtest.h>
#include "mystl/container/vector.h"

TEST(VectorTests, Test_PushBack) {
    mystl::vector<int> v{};
    v.push_back(1);
}