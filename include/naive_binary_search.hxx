#pragma once

template<typename InputType>
BinarySearch::BinarySearch(InputType&& input)
: _input(std::forward<InputType>(input))
{
  for (auto& line : _input)
    std::sort(line.begin(), line.end());
}

void BinarySearch::binary_search(long key, std::function<void(long)> onFound) const
{
  for (const auto& list : _input)
  {
    if (std::binary_search(list.begin(), list.end(), key))
      onFound(key);
  }
}

std::size_t BinarySearch::count(long key) const
{
  std::size_t count = 0;
  binary_search(key, [&count](long){ count++; });
  return count;
}
