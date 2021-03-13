#pragma once

#include <cassert>
#include <vector>
#include <functional>

#include "cascading_element.hpp"

template <typename T>
using ElementIt = typename std::vector<Element<T>>::const_iterator;

template<typename KeyType>
class FractionalCascading
{
public:
  FractionalCascading(const std::vector<std::vector<KeyType>>& input);

  void lower_bound(KeyType key, std::function<void(KeyType)> onFound) const;

  std::size_t count(KeyType key) const;

private:
  std::vector<std::vector<Element<KeyType>>> build_fractional_cascading(const std::vector<std::vector<KeyType>>& input);

  inline ElementIt<KeyType> get_nearest_lower_upper_promoted(
    ElementIt<KeyType> begin, ElementIt<KeyType> end,
    ElementIt<KeyType> lower) const;

  const std::vector<std::vector<Element<KeyType>>> _cascade;
};

#include "fractional_cascading.hxx"
