#pragma once

#include <random>
#include <algorithm>

static std::random_device dev;
static std::mt19937 rng(dev());
static std::uniform_int_distribution<std::mt19937::result_type> keysDistribution(0, 100);

std::vector<std::vector<long>> generate_input(std::size_t N, std::size_t M)
{
  std::uniform_int_distribution<std::mt19937::result_type> listSizeDistribution(0, M);

  std::vector<std::vector<long>> input(N);
  for (auto& list : input)
  {
    list.resize(listSizeDistribution(rng));
    for (auto& x : list)
      x = keysDistribution(rng);
  }
  return input;
}
