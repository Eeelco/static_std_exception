#include "exception_types.h"
#include <bitset>
#include <codecvt>
#include <dlfcn.h>
#include <gtest/gtest.h>
#include <locale>
#include <map>
#include <string>
#include <vector>

static bool g_forbid_malloc;

void *malloc(size_t size) {
  static void *(*real_malloc)(size_t) = nullptr;
  if (!real_malloc) {
    real_malloc = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
  }
  void *p = real_malloc(size);
  if (g_forbid_malloc) {
    fprintf(stderr, "malloc(%d) = %p\n", static_cast<int>(size), p);
    std::terminate();
  }
  return p;
}

class ExceptionTest : public ::testing::Test {
protected:
  void SetUp() override { g_forbid_malloc = false; }
  void TearDown() override { g_forbid_malloc = false; }
};

TEST_F(ExceptionTest, StackString_SmallString_CompletelyCopied) {
  std::string s("hello");
  StackString ss(s.c_str());

  EXPECT_STREQ(s.c_str(), ss.c_str());
}

TEST_F(ExceptionTest, StackString_MaximumLength_CompletelyCopied) {
  std::string s(stackBufferSize - 1, 'a');
  StackString ss(s.c_str());

  EXPECT_STREQ(s.c_str(), ss.c_str());
}

TEST_F(ExceptionTest, StackString_TooLongString_Truncated) {
  std::string s(stackBufferSize, 'a');
  StackString ss(s.c_str());

  std::string result(ss.c_str());

  EXPECT_EQ(stackBufferSize - 1, result.size());
  EXPECT_TRUE(result.ends_with(truncatedMessage));
}

TEST_F(ExceptionTest, Malloc_ForbiddenMalloc_Terminates) {
  auto test = []() {
    g_forbid_malloc = true;
    std::ignore = malloc(1);
  };
  EXPECT_DEATH(test(), "malloc\\(1\\)");
}

TEST_F(ExceptionTest, BitsetCtor_StringcontainsTwo_ThrowsInvalidArgument) {
  g_forbid_malloc = true;
  try {
    std::bitset<3> bs("012");
  } catch (const std::range_error &e) {
    // Make sure we are only catching the expected exception
    g_forbid_malloc = false;
    FAIL();
  } catch (const std::invalid_argument &e) {
    g_forbid_malloc = false;
    SUCCEED();
  } catch (...) {
    g_forbid_malloc = false;
    FAIL();
  }
}

TEST_F(ExceptionTest, VectorReserve_NegativeSize_ThrowsLengthError) {
  g_forbid_malloc = true;
  try {
    std::vector<int> v;
    v.reserve(-1);
  } catch (const std::length_error &e) {
    g_forbid_malloc = false;
    SUCCEED();
  } catch (...) {
    g_forbid_malloc = false;
    FAIL();
  }
}

TEST_F(ExceptionTest, MapAt_IndexOutOfRange_ThrowsOutOfRange) {
  g_forbid_malloc = true;

  try {
    std::map<int, int> m;
    m.at(5);
  } catch (const std::out_of_range &e) {
    g_forbid_malloc = false;
    SUCCEED();
  } catch (...) {
    g_forbid_malloc = false;
    FAIL();
  }
}

TEST_F(ExceptionTest, VectorAt_IndexOutOfRange_ThrowsFormattingOutOfRange) {
  g_forbid_malloc = true;

  try {
    std::vector<int> v;
    std::ignore = v.at(5);
  } catch (const std::out_of_range &e) {
    g_forbid_malloc = false;
    SUCCEED();
  } catch (...) {
    g_forbid_malloc = false;
    FAIL();
  }
}

TEST_F(ExceptionTest, LocaleCtor_Nullptr_ThrowsRuntimeError) {
  g_forbid_malloc = true;

  try {
    std::locale(static_cast<const char *>(nullptr));
  } catch (const std::runtime_error &e) {
    g_forbid_malloc = false;
    SUCCEED();
  } catch (...) {
    g_forbid_malloc = false;
    FAIL();
  }
}
