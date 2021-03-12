#pragma once

template<typename KeyType>
FractionalCascading<KeyType>::FractionalCascading(const std::vector<std::vector<KeyType>>& input)
: _cascade(build_fractional_cascading(input))
{
}

template<typename KeyType>
void FractionalCascading<KeyType>::binary_search(KeyType key, std::function<void(KeyType)> onFound) const
{
  if (_cascade.empty() || _cascade[0].empty())
    return;

  const auto it = std::lower_bound(_cascade[0].begin(), _cascade[0].end(), key);
  const auto* lower_bound = (it != _cascade[0].end()) ? &*it : nullptr;

  if (lower_bound && lower_bound->value == key && !lower_bound->isPromoted())
    onFound(key);

  auto [lower, upper] = get_nearest_lower_upper_promoted(&_cascade[0].front(), &_cascade[0].back(), lower_bound);

  for (std::size_t i=1; i<_cascade.size(); ++i)
  {
    if (_cascade[i].empty())
      return;

    if (lower == nullptr)
      lower = &_cascade[i][0];
    else
      lower = lower->out;
    if (upper == nullptr)
      upper = &_cascade[i].back();
    else
      upper = upper->out;

    assert((upper - lower) <= 3);
    for (const auto* ptr = lower; ptr <= upper; ptr++)
      if (ptr->value == key)
      {
        if (!ptr->isPromoted())
          onFound(ptr->value);
        std::tie(lower, upper) = get_nearest_lower_upper_promoted(&_cascade[i].front(), &_cascade[i].back(), ptr);
        goto next_line;
      }
      else if  (ptr->value > key)
      {
        std::tie(lower, upper) = get_nearest_lower_upper_promoted(&_cascade[i].front(), &_cascade[i].back(), ptr);
        goto next_line;
      }

    std::tie(lower, upper) = get_nearest_lower_upper_promoted(&_cascade[i].front(), &_cascade[i].back(), nullptr);

    next_line:;
  }
}

template<typename KeyType>
std::size_t FractionalCascading<KeyType>::count(KeyType key) const
{
  std::size_t count = 0;
  binary_search(key, [&count](KeyType){ count++; });
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
inline std::pair<const Element<KeyType>*, const Element<KeyType>*> FractionalCascading<KeyType>::get_nearest_lower_upper_promoted(const Element<KeyType>* first, const Element<KeyType>* last, const Element<KeyType>* upper) const
{
  const Element<KeyType>* lower = nullptr;
  if (upper)
  {
    if (upper->isPromoted())
    {
      if (upper != first)
      {
        lower = upper-1;
        if (!lower->isPromoted())
          lower = lower->prev;
      }
    }
    else
    {
      lower = upper->prev;
      upper = upper->next;
    }
  }
  else
  {
    lower = last;
    if (!lower->isPromoted())
      lower = lower->prev;
  }
  assert(lower == nullptr || lower->isPromoted());
  assert(upper == nullptr || upper->isPromoted());

  return std::make_pair(lower, upper);
}
