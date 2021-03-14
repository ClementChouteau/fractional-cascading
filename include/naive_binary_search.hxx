#pragma once

template<typename KeyType>
template<typename Input>
BinarySearch<KeyType>::BinarySearch(Input&& input)
: _input(std::forward<Input>(input))
{
  for (auto& line : _input)
    std::sort(line.begin(), line.end());
}

template<typename KeyType>
void BinarySearch<KeyType>::lower_bound(KeyType key, std::function<void(KeyType)> onFound) const
{
  for (const auto& list : _input)
  {
    const auto it = std::lower_bound(list.begin(), list.end(), key);
    if (it != list.end())
      onFound(*it);
  }
}

template<typename KeyType>
std::size_t BinarySearch<KeyType>::count(KeyType key) const
{
  std::size_t count = 0;
  lower_bound(key, [&](KeyType lower){ if (lower == key) count++; });
  return count;
}
