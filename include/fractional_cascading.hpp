#pragma once

#include <cassert>
#include <vector>
#include <functional>

#include "cascading_element.hpp"

template <typename T>
using ElementIt = typename std::vector<Element<T>>::const_iterator;

template<typename KeyType>
class LowerBoundSearch;
template<typename KeyType>
class UpperBoundSearch;

template<typename KeyType, typename SearchType>
class FractionalCascadingIterable;

template<typename KeyType>
class FractionalCascading
{
public:
  FractionalCascading(const std::vector<std::vector<KeyType>>& input);

  std::size_t count(KeyType key) const;

  FractionalCascadingIterable<KeyType, LowerBoundSearch<KeyType>> lower_bound_iterator(KeyType key) const;
  FractionalCascadingIterable<KeyType, UpperBoundSearch<KeyType>> upper_bound_iterator(KeyType key) const;

private:
  friend class FractionalCascadingIterable<KeyType, LowerBoundSearch<KeyType>>;
  friend class FractionalCascadingIterable<KeyType, UpperBoundSearch<KeyType>>;

  std::vector<std::vector<Element<KeyType>>> build_fractional_cascading(const std::vector<std::vector<KeyType>>& input);

  std::vector<std::vector<Element<KeyType>>> _cascade;
};


#include "fractional_cascading_iterator.hxx"
#include "fractional_cascading.hxx"
