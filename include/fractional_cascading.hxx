#pragma once

template<typename KeyType>
FractionalCascading<KeyType>::FractionalCascading(const std::vector<std::vector<KeyType>>& input)
: _cascade(build_fractional_cascading(input))
{
}

template<typename KeyType>
void FractionalCascading<KeyType>::lower_bound(KeyType key, std::function<void(KeyType)> onFound) const
{
  if (_cascade.empty() || _cascade[0].empty())
    return;

  // std::pair containing a pair of iterators defining the wanted range,
  // the first pointing to the first element that is not less than value (>= value)
  // and the second pointing to the first element greater than value. (>value)
  // elements in [first, last[ == key, and may be promoted or not

  // If there are no elements not less than value, last is returned as the first element.
  // Similarly if there are no elements greater than value, last is returned as the second element
  auto lower = std::lower_bound(_cascade[0].begin(), _cascade[0].end(), key);

  if (lower != _cascade[0].end())
  {
    if (!lower->isPromoted())
      onFound(lower->value);
    else if (lower->next)
      onFound(lower->next->value);
  }

  lower = get_nearest_lower_upper_promoted(_cascade[0].begin(), _cascade[0].end(), lower);

  for (std::size_t i=1; i<_cascade.size(); ++i)
  {
    if (_cascade[i].empty())
      return;

    // From equal_range of i-1, deduce equal_range of i
    if (lower == _cascade[i-1].end())
      lower = _cascade[i].end();
    else
      lower = _cascade[i].begin() + (lower->out - &_cascade[i][0]);
    std::size_t count = 0;
    while (lower != _cascade[i].begin() && (lower-1)->value >= key)
      lower--;
    while (lower != _cascade[i].end() && lower->value < key)
      lower++;

    assert(lower == std::lower_bound(_cascade[i].begin(), _cascade[i].end(), key));

    if (lower != _cascade[i].end())
    {
      if (!lower->isPromoted())
        onFound(lower->value);
      else if (lower->next)
        onFound(lower->next->value);
    }

    lower = get_nearest_lower_upper_promoted(_cascade[i].begin(), _cascade[i].end(), lower);
  }
}

template<typename KeyType>
std::size_t FractionalCascading<KeyType>::count(KeyType key) const
{
  std::size_t count = 0;
  lower_bound(key, [&](KeyType lower){ if (lower == key) count++; });
  return count;
}

template<typename KeyType>
std::vector<std::vector<Element<KeyType>>> FractionalCascading<KeyType>::build_fractional_cascading(const std::vector<std::vector<KeyType>>& input)
{
  std::vector<std::vector<Element<KeyType>>> output;
  if (input.empty())
    return output;

  output.resize(input.size());
  output.back().resize(input.back().size());

  std::vector<KeyType> input_sorted(input.back());
  std::sort(input_sorted.begin(), input_sorted.end());

  std::vector<Element<KeyType>*> promoted;
  for (std::size_t i=0; i < input_sorted.size(); ++i)
  {
    auto& element = output.back()[i];
    if ((i & 1) == 0)
      promoted.push_back(&element);

    element.value = input_sorted[i];
    element.prev = nullptr;
    element.next = nullptr;
    element.out = nullptr;
  }

  std::vector<Element<KeyType>*> next_promoted;
  for (auto k = (long) input.size() - 2; k >= 0; --k)
  {
    input_sorted = input[k];
    std::sort(input_sorted.begin(), input_sorted.end());
    output[k].reserve(input_sorted.size() + promoted.size());

    std::size_t i = 0;
    std::size_t j = 0;

    Element<KeyType>* prev_promoted_value = nullptr;
    Element<KeyType>* prev_value = nullptr;
    while (i < input_sorted.size() || j < promoted.size())
    {
      // We must make sure that the non promoted are always before the promoted

      // Lower (or equal) value is a non promoted, or promoted are exhausted
      if (i < input_sorted.size() && (j >= promoted.size() || input_sorted[i] <= promoted[j]->value))
      {
        output[k].push_back(Element<KeyType>());
        auto& element = output[k].back();

        if (((i+j) & 1) == 0)
          next_promoted.push_back(&element);

        prev_value = &element;
        element.prev = prev_promoted_value;
        // Next is filled during the next pass
        element.out = nullptr; // Non promoted element here
        element.value = input_sorted[i];
        i++;
      }

      // Lower (strict) value is a promoted, or non promoted are exhausted
      while (j < promoted.size() && (i >= input_sorted.size() || promoted[j]->value < input_sorted[i]))
      {
        output[k].push_back(Element<KeyType>());
        auto& element = output[k].back();

        if (((i+j) & 1) == 0)
          next_promoted.push_back(&element);

        prev_promoted_value = &element;
        element.prev = prev_value;
        // Next is filled during the next pass
        element.out = promoted[j];
        element.value = promoted[j]->value;
        j++;
      }
    }

    // Another pass to fill next
    Element<KeyType>* next_promoted_value = nullptr;
    Element<KeyType>* next_value = nullptr;
    for (auto it = output[k].rbegin(); it != output[k].rend(); ++it)
    {
      if (it->isPromoted())
      {
        it->next = next_value;
        next_promoted_value = &*it;
      }
      else
      {
        it->next = next_promoted_value;
        next_value = &*it;
      }
    }

    promoted = std::move(next_promoted);
    next_promoted.clear();
  }

  return output;
}

template<typename KeyType>
inline ElementIt<KeyType> FractionalCascading<KeyType>::get_nearest_lower_upper_promoted(
  ElementIt<KeyType> begin, ElementIt<KeyType> end,
  ElementIt<KeyType> lower) const
{
  // elements in [first, last[ == key, but they are not necessarily promoted

  if (lower != end)
  {
    if (!lower->isPromoted())
    {
      if (lower->prev)
        lower = begin + (lower->prev - &*begin);
      else if (lower->next)
        lower = begin + (lower->next - &*begin);
      else
        lower = end;
    }
    // else ok
  }
  assert(lower == end || lower->isPromoted());

  return lower;
}
