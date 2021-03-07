#pragma once

#include <cassert>
#include <vector>
#include <functional>

#include "cascading_element.hpp"

class FractionalCascading
{
public:
  FractionalCascading(const std::vector<std::vector<long>>& input);

  void binary_search(long key, std::function<void(long)> onFound) const;

  std::size_t count(long key) const;

private:
  std::vector<std::vector<Element>> build_fractional_cascading(const std::vector<std::vector<long>>& input);

  inline std::pair<const Element*, const Element*> get_nearest_lower_upper_promoted(const Element* first, const Element* last, const Element* upper) const;

  const std::vector<std::vector<Element>> _cascade;
};

#include "fractional_cascading.hxx"
