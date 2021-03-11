#pragma once

#include <cassert>
#include <vector>
#include <functional>

#include "cascading_element.hpp"

template<typename KeyType>
class FractionalCascading
{
public:
  FractionalCascading(const std::vector<std::vector<KeyType>>& input);

  void binary_search(KeyType key, std::function<void(KeyType)> onFound) const;

  std::size_t count(KeyType key) const;

private:
  std::vector<std::vector<Element<KeyType>>> build_fractional_cascading(const std::vector<std::vector<KeyType>>& input);

  inline std::pair<const Element<KeyType>*, const Element<KeyType>*> get_nearest_lower_upper_promoted(const Element<KeyType>* first, const Element<KeyType>* last, const Element<KeyType>* upper) const;

  const std::vector<std::vector<Element<KeyType>>> _cascade;
};

#include "fractional_cascading.hxx"
