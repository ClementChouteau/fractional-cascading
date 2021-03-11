#pragma once

#include <vector>
#include <algorithm>
#include <functional>

template<typename KeyType>
class BinarySearch
{
public:
  template<typename Input>
  BinarySearch(Input&& input);

  void binary_search(KeyType key, std::function<void(KeyType)> onFound) const;

  std::size_t count(KeyType key) const;

private:
  std::vector<std::vector<KeyType>> _input;
};

#include "naive_binary_search.hxx"
