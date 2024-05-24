#include <gtest/gtest.h>
#include "mystl/container/vector.h"

TEST(VectorTests, PushBack) {
    mystl::vector<int> v{};
    v.push_back(1);
}