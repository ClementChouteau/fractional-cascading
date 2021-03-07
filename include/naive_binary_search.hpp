#pragma once

#include <vector>
#include <algorithm>
#include <functional>

class BinarySearch
{
public:
  template<typename InputType>
  BinarySearch(InputType&& input);

  void binary_search(long key, std::function<void(long)> onFound) const;

  std::size_t count(long key) const;

private:
  std::vector<std::vector<long>> _input;
};

#include "naive_binary_search.hxx"
