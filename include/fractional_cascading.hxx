#pragma once

template<typename KeyType>
FractionalCascading<KeyType>::FractionalCascading(const std::vector<std::vector<KeyType>>& input)
: _cascade(build_fractional_cascading(input))
{
}

template<typename KeyType>
std::size_t FractionalCascading<KeyType>::count(KeyType key) const
{
  std::size_t count = 0;
  for (auto* lowerPtr : lower_bound_iterator(key))
    if (lowerPtr && *lowerPtr == key)
      count++;
  return count;
}

template<typename KeyType>
FractionalCascadingIterable<KeyType, LowerBoundSearch<KeyType>> FractionalCascading<KeyType>::lower_bound_iterator(KeyType key) const
{
  return {*this, key};
};

template<typename KeyType>
FractionalCascadingIterable<KeyType, UpperBoundSearch<KeyType>> FractionalCascading<KeyType>::upper_bound_iterator(KeyType key) const
{
  return {*this, key};
};

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
