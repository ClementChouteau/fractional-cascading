#include <gtest/gtest.h>

#include "fractional_cascading.hpp"
#include "naive_binary_search.hpp"
#include "generation.hpp"

struct TestCase
{
  std::size_t numberOfLists;
  std::size_t numberOfItems;
  std::size_t repetitions;
};

class FractionalCascadingTest : public ::testing::TestWithParam<TestCase>
{
};

TEST(FractionalCascadingTest, simpleTest)
{
  std::vector<std::vector<long>> input =
  {
    {37},
    {51},
    {92},
    {65},
    {83},
    {67}
  };

  const auto cascade = FractionalCascading(std::move(input));

  EXPECT_EQ(cascade.count(37), 1);
  EXPECT_EQ(cascade.count(51), 1);
  EXPECT_EQ(cascade.count(92), 1);
  EXPECT_EQ(cascade.count(65), 1);
  EXPECT_EQ(cascade.count(83), 1);
  EXPECT_EQ(cascade.count(67), 1);

  EXPECT_EQ(cascade.count(100), 0);
}

TEST_P(FractionalCascadingTest, shouldFindSameResultsAsNaive)
{
  auto input = generate_input(GetParam().numberOfLists, GetParam().numberOfItems);
  const auto binarySearch = BinarySearch<long>(input);
  const auto cascade = FractionalCascading<long>(input);

  for (long i=0; i<GetParam().repetitions; i++)
  {
    long key = keysDistribution(rng);
    EXPECT_EQ(binarySearch.count(key), cascade.count(key));
  }
}

INSTANTIATE_TEST_CASE_P(
  FractionalCascadingTestInputs,
  FractionalCascadingTest,
  ::testing::Values(
    // Edge cases of no lists or empty lists
    TestCase{0, 0, 1},
    TestCase{1, 0, 1},
    TestCase{2, 0, 1},
    TestCase{3, 0, 1},
    TestCase{10, 1, 10000},
    // Small cases
    TestCase{1, 10, 10000},
    TestCase{2, 10, 10000},
    TestCase{3, 10, 10000},
    TestCase{4, 10, 10000},
    TestCase{5, 10, 10000},
    TestCase{6, 10, 10000},
    TestCase{7, 10, 10000},
    TestCase{8, 10, 10000},
    TestCase{9, 10, 10000},
    TestCase{10, 10, 10000},
    // Large cases
    TestCase{100, 100,  1000},
    TestCase{1000, 100,  100}
  )
);
