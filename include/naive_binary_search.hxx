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
void BinarySearch<KeyType>::binary_search(KeyType key, std::function<void(KeyType)> onFound) const
{
  for (const auto& list : _input)
  {
    if (std::binary_search(list.begin(), list.end(), key))
      onFound(key);
  }
}

template<typename KeyType>
std::size_t BinarySearch<KeyType>::count(KeyType key) const
{
  std::size_t count = 0;
  binary_search(key, [&count](KeyType){ count++; });
  return count;
}
